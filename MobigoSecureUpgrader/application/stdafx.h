
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#include <sys/stat.h>
#include <afxcview.h>
#include <afxdhtml.h>

// GDI+
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// feature
#include "MyFeatures.h"

// common 
#include "BuildTime.h"
#include "util.h"
#include "util2.h"
#include "utilget.h"
#include "utilfile.h"
#include "utilprog.h"
#include "utilsys.h"
#include "utilos.h"
#include "utilregkey.h"
#include "UtilCrypt.h"
#include "UtilHttpPost.h"
#include "UtilHttpGet.h"
#include "UtilMessageBox.h"
#include "UtilJson.h"
#include "UtilZip.h"
#include "UtilService.h"
#include "UtilTime.h"
#include "UtilApp.h"
#include "UtilAccount.h"
#include "UtilEnv.h"
#include "MyPtrArray.h"
#include "MyStringArray.h"
#include "UpgradeData.h"
#include "UpgradeItem.h"
#include "DataMgr.h"

// mobigo debug
#include "debugex.h"

// mobogi secure upgrader
#include "MyMacro.h"
#include "MyEnv.h"
#include "MyMsg.h"








