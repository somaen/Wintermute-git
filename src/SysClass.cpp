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
#include "SysClass.h"



//////////////////////////////////////////////////////////////////////////
CSysClass::CSysClass(const char* name, PERSISTBUILD build, PERSISTLOAD load, bool persistent_class)
{
	m_Name = new char[strlen(name)+1];
	strcpy(m_Name, name);

	m_Build = build;
	m_Load = load;
	m_Next = NULL;
	m_SavedID = -1;
	m_Persistent = persistent_class;
	m_Inst = NULL;
	m_NumInst = 0;

	CSysClassRegistry::RegisterClass(this);
}


//////////////////////////////////////////////////////////////////////////
CSysClass::~CSysClass()
{
	CSysClassRegistry::UnregisterClass(this);
/*
	for(int i=0; i<m_Instances.GetSize(); i++) delete m_Instances[i];
	m_Instances.RemoveAll();
*/

	CSysInstance* Inst = m_Inst;
	while(Inst!=NULL){
		CSysInstance* Temp = Inst;
		Inst = Inst->m_Next;
		delete Temp;
	}
	m_NumInst = 0;


	delete [] m_Name;
}

//////////////////////////////////////////////////////////////////////////
bool CSysClass::RemoveAllInstances()
{
	CSysInstance* Inst = m_Inst;
	while(Inst!=NULL){
		CSysInstance* Temp = Inst;
		Inst = Inst->m_Next;
		delete Temp;
	}
	m_NumInst = 0;
	m_Inst = NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CSysClass::AddInstance(void *Instance, int ID)
{
	CSysInstance* inst = new CSysInstance(Instance, ID);
	
	//m_Instances.Add(inst);

	inst->m_Next = m_Inst;
	m_Inst = inst;

	m_NumInst++;

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClass::RemoveInstance(void *Instance)
{
	CSysInstance* Inst = m_Inst;
	if(Inst && Inst->m_Instance==Instance){
		m_Inst = Inst->m_Next;
		delete Inst;
		m_NumInst--;
		return true;
	}
	while(Inst!=NULL){

		if(Inst->m_Next && Inst->m_Next->m_Instance==Instance){
			CSysInstance* Temp = Inst->m_Next;
			Inst->m_Next = Inst->m_Next->m_Next;
			delete Temp;
			m_NumInst--;
			return true;
		}
		else Inst = Inst->m_Next;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
CSysInstance* CSysClass::GetInstanceAt(int Index)
{
	int Count = 0;
	CSysInstance* Inst = m_Inst;
	while(Inst!=NULL){
		if(Count==Index) return Inst;

		Count++;
		Inst = Inst->m_Next;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
int CSysClass::GetNumInstances()
{
	return m_NumInst;
}
