
// MobigoSecureUpgrader.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MobigoSecureUpgrader.h"
#include "MainFrm.h"

#include "UiTddController.h"
#include "UiDoc.h"
#include "UiDebugView.h"
#include "AboutDlg.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// application instance
////////////////////////////////////////////////////////////////////////////////

// 유일한 CMobigoSecureUpgraderApp 개체입니다.
CMobigoSecureUpgraderApp g_theApp;

////////////////////////////////////////////////////////////////////////////////
// message map
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMobigoSecureUpgraderApp, CWinApp)
    ON_COMMAND(ID_LOG_ON,           &CMobigoSecureUpgraderApp::OnLogOn)
    ON_COMMAND(ID_LOG_OFF,          &CMobigoSecureUpgraderApp::OnLogOff)
    ON_COMMAND(ID_LOG_CLEAR,        &CMobigoSecureUpgraderApp::OnLogClear)
    ON_COMMAND(ID_LOG_FILE,         &CMobigoSecureUpgraderApp::OnLogFile)
    ON_COMMAND(ID_SAVE_CONFIG,      &CMobigoSecureUpgraderApp::OnSaveConfig)
    ON_COMMAND(ID_APP_ABOUT,        &CMobigoSecureUpgraderApp::OnAppAbout)
	ON_COMMAND(ID_APP_EXIT,         &CMobigoSecureUpgraderApp::OnExit)	

    ON_COMMAND(ID_SERVICE_INSTALL,      &CMobigoSecureUpgraderApp::OnServiceInstall)
    ON_COMMAND(ID_SERVICE_START,        &CMobigoSecureUpgraderApp::OnServiceStart)
    ON_COMMAND(ID_SERVICE_STOP,         &CMobigoSecureUpgraderApp::OnServiceStop)
    ON_COMMAND(ID_SERVICE_UNINSTALL,    &CMobigoSecureUpgraderApp::OnServiceUninstall)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CMobigoSecureUpgraderApp::CMobigoSecureUpgraderApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MobigoSecureUpgrader.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
    m_pDataMgr      = NULL;
}


////////////////////////////////////////////////////////////////////////////////
// life cycle
////////////////////////////////////////////////////////////////////////////////

BOOL CMobigoSecureUpgraderApp::InitInstance()
{
    CMyEnv::Init();
    CMyMsg::Init();
    CMyMacro::Init();
    // mobigo-secure-app data manager  
    m_pDataMgr = new CMobigoSecureUpgraderDataMgr();
    m_pDataMgr->Init();

#ifdef FEATURE_PREVENT_DUAL_RUN_DURING_SHORTTIME // 짧은 시간에 연속된 App의 중복 실행을 방지 한다
    BOOL bDualRunDuringShortTime = CUtilApp::IsDualRunDuringShortTime( MOBIGO_SECURE_UPGRADER_EXE_PATH );
    if( bDualRunDuringShortTime == TRUE ) {
        return FALSE;
    }
#endif

#ifdef FEATURE_RUN_BACKGROUND
    // Mobigo Secure Application 중복 실행 방지 
    BOOL bActive = CUtilSys::PreventDuplicateExecution(MOBIGO_SECURE_APP_NAME, MOBIGO_SECURE_APP_EXE_PATH, TRUE ); 
    if( bActive == FALSE ) {
        if( CUtilFile::IsExistPath( MOBIGO_SECURE_APP_EXE_PATH ) == TRUE ) {
            CUtilSys::Run( MOBIGO_SECURE_APP_EXE_PATH, "", MOBIGO_ROOT_PATH, FALSE );
        }
    }
#endif    

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);
    
    // GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MobigoSecureUpgTYPE,
		RUNTIME_CLASS(CUiDoc),
		RUNTIME_CLASS(CUiTddController), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CUiDebugView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
    CMyMacro::SetMainFrame( pMainFrame );
#ifdef FEATURE_MOBIGO_DEBUGGER
	DEBUG_SET_FILE_LOG(TRUE);
    DEBUG_SET_APP_PATH( MOBIGO_SECURE_UPGRADER_DATA_PATH );
    DEBUG_SET_TITLE( MOBIGO_SECURE_UPGRADER_DEBUG_NAME );
    if( IS_ON_DEBUG() == TRUE ) {
	    DEBUG_SET_CONSOLE_LOG(TRUE);
        DEBUG_INIT();
    } else {
	    DEBUG_SET_CONSOLE_LOG(FALSE);
        DEBUG_INIT();
    }
#endif
    
    CString strDisk = CMyEnv::GetRootHardDisk();
    long lTotalSpace = CUtilSys::GetDiskTotalSpace( strDisk );
    long lUsedSpace  = CUtilSys::GetDiskUsedSpace(  strDisk );
    long lFreeSpace  = CUtilSys::GetDiskFreeSpace(  strDisk );
    
    int nTotalPercent = (int)(((double)lTotalSpace / (double)lTotalSpace)*100);
    int nUsedPercent  = (int)(((double)lUsedSpace  / (double)lTotalSpace)*100);
    int nFreePercent  = (int)(((double)lFreeSpace  / (double)lTotalSpace)*100);

    DEBUG_LOG_R( "/**************************************************************************************************/"          );
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*    Mobigo Secure Upgrader                                                                        "          ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - MOBIGO_SECURE_UPGRADER_APP_NAME     = %s", MOBIGO_SECURE_UPGRADER_NAME                               ); 
    DEBUG_LOG_R( "/*      - BUILD_TIME                          = %s", BUILD_TIME()                                              ); 
    DEBUG_LOG_R( "/*                                                                                                  "          );     
    DEBUG_LOG_R( "/*      - ROOT_PATH                           = %s", MOBIGO_ROOT_PATH                                          ); 
    DEBUG_LOG_R( "/*      - UPGRADE_DOWNLOD_PATH                = %s", MOBIGO_UPGRADE_DOWNLOD_PATH                               ); 
    DEBUG_LOG_R( "/*      - MOBIGO_UPGRADE_LIST_PATH            = %s", MOBIGO_UPGRADE_LIST_PATH                                  ); 
    DEBUG_LOG_R( "/*      - MOBIGO_TEMP_PATH                    = %s", MOBIGO_TEMP_PATH                                          ); 
    DEBUG_LOG_R( "/*                                                                                                  "          );     
    DEBUG_LOG_R( "/*      - APP_CONF_PATH                       = %s", MOBIGO_SECURE_UP_CONF_PATH                                ); 
    DEBUG_LOG_R( "/*      - FILETREE_PATH                       = %s", MOBIGO_SECURE_UP_FILETREE_PATH                            );
    DEBUG_LOG_R( "/*      - WORKSPACE_PATH                      = %s", MOBIGO_SECURE_UP_WORKSPACE_PATH                           );
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - LOG_PATH                            = %s", DEBUG_GET_LOG_PATH()                                      ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - URL_UPGRADE_LIST_JSON               = %s", URL_UPGRADE_LIST_JSON                                     ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - %s Total Space                      = %3d G byte (%d%%)", strDisk, (lTotalSpace), nTotalPercent      ); 
    DEBUG_LOG_R( "/*      - %s Used  Space                      = %3d G byte (%d%%)", strDisk, (lUsedSpace ), nUsedPercent       ); 
    DEBUG_LOG_R( "/*      - %s Free  Space                      = %3d G byte (%d%%)", strDisk, (lFreeSpace ), nFreePercent       ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 

    DEBUG_LOG_R( "/*      - Computer Name                       = %s",     CUtilSys::GetComputerName()                           ); 
    DEBUG_LOG_R( "/*      - Account ID                          = %s",     CUtilAccount::GetAccountID()                          ); 
    CMyStringArray saGroupName = CUtilAccount::GetAccountGroup( CUtilAccount::GetAccountID() );
    for( int i = 0; i < saGroupName.GetCount(); i++ ) {
        CString strGroupName = saGroupName.GetAt( i );
        DEBUG_LOG_R( "/*      - Group Name(%d)                       = %s", i, strGroupName                                      );                
    }

    DEBUG_LOG_R( "/*      - Is Current User Local Administrator = %s",CUtilAccount::IsCurrentUserLocalAdministrator()? "TRUE":"FALSE");
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - OS Version                          = %s",     CUtilOS::GetOSVersion()                               ); 
    DEBUG_LOG_R( "/*      - OS Name                             = %s",     CUtilOS::GetOSName()                                  ); 
    DEBUG_LOG_R( "/*      - OS Bit                              = %s",     (CUtilOS::Is64BitOS() ? "64 bit" : "32 bit" )         ); 
    DEBUG_LOG_R( "/*      - Tablet                              = %s",     (CUtilOS::IsTablet()  ? "Tablet" : "No Tablet" )      ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - IE Version                          = %s",     CUtilSys::GetIEVersion()                              ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/*      - IE Version                          = %s", CUtilSys::GetIEVersion()                                  ); 
    DEBUG_LOG_R( "/*                                                                                                  "          ); 
    DEBUG_LOG_R( "/**************************************************************************************************/"          );
    DEBUG_LOG_R( "                                                                                                    "          );
	m_pMainWnd = pMainFrame;

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
#ifdef FEATURE_RELEASE_VIEW
    pMainFrame->ShowWindow(SW_HIDE);
    #ifndef _DEBUG
        m_pMainWnd = CUiTddController::GetReleaseView();
    #endif
#else 
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
#endif
	pMainFrame->UpdateWindow();   

    // mobigo tray init
    MOBIGO_TRAY_INIT( m_mobigoTray, pMainFrame, MY_MSG_UPGRADE_TRAY_NOTIFY, IDR_UPGRADE_TRAY_MENU );

	return TRUE;
}

int CMobigoSecureUpgraderApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

    DELETE_MY_OBJECT( m_pDataMgr );

#ifdef FEATURE_MOBIGO_DEBUGGER
    #if 0 // 2018017_jkim - DEBUG_EXIT()를 호출하는 것이 맞지만, DEBUG_EXIT() 호출 후, 어떤 Thread가 종료하지 않고 동작하면 죽는 이슈 발생하여 disable 처리  
        DEBUG_EXIT();
    #endif 
#endif

    CMyMsg::Exit();
    CMyEnv::Exit();
    CMyMacro::Exit();

	return CWinApp::ExitInstance();
}

////////////////////////////////////////////////////////////////////////////////
// message handler
////////////////////////////////////////////////////////////////////////////////

void CMobigoSecureUpgraderApp::OnLogOn()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnLogOn()");
    DEBUG_TOGGLE();
}

void CMobigoSecureUpgraderApp::OnLogOff()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnLogOff()");
    DEBUG_TOGGLE();
}

void CMobigoSecureUpgraderApp::OnLogClear()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnLogClear()");
    DEBUG_CLEAR();
}

void CMobigoSecureUpgraderApp::OnLogFile()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnLogFile()");

	DEBUG_LOG("++ strAppDataPath=%s", DEBUG_GET_LOG_PATH() );

    ::ShellExecute(NULL, "open", "explorer", DEBUG_GET_LOG_PATH(), NULL, SW_SHOW);
}


void CMobigoSecureUpgraderApp::OnSaveConfig()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnSaveConfig()");
    
    ((CMainFrame*)AfxGetMainWnd())->SaveConfig();
}

void CMobigoSecureUpgraderApp::OnAppAbout()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnAppAbout()");

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMobigoSecureUpgraderApp::OnExit()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnExit()");

    m_mobigoTray.RemoveIcon();

    exit(-1);
}

void CMobigoSecureUpgraderApp::OnServiceInstall()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnServiceInstall()");
    int nResult = CUtilService::Install(  MOBIGO_SECURE_SERVICE_NAME,
                                          MOBIGO_SECURE_SERVICE_NAME,
                                          MOBIGO_SECURE_SERVICE_EXE_PATH, 
                                          "" );
    if( nResult == ERROR_NONE ) {
        LIGHT_LOG("++   INSTALL(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    } else {
        ERROR_LOG("++   [ERROR] fail to INSTALL(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    }
}

void CMobigoSecureUpgraderApp::OnServiceStart()
{
    OnServiceInstall();

	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnServiceStart()");
    DEBUG_LOG("++   ID_SERVICE_START" );
    int nResult = CUtilService::Start(MOBIGO_SECURE_SERVICE_NAME);
    if( nResult == ERROR_NONE ) {
        LIGHT_LOG("++   START(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    } else {
        ERROR_LOG("++   [ERROR] fail to START(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    }
}

void CMobigoSecureUpgraderApp::OnServiceStop()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnServiceStop()");
    int nResult = CUtilService::Stop(MOBIGO_SECURE_SERVICE_NAME);
    if( nResult == ERROR_NONE ) {
        LIGHT_LOG("++   STOP(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    } else {
        ERROR_LOG("++   [ERROR] fail to STOP(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    }

    OnServiceUninstall();
}

void CMobigoSecureUpgraderApp::OnServiceUninstall()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::OnServiceUninstall()");
    int nResult = CUtilService::Uninstall(MOBIGO_SECURE_SERVICE_NAME);
    if( nResult == ERROR_NONE ) {
        LIGHT_LOG("++   Uninstall(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    } else {
        ERROR_LOG("++   [ERROR] fail to Uninstall(%s)", MOBIGO_SECURE_SERVICE_NAME  );
    }
}

////////////////////////////////////////////////////////////////////////////////
// get active Doc/View
////////////////////////////////////////////////////////////////////////////////

CDocument* CMobigoSecureUpgraderApp::GetActiveDoc()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::GetActiveDoc()");

	POSITION		pos, posDoc, posView;
	CDocTemplate	*pDocTemplate = NULL;
	CDocument		*pDoc = NULL;
	CView			*pView = NULL;

	pos = this->GetFirstDocTemplatePosition();
	for (; pos != NULL;) {
		pDocTemplate = this->GetNextDocTemplate(pos);
		posDoc = pDocTemplate->GetFirstDocPosition();
		for (; posDoc != NULL;) {
			pDoc = (CDocument*)pDocTemplate->GetNextDoc(posDoc);

			return pDoc;
		}
	}

	return NULL;
}

CView* CMobigoSecureUpgraderApp::GetActiveView()
{
	DEBUG_LOG(">> CMobigoSecureUpgraderApp::GetActiveView()");

    POSITION		pos, posDoc, posView;
	CDocTemplate	*pDocTemplate = NULL;
	CDocument		*pDoc = NULL;
	CView			*pView = NULL;

	pos = this->GetFirstDocTemplatePosition();
	for (; pos != NULL;) {
		pDocTemplate = this->GetNextDocTemplate(pos);
		posDoc = pDocTemplate->GetFirstDocPosition();
		for (; posDoc != NULL;) {
			pDoc = (CDocument*)pDocTemplate->GetNextDoc(posDoc);

			pDoc->GetFirstViewPosition();
			posView = pDoc->GetFirstViewPosition();
			pView = pDoc->GetNextView(posView);

			return pView;
		}
	}

	return NULL;
}

