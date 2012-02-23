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
#include <algorithm>
#include <fstream>
#include "PathUtil.h"
#include "StringUtil.h"


#ifdef __WIN32__
#   include <shlobj.h>
#endif

#ifdef __MACOSX__
#   include <CoreServices/CoreServices.h>
#endif

#ifdef __IPHONEOS__
#   include "ios_utils.h"
#endif

namespace WinterMute {

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::UnifySeparators(const AnsiString &path) {
	AnsiString newPath = path;

	std::replace(newPath.begin(), newPath.end(), L'\\', L'/');
	return newPath;
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::NormalizeFileName(const AnsiString &path) {
	AnsiString newPath = UnifySeparators(path);
	StringUtil::ToLowerCase(newPath);
	return newPath;
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::Combine(const AnsiString &path1, const AnsiString &path2) {
	AnsiString newPath1 = UnifySeparators(path1);
	AnsiString newPath2 = UnifySeparators(path2);

	if (!StringUtil::EndsWith(newPath1, "/", true) && !StringUtil::StartsWith(newPath2, "/", true))
		newPath1 += "/";

	return newPath1 + newPath2;
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetDirectoryName(const AnsiString &path) {
	AnsiString newPath = UnifySeparators(path);

	size_t pos = newPath.find_last_of(L'/');

	if (pos == AnsiString::npos) return "";
	else return newPath.substr(0, pos + 1);
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetFileName(const AnsiString &path) {
	AnsiString newPath = UnifySeparators(path);

	size_t pos = newPath.find_last_of(L'/');

	if (pos == AnsiString::npos) return path;
	else return newPath.substr(pos + 1);
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetFileNameWithoutExtension(const AnsiString &path) {
	AnsiString fileName = GetFileName(path);

	size_t pos = fileName.find_last_of('.');

	if (pos == AnsiString::npos) return fileName;
	else return fileName.substr(0, pos);
}

//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetExtension(const AnsiString &path) {
	AnsiString fileName = GetFileName(path);

	size_t pos = fileName.find_last_of('.');

	if (pos == AnsiString::npos) return "";
	else return fileName.substr(pos);
}


//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetSafeLogFileName() {
	AnsiString logFileName = GetUserDirectory();

#ifdef __WIN32__
	char moduleName[MAX_PATH];
	::GetModuleFileName(NULL, moduleName, MAX_PATH);

	AnsiString fileName = GetFileNameWithoutExtension(moduleName) + ".log";
	fileName = Combine("/Wintermute Engine/Logs/", fileName);
	logFileName = Combine(logFileName, fileName);

#else
	// !PORTME
	logFileName = Combine(logFileName, "/Wintermute Engine/wme.log");
#endif

	CreateDirectory(GetDirectoryName(logFileName));
	return logFileName;
}

//////////////////////////////////////////////////////////////////////////
bool PathUtil::CreateDirectory(const AnsiString &path) {
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool PathUtil::MatchesMask(const AnsiString &fileName, const AnsiString &mask) {
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool PathUtil::FileExists(const AnsiString &fileName) {
	std::ifstream stream;

	stream.open(fileName.c_str());
	bool ret = stream.is_open();
	stream.close();

	return ret;
}


//////////////////////////////////////////////////////////////////////////
AnsiString PathUtil::GetUserDirectory() {
	AnsiString userDir = "./";

#ifdef __WIN32__
	char buffer[MAX_PATH];
	buffer[0] = '\0';
	LPITEMIDLIST pidl = NULL;
	LPMALLOC pMalloc;
	if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		if (pidl) {
			SHGetPathFromIDList(pidl, buffer);
		}
		pMalloc->Free(pidl);
		userDir = AnsiString(buffer);
	}
#elif __MACOSX__
	FSRef fileRef;
	OSStatus error = FSFindFolder(kUserDomain, kApplicationSupportFolderType, true, &fileRef);
	if (error == noErr) {
		char buffer[MAX_PATH];
		error = FSRefMakePath(&fileRef, (UInt8 *)buffer, sizeof(buffer));
		if (error == noErr)
			userDir = buffer;

	}
#elif __IPHONEOS__
	char path[MAX_PATH];
	IOS_GetDataDir(path);
	userDir = AnsiString(path);
#endif

	return userDir;
}

} // end of namespace WinterMute
