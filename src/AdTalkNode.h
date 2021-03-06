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

#ifndef __WmeAdTalkNode_H__
#define __WmeAdTalkNode_H__

#include "persistent.h"
#include "BBase.h"

namespace WinterMute {
class CAdSpriteSet;
class CBSprite;
class CAdTalkNode : public CBBase {
public:
	char *m_SpriteSetFilename;
	CAdSpriteSet *m_SpriteSet;
	CBSprite *GetSprite(TDirection Dir);
	bool IsInTimeInterval(uint32 Time, TDirection Dir);
	HRESULT LoadSprite();
	DECLARE_PERSISTENT(CAdTalkNode, CBBase)

	CAdTalkNode(CBGame *inGame);
	virtual ~CAdTalkNode();
	HRESULT LoadBuffer(byte  *Buffer, bool Complete = true);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent = 0);
	char *m_SpriteFilename;
	CBSprite *m_Sprite;
	uint32 m_StartTime;
	uint32 m_EndTime;
	bool m_PlayToEnd;
	bool m_PreCache;
	char *m_Comment;

};

} // end of namespace WinterMute

#endif
