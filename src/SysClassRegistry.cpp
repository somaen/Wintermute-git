/*
This file is part of WME Lite.
http://dead-code.org/redir.php?target=wmelite

Copyright (c) 2011 Jan Nedoma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "dcgf.h"
#include "SysClassRegistry.h"


int CSysClassRegistry::m_Count = 0;
bool CSysClassRegistry::m_Disabled = false;
CSysClass* CSysClassRegistry::m_Classes = NULL;

//////////////////////////////////////////////////////////////////////////
CSysClassRegistry::CSysClassRegistry()
{

}


//////////////////////////////////////////////////////////////////////////
CSysClassRegistry::~CSysClassRegistry()
{

}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::RegisterClass(CSysClass *Class)
{
	Class->m_ID = m_Count++;

	CSysClass* next = m_Classes;
	m_Classes = Class;
	m_Classes->m_Next = next;

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::UnregisterClass(CSysClass *Class)
{
	CSysClass* last = NULL;
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		if(c==Class){
			int NumInst = 0;
			CSysInstance* Inst = c->m_Inst;
			while(Inst!=NULL){
				NumInst++;
				Inst = Inst->m_Next;
			}
			if(NumInst!=0){
				char str[MAX_PATH];
				sprintf(str, "Memory leak@class %-20s: %d instance(s) left\n", c->m_Name, NumInst);
				CBPlatform::OutputDebugString(str);
			}

			if(!last) m_Classes = c->m_Next;
			else last->m_Next = c->m_Next;

			break;
		}
		last = c;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::RegisterInstance(const char* ClassName, void* Instance)
{
	if(m_Disabled) return true;
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		if(!strcmp(c->m_Name, ClassName)){
			c->AddInstance(Instance, m_Count++);
			//CSysInstance* inst = new CSysInstance(Instance, m_Count++);
			//c->m_Instances.Add(inst);
			break;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::UnregisterInstance(const char* ClassName, void* Instance)
{
	//if(Instance!=NULL && !((CBBase*)Instance)->m_Persistable) return true;
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		if(!strcmp(c->m_Name, ClassName)){
			//for(int i=0; i<c->m_Instances.GetSize(); i++){
			//	if(c->m_Instances[i]->m_Instance==Instance){
			//		delete c->m_Instances[i];
			//		c->m_Instances.RemoveAt(i);
			//		break;
			//	}			
			//}
			c->RemoveInstance(Instance);

			break;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::GetPointerID(void *Pointer, int *ClassID, int *InstanceID)
{
	if(Pointer==NULL) return true;
	
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){		
		
		for(CSysInstance* Inst=c->m_Inst; Inst!=NULL; Inst = Inst->m_Next){
			if(Inst->m_Instance==Pointer){
				*ClassID = c->m_ID;
				*InstanceID = Inst->m_ID;
				return true;
			}
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
void* CSysClassRegistry::IDToPointer(int ClassID, int InstanceID)
{
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		if(c->m_SavedID==ClassID){
			CSysInstance* Inst = c->m_Inst;
			while(Inst!=NULL){
				if(Inst->m_SavedID==InstanceID)
				{
					Inst->m_Used = true;
					return Inst->m_Instance;
				}
				Inst = Inst->m_Next;
			}
			break;
		}
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSysClassRegistry::SaveTable(CBGame* Game, CBPersistMgr *PersistMgr)
{
	CSysClass* c;

	int NumClasses = 0;
	for(c=m_Classes; c!=NULL; c=c->m_Next) NumClasses++;

	PersistMgr->PutDWORD(NumClasses);

	int CurrentClass = 0;
	for(c=m_Classes; c!=NULL; c=c->m_Next){
		CurrentClass++;
		Game->m_IndicatorProgress = 50.0f / (float)((float)NumClasses / (float)CurrentClass);
		Game->DisplayContent(false);
		Game->m_Renderer->Flip();

		PersistMgr->PutString(c->m_Name);
		PersistMgr->PutDWORD(c->m_ID);

		int NumInst = 0;
		CSysInstance* Inst = c->m_Inst;
		while(Inst!=NULL){
			NumInst++;
			Inst = Inst->m_Next;
		}

		PersistMgr->PutDWORD(NumInst);

		Inst = c->m_Inst;
		while(Inst!=NULL){
			PersistMgr->PutDWORD(Inst->m_ID);
			Inst = Inst->m_Next;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSysClassRegistry::LoadTable(CBGame* Game, CBPersistMgr *PersistMgr)
{
	//MarkInstances(false);

	int i;
	CSysInstance* Inst;

	// reset SavedID of current instances
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next)
	{
		Inst = c->m_Inst;
		while(Inst!=NULL){
			Inst->m_SavedID = -1;
			Inst = Inst->m_Next;
		}
	}

	// TEST
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next)
	{
		if(c->m_Persistent) continue;
		c->RemoveAllInstances();
	}



	int NumClasses = PersistMgr->GetDWORD();

	for(i=0; i<NumClasses; i++){
		
		Game->m_IndicatorProgress = 50.0f / (float)((float)NumClasses / (float)i);
		Game->DisplayContentSimple();
		Game->m_Renderer->Flip();
				

		char* str = PersistMgr->GetString();
		for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
			if(!strcmp(c->m_Name, str)){
				c->m_SavedID = PersistMgr->GetDWORD();
				int NumInstances = PersistMgr->GetDWORD();
				for(int j=0; j<NumInstances; j++){
					// persistent class, just map instance (there SHOULD at most be one instance)
					if(c->m_Persistent){
						if(j>0) Game->LOG(0, "Warning: attempting to load multiple instances of persistent class %s (%d)", c->m_Name, NumInstances);
						
						int inst_id = PersistMgr->GetDWORD();

						CSysInstance* Inst = c->GetInstanceAt(j);
						if(Inst!=NULL) Inst->m_SavedID = inst_id;
						else Game->LOG(0, "Warning: instance %d of persistent class %s not found", j, c->m_Name);
					}
					// normal instances, create empty objects
					else{
						void* empty_object = c->m_Build();
						for(int k=0; k<c->GetNumInstances(); k++){
							CSysInstance* Inst = c->GetInstanceAt(k);
							if(Inst->m_Instance==empty_object){
								Inst->m_SavedID = PersistMgr->GetDWORD();
								//c->m_Instances[k]->m_ID = m_Count++;
								break;
							}
						}
					}
				}
				break;
			}
		}
	}

	//WeedUnusedInstances();
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSysClassRegistry::SaveInstances(CBGame* Game, CBPersistMgr *PersistMgr)
{
	CSysClass* c;
	HRESULT ret;

	// count total instances
	int NumInstances = 0;
	int NumClasses = 0;
	for(c=m_Classes; c!=NULL; c=c->m_Next){
		NumInstances+=c->GetNumInstances();
		NumClasses++;
	}
	PersistMgr->PutDWORD(NumInstances);

	int CurrentClass = 0;
	for(c=m_Classes; c!=NULL; c=c->m_Next){
		CurrentClass++;

		if(CurrentClass%20 == 0){
			Game->m_IndicatorProgress = 50 + 50.0f / (float)((float)NumClasses / (float)CurrentClass);
			Game->DisplayContent(false);
			Game->m_Renderer->Flip();
		}
		Game->MiniUpdate();
		
		CSysInstance* Inst = c->m_Inst;
		while(Inst!=NULL){
			// write instace header
			PersistMgr->PutDWORD(c->m_ID);
			PersistMgr->PutDWORD(Inst->m_ID);

			if(FAILED(ret=c->m_Load(Inst->m_Instance, PersistMgr))) return ret;

			Inst = Inst->m_Next;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSysClassRegistry::LoadInstances(CBGame* Game, CBPersistMgr *PersistMgr)
{
	CSysClass* c;
	HRESULT ret;

	int ClassID, InstanceID;

	// get total instances
	int NumInstances = PersistMgr->GetDWORD();

	for(int i=0; i<NumInstances; i++){
		
		if(i%20 == 0)
		{
			Game->m_IndicatorProgress = 50 + 50.0f / (float)((float)NumInstances / (float)i);
			Game->DisplayContentSimple();
			Game->m_Renderer->Flip();
		}

		ClassID = PersistMgr->GetDWORD();
		InstanceID = PersistMgr->GetDWORD();
		void* instance = IDToPointer(ClassID, InstanceID);

		for(c=m_Classes; c!=NULL; c=c->m_Next){
			if(c->m_SavedID==ClassID){
				if(FAILED(ret=c->m_Load(instance, PersistMgr))) return ret;
				break;
			}
		}		
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSysClassRegistry::EnumInstances(SYS_INSTANCE_CALLBACK lpCallback, const char* ClassName, void *lpData)
{
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		if(!strcmp(c->m_Name, ClassName)){

			/*
			for(int i=0; i<c->m_Instances.GetSize(); i++){
				lpCallback(c->m_Instances[i]->m_Instance, lpData);
			}
			*/
			CSysInstance* Inst = c->m_Inst;
			while(Inst!=NULL){
				lpCallback(Inst->m_Instance, lpData);
				Inst = Inst->m_Next;
			}
			break;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::MarkInstances(bool Used)
{
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		CSysInstance* Inst = c->m_Inst;
		while(Inst!=NULL){
			Inst->m_Used = Used;
			Inst = Inst->m_Next;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSysClassRegistry::WeedUnusedInstances()
{
	for(CSysClass* c=m_Classes; c!=NULL; c=c->m_Next){
		CSysInstance* Inst = c->m_Inst;
		CSysInstance* LastInst = NULL;
		while(Inst!=NULL){
			if(!Inst->m_Used)
			{
				if(LastInst==NULL) c->m_Inst = Inst->m_Next;
				else LastInst->m_Next = Inst->m_Next;

				CSysInstance* Temp = Inst;
				Inst = Inst->m_Next;
				delete Temp;
			}
			else
			{
				LastInst = Inst;
				Inst = Inst->m_Next;
			}
		}
	}
	return true;
}