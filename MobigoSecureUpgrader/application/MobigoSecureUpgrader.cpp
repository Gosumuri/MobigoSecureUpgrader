
// MobigoSecureUpgrader.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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

// ������ CMobigoSecureUpgraderApp ��ü�Դϴ�.
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
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MobigoSecureUpgrader.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
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

#ifdef FEATURE_PREVENT_DUAL_RUN_DURING_SHORTTIME // ª�� �ð��� ���ӵ� App�� �ߺ� ������ ���� �Ѵ�
    BOOL bDualRunDuringShortTime = CUtilApp::IsDualRunDuringShortTime( MOBIGO_SECURE_UPGRADER_EXE_PATH );
    if( bDualRunDuringShortTime == TRUE ) {
        return FALSE;
    }
#endif

#ifdef FEATURE_RUN_BACKGROUND
    // Mobigo Secure Application �ߺ� ���� ���� 
    BOOL bActive = CUtilSys::PreventDuplicateExecution(MOBIGO_SECURE_APP_NAME, MOBIGO_SECURE_APP_EXE_PATH, TRUE ); 
    if( bActive == FALSE ) {
        if( CUtilFile::IsExistPath( MOBIGO_SECURE_APP_EXE_PATH ) == TRUE ) {
            CUtilSys::Run( MOBIGO_SECURE_APP_EXE_PATH, "", MOBIGO_ROOT_PATH, FALSE );
        }
    }
#endif    

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
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

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MobigoSecureUpgTYPE,
		RUNTIME_CLASS(CUiDoc),
		RUNTIME_CLASS(CUiTddController), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CUiDebugView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
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

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
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
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

    DELETE_MY_OBJECT( m_pDataMgr );

#ifdef FEATURE_MOBIGO_DEBUGGER
    #if 0 // 2018017_jkim - DEBUG_EXIT()�� ȣ���ϴ� ���� ������, DEBUG_EXIT() ȣ�� ��, � Thread�� �������� �ʰ� �����ϸ� �״� �̽� �߻��Ͽ� disable ó��  
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

