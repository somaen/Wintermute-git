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

#ifndef __WmeBPersistMgr_H__
#define __WmeBPersistMgr_H__


#include "BBase.h"

namespace WinterMute {

class Vector2;

class CBPersistMgr : public CBBase {
public:
	char *m_SavedDescription;
	time_t m_SavedTimestamp;
	byte m_SavedVerMajor;
	byte m_SavedVerMinor;
	byte m_SavedVerBuild;
	byte m_SavedExtMajor;
	byte m_SavedExtMinor;
	HRESULT SaveFile(char *Filename);
	uint32 GetDWORD();
	void PutDWORD(uint32 Val);
	char *GetString();
	void PutString(const char *Val);
	void Cleanup();
	HRESULT InitLoad(char *Filename);
	HRESULT InitSave(char *Desc);
	HRESULT GetBytes(byte  *Buffer, uint32 Size);
	HRESULT PutBytes(byte  *Buffer, uint32 Size);
	uint32 m_Offset;
	uint32 m_BufferSize;
	byte *m_Buffer;
	bool m_Saving;

	uint32 m_RichBufferSize;
	byte *m_RichBuffer;

	HRESULT Transfer(const char *Name, void *Val);
	HRESULT Transfer(const char *Name, int *Val);
	HRESULT Transfer(const char *Name, uint32 *Val);
	HRESULT Transfer(const char *Name, float *Val);
	HRESULT Transfer(const char *Name, double *Val);
	HRESULT Transfer(const char *Name, bool *Val);
	HRESULT Transfer(const char *Name, byte *Val);
	HRESULT Transfer(const char *Name, RECT *Val);
	HRESULT Transfer(const char *Name, POINT *Val);
	HRESULT Transfer(const char *Name, char **Val);
	HRESULT Transfer(const char *Name, Vector2 *Val);
	HRESULT Transfer(const char *Name, AnsiStringArray &Val);
	CBPersistMgr(CBGame *inGame = NULL);
	virtual ~CBPersistMgr();
	bool CheckVersion(byte  VerMajor, byte VerMinor, byte VerBuild);

	uint32 m_ThumbnailDataSize;
	byte *m_ThumbnailData;

};

} // end of namespace WinterMute

#endif
