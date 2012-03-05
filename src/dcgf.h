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

#ifndef _DCGF_H_
#define _DCGF_H_


//////////////////////////////////////////////////////////////////////////
#define DCGF_VER_MAJOR 1
#define DCGF_VER_MINOR 0
#define DCGF_VER_BUILD 1
#define DCGF_VER_SUFFIX "beta"
#define DCGF_VER_BETA true

#define DCGF_NAME "WME Lite"
#define DCGF_MAGIC 0xDEC0ADDE

// minimal saved game version we support
#define SAVEGAME_VER_MAJOR 1
#define SAVEGAME_VER_MINOR 0
#define SAVEGAME_VER_BUILD 0
//////////////////////////////////////////////////////////////////////////

#define COMPRESSED_FILE_MAGIC 0x504D435A // ZCMP

#ifdef GetClassName
#undef GetClassName
#endif

// macros
#define RELEASE(obj) if(obj) { obj->Release(); obj = NULL; } else 0
#define SAFE_DELETE(obj)  if(obj) { delete obj; obj = NULL; } else 0
#define SAFE_DELETE_ARRAY(obj)  if(obj) { delete [] obj; obj = NULL; } else 0
#define DegToRad(_val) (_val*PI*(1.0f/180.0f))
#define RadToDeg(_val) (_val*(180/PI))

#endif // _DCGF_H_
