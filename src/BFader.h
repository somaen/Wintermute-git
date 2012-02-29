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

#ifndef __WmeBFader_H__
#define __WmeBFader_H__


#include "BObject.h"

namespace WinterMute {

class CBFader : public CBObject {
public:
	bool m_System;
	uint32 GetCurrentColor();
	HRESULT FadeOut(uint32 TargetColor, uint32 Duration, bool System = false);
	HRESULT FadeIn(uint32 SourceColor, uint32 Duration, bool System = false);
	HRESULT Deactivate();
	HRESULT Display();
	HRESULT Update();
	DECLARE_PERSISTENT(CBFader, CBObject)
	CBFader(CBGame *inGame);
	virtual ~CBFader();
	bool m_Active;
	byte m_Red;
	byte m_Green;
	byte m_Blue;
	byte m_CurrentAlpha;
	byte m_TargetAlpha;
	byte m_SourceAlpha;
	uint32 m_Duration;
	uint32 m_StartTime;
};

} // end of namespace WinterMute

#endif
