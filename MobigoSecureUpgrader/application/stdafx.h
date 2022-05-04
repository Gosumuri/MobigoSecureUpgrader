
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����

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








