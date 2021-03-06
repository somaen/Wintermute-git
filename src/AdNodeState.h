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

#ifndef __WmeAdNodeState_H__
#define __WmeAdNodeState_H__

namespace WinterMute {

class CAdEntity;

class CAdNodeState : public CBBase {
public:
	HRESULT TransferEntity(CAdEntity *Entity, bool IncludingSprites, bool Saving);
	void SetName(char *Name);
	void SetFilename(char *Filename);
	void SetCursor(char *Filename);
	DECLARE_PERSISTENT(CAdNodeState, CBBase)
	CAdNodeState(CBGame *inGame);
	virtual ~CAdNodeState();
	char *m_Name;
	bool m_Active;
	char *m_Caption[7];
	void SetCaption(char *Caption, int Case);
	char *GetCaption(int Case);
	uint32 m_AlphaColor;
	char *m_Filename;
	char *m_Cursor;

};

} // end of namespace WinterMute

#endif
