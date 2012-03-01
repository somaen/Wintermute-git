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

#ifndef __WmeWintypes_H__
#define __WmeWintypes_H__

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "common/scummsys.h"

//namespace WinterMute {

#ifndef __WIN32__

#define WINAPI
#define CALLBACK

#ifndef __OBJC__
typedef int BOOL;
#endif

#ifndef TRUE
#   define TRUE 1
#   define FALSE 0
#endif


#define CONST const

#define MAX_PATH 512

typedef char CHAR;
typedef short SHORT;
typedef int32_t LONG;

typedef uint16 WORD;
typedef uint64_t QWORD;
typedef int      INT;
typedef unsigned int UINT;

typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef PSTR *PZPSTR;
typedef const PSTR *PCZPSTR;
typedef const CHAR *LPCSTR, *PCSTR;
typedef PCSTR *PZPCSTR;

typedef struct tagRECT {
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *LPRECT;


typedef struct tagPOINT {
	LONG  x;
	LONG  y;
} POINT, *LPPOINT;


typedef uint32 HINSTANCE;
typedef uint32 HMODULE;
typedef uint32 HWND;

//typedef uint32 HRESULT;
typedef long HRESULT;

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define S_OK      ((HRESULT)0)
//#define S_FALSE   ((HRESULT)1)
#define E_FAIL   ((HRESULT)-1)


#endif // !__WIN32__

//} // end of namespace WinterMute

#endif // __WmeWintypes_H__
