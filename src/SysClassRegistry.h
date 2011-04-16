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

#ifndef __WmeSysClassRegistry_H__
#define __WmeSysClassRegistry_H__


class CBGame;

class CSysClassRegistry  
{
public:
	static HRESULT EnumInstances(SYS_INSTANCE_CALLBACK lpCallback, const char* ClassName, void* lpData);
	static HRESULT LoadTable(CBGame* Game, CBPersistMgr* PersistMgr);
	static HRESULT SaveTable(CBGame* Game, CBPersistMgr* PersistMgr);
	static HRESULT LoadInstances(CBGame* Game, CBPersistMgr* PersistMgr);
	static HRESULT SaveInstances(CBGame* Game, CBPersistMgr* PersistMgr);
	static void* IDToPointer(int ClassID, int InstanceID);
	static bool GetPointerID(void* Pointer, int* ClassID, int* InstanceID);
	static bool RegisterClass(CSysClass* Class);
	static bool UnregisterClass(CSysClass* Class);
	static bool RegisterInstance(const char* ClassName, void* Instance);
	static bool UnregisterInstance(const char* ClassName, void* Instance);
	static bool MarkInstances(bool Used);
	static bool WeedUnusedInstances();
	CSysClassRegistry();
	virtual ~CSysClassRegistry();

	static bool m_Disabled;
	static int m_Count;
	static CSysClass* m_Classes;
};

#endif
