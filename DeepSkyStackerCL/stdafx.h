// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//
// Want to support Windows XP and up
//
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include <algorithm>
using std::min;
using std::max;

#include <windows.h>

#include <vector>
#include <math.h>
#include <atlbase.h>
#include <atlstr.h>

#include <DSSCommon.h>
#include <resource.h>
#include <ztrace.h>

//#include <StdString.h>
//#define CString			CStdString

#define TRACE0(x)
#define TRACE1(x, y)

// TODO: reference additional headers your program requires here

#include "BitmapExt.h"


#include <QFileInfo>
#include <QDir>

#include <gdiplus.h>
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")