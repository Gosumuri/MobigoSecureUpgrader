////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "MainFrm.h"

// debug
#include "DebugEditView.h"
#include "DebugListView.h"

// tree
#include "WorkspaceTreeView.h"
#include "FileTreeView.h"
#include "ConfigTreeView.h"

// etc
#include "utilprog.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// const/static
////////////////////////////////////////////////////////////////////////////////

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
    ID_INDICATOR_PROGRESS,  // Progress 
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	//-DEBUG_LOG(">> CMainFrame::CMainFrame()");
}

CMainFrame::~CMainFrame()
{
	//-DEBUG_LOG(">> CMainFrame::~CMainFrame()");
}

////////////////////////////////////////////////////////////////////////////////
// virtual
////////////////////////////////////////////////////////////////////////////////

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CFrameWnd::RecalcLayout(bNotify);
	CFrameWnd::RecalcLayout(bNotify);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

    // doc title이 합쳐져서 보이지 않도록 처리
    //  - "Mobigo Secure Application 0.0.1 [MobigoSecureApp1]" -> "Mobigo Secure Application 0.0.1"
	cs.style &= ~FWS_ADDTOTITLE;

    return CMDIFrameWnd::PreCreateWindow(cs);
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//-DEBUG_LOG(">> CMainFrame::OnCreate()");

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    // 0. hide toolbar
    m_wndToolBar.ShowWindow( SW_HIDE );

	// 1. xml file의 존재 유무를 check하여 존재하지 않으면, 프로그램 종료한다.
	{
		// 1.1. workspace xml
        CString strWorkspaceFilePath = MOBIGO_SECURE_UP_WORKSPACE_PATH;
		if (CUtilFile::IsExistPath(strWorkspaceFilePath) == FALSE) {
			AfxMessageBox("\"" + strWorkspaceFilePath + "\" is not existed.");
            QUIT_APP(-1);
			return 0;
		}
#ifdef FEATURE_LOG_FILE_EXPLORER
        //1.2. file xml
        CString strFileTreeFilePath = MOBIGO_SECURE_UP_FILETREE_PATH;
        if (CUtilFile::IsExistPath(strFileTreeFilePath) == FALSE) {
            AfxMessageBox("\"" + strFileTreeFilePath + "\" is not existed.");
            QUIT_APP(-1);
            return 0;
        }
#endif
#ifdef FEATURE_APP_CONFIG_EXPLORER
		// 1.3. app conf xml
        CString strConfFilePath = MOBIGO_SECURE_UP_CONF_PATH;
		if (CUtilFile::IsExistPath(strConfFilePath) == FALSE) {
			AfxMessageBox("\"" + strConfFilePath + "\" is not existed.");
            QUIT_APP(-1);
			return 0;
		}
#endif
	}
	

	// 3. DockBar
#ifdef FEATURE_DEBUG_DOCKING_BAR
	InitDebugBar();
#endif 
    // 4. TreeBar
	InitTreeBar();
    LoadTreeXML();

    // 5. 관리자 권한 확인
    CConfigTreeView *pConfigTreeView = (CConfigTreeView*)m_wndTreeBar.GetView(2);
    BOOL bCheckAdmin = pConfigTreeView->IsOn( "ID_ON_OFF_ADMINISTRATOR" );
	//-DEBUG_LOG("++   bCheckAdmin=%s", B2S( bCheckAdmin ) );
    if( bCheckAdmin == TRUE ) {
        if( CUtilAccount::IsCurrentUserLocalAdministrator() == FALSE ) {
		    AfxMessageBox( "관리자 권한으로 실행하세요.\r\n(Run as Administrator.)" );
            QUIT_APP(-1);
		    return 0;
        }
    }
    // 2. title 수정
    CString strUrl;
    CString strUrlJson =   _T("/api/apiSecureUpgradeListJson.php");
    BOOL bIsOnBetaServer = pConfigTreeView->IsOn( "ID_ON_BETA_SERVER" );
    if(bIsOnBetaServer){
        strUrl       = pConfigTreeView->GetString("ID_BETA_SERVER");
    }else{
        strUrl       = pConfigTreeView->GetString("ID_REAL_SERVER");
    }
    
    CString strTitle;
    strTitle.Format( "%s( %s ) - %s%s", MOBIGO_SECURE_UPGRADER_NAME, BUILD_TIME(), strUrl , strUrlJson ); //URL_UPGRADE_LIST_JSON =  strUrl + strUrlJson 
    this->SetWindowText( strTitle ); 

	// 6. progress init
	CUtilProg::Init(m_wndStatusBar.GetSafeHwnd());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// debug
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////
// init fucntion 
////////////////////////////////////////////////////////////////////////////////

// debug bar
void CMainFrame::InitDebugBar()
{
	//-DEBUG_LOG(">> CMainFrame::InitDebugBar()");

	// 1 Debug Bar
	m_wndDebugBar.Create(this, CSize(200, 200), IDW_PROP_DEBUG_BAR);
	// m_wndDebugBar.SetBarStyle( m_wndMainToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndDebugBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDebugBar, AFX_IDW_DOCKBAR_BOTTOM);

	// 2. Add message View
	m_wndDebugBar.AddView("Message", RUNTIME_CLASS(CDebugEditView));
	//-m_wndDebugBar.AddView("Error",   RUNTIME_CLASS(CDebugListView));
	//-m_wndDebugBar.AddView("Warning", RUNTIME_CLASS(CWarningEditView));
	//-m_wndDebugBar.AddView("Error",   RUNTIME_CLASS(CErrorEditView));
	//-m_wndDebugBar.AddView("Watch",   RUNTIME_CLASS(CWatchEditView));

	// 3. set message window
    SET_MSG_WND( m_wndDebugBar.GetView(0)->m_hWnd );
	//	CDebug::m_hWndWarningSelf = m_wndDebugBar.GetView( 1 )->m_hWnd;
	//	CDebug::m_hWndErrorSelf   = m_wndDebugBar.GetView( 2 )->m_hWnd;
	//	CDebug::m_hWndWatchSelf   = m_wndDebugBar.GetView( 3 )->m_hWnd;

    // debugBar는 거의 사용하지 않으므로, hide 시킨다. 
    m_wndDebugBar.ShowWindow(SW_HIDE);
}

// tree bar
void CMainFrame::InitTreeBar()
{
    //-DEBUG_LOG(">> CMainFrame::InitTreeBar()");

	// 1. Tree Bar
	m_wndTreeBar.Create(this, CSize(320, 100), IDW_PROP_TREE_BAR);
	//	m_wndTreeBar.SetBarStyle(m_wndMainToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTreeBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndTreeBar, AFX_IDW_DOCKBAR_LEFT);

	// 1.1. Add Tree View
	m_wndTreeBar.AddView("Workspace", RUNTIME_CLASS(CWorkspaceTreeView));
#ifdef FEATURE_LOG_FILE_EXPLORER
	m_wndTreeBar.AddView("File", RUNTIME_CLASS(CFileTreeView));
#endif
#ifdef FEATURE_APP_CONFIG_EXPLORER
	m_wndTreeBar.AddView("Config", RUNTIME_CLASS(CConfigTreeView));
#endif

	// 1.2. Set Active View
	m_wndTreeBar.SetActiveView(0);
}

void CMainFrame::LoadTreeXML()
{
	// 4. Load XML
    {
	    // 4.1. WorkSpace Load XML
	    CWorkspaceTreeView *pWorkspaceTreeView = (CWorkspaceTreeView*)m_wndTreeBar.GetView(0);
        CString strWorkspaceFilePath = MOBIGO_SECURE_UP_WORKSPACE_PATH;
	    if (pWorkspaceTreeView->Load(strWorkspaceFilePath) == FALSE) {
            QUIT_APP(-1);
            return;
	    }
	    pWorkspaceTreeView->ExpandChildItem(2);	// depth

#ifdef FEATURE_LOG_FILE_EXPLORER
        // 4.2. File tree Load XML
        CFileTreeView *pFileTreeView = (CFileTreeView*)m_wndTreeBar.GetView(1);
        CString strFileTreePath = MOBIGO_SECURE_UP_FILETREE_PATH;
        if (pFileTreeView->Load(strFileTreePath) == FALSE) {
            QUIT_APP(-1);
            return;
        }
        pFileTreeView->ExpandChildItem(0);	// depth
#endif
#ifdef FEATURE_APP_CONFIG_EXPLORER
        // 4.3. App Config tree Load XML
        CConfigTreeView *pConfigTreeView = (CConfigTreeView*)m_wndTreeBar.GetView(2);
        CString strConfFilePath = MOBIGO_SECURE_UP_CONF_PATH;
        if (pConfigTreeView->Load(strConfFilePath) == FALSE) {
            QUIT_APP(-1);
            return;
        }
        pConfigTreeView->ExpandChildItem(1);	// depth
#endif
    }
}

BOOL CMainFrame::IsOnConfig( CString strConfigID )
{
#ifdef FEATURE_APP_CONFIG_EXPLORER
    CConfigTreeView *pConfigTreeView = (CConfigTreeView*)m_wndTreeBar.GetView(2);

    BOOL bDebugOn = pConfigTreeView->IsOn( strConfigID );
    if( bDebugOn == TRUE ) {
        return TRUE;
    }
#endif

    return FALSE;
}

void CMainFrame::SaveConfig()
{
#ifdef FEATURE_APP_CONFIG_EXPLORER
    CConfigTreeView *pConfigTreeView = (CConfigTreeView*)m_wndTreeBar.GetView(2);
    CString strConfigXML = pConfigTreeView->Tree2XML();
    DEBUG_LOG_G( "++   " + strConfigXML );
    CString strConfFilePath = MOBIGO_SECURE_UP_CONF_PATH;
    CUtilFile::WriteString( strConfFilePath, strConfigXML ); 
#endif
}

CString CMainFrame::GetStringOnConfig( CString strConfigID )
{
#ifdef FEATURE_APP_CONFIG_EXPLORER
    CConfigTreeView *pConfigTreeView = (CConfigTreeView*)m_wndTreeBar.GetView(2);

    CString strString = pConfigTreeView->GetString( strConfigID );
    return strString;
#endif

    return "";
}

