
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "mymacro.h"
#include "MobigoSecureUpgrader.h"
#include "MainFrm.h"
#include "UiController.h"


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// structure
////////////////////////////////////////////////////////////////////////////////

struct PostMessageDelayedInfo
{
	HWND   hwnd;
	UINT   msg;
	WPARAM wParam;
	LPARAM lParam;
	DWORD  dwDelay;
};
////////////////////////////////////////////////////////////////////////////////
// static function 
////////////////////////////////////////////////////////////////////////////////

static unsigned __stdcall PostMessageDelayedReal(void *vpParam )
{
	PostMessageDelayedInfo *pDelayInfo = (PostMessageDelayedInfo*)vpParam;
	Sleep(pDelayInfo->dwDelay);
	PostMessage(pDelayInfo->hwnd, pDelayInfo->msg, pDelayInfo->wParam, pDelayInfo->lParam);
	delete pDelayInfo;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

CWnd*       CMyMacro::m_pMainFrame          = NULL;

// for Kernel Thread
CWnd*       CMyMacro::m_pUiController       = NULL;
CWnd*       CMyMacro::m_pDataController     = NULL;
void*       CMyMacro::m_pDataMgr            = NULL;

// for MESSAGE_LOG
HWND        CMyMacro::m_hWndMsg             = NULL;
char*		CMyMacro::m_pSzMsg			    = (char*)NULL;
CString*	CMyMacro::m_pStrMsg			    = (CString*)NULL;
int			CMyMacro::m_nCount			    = 0; 

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CMyMacro::CMyMacro(void)
{
}

CMyMacro::~CMyMacro(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// life cycle 
////////////////////////////////////////////////////////////////////////////////

void	CMyMacro::Init()
{
	m_pSzMsg			= (char*)malloc(MAX_BUF + 1);
	memset( m_pSzMsg, 0, sizeof(char)*( MAX_BUF + 1) );
	m_pStrMsg			= (CString*)new CString();
}

void	CMyMacro::Exit()
{
	free( m_pSzMsg );
	delete m_pStrMsg;
}

////////////////////////////////////////////////////////////////////////////////
// post/send function
////////////////////////////////////////////////////////////////////////////////

void CMyMacro::PostUiThread( int nMsg, WPARAM wParam, LPARAM lParam )
{
	::PostMessage( m_pUiController->GetSafeHwnd(), nMsg, wParam, lParam );
}

void CMyMacro::SendUiThread( int nMsg, WPARAM wParam, LPARAM lParam )
{
	::SendMessage( m_pUiController->GetSafeHwnd(), nMsg, wParam, lParam );
}

void CMyMacro::PostUiThreadDelay( int nMsg, WPARAM wParam, LPARAM lParam, DWORD dwDelay )
{
	PostMessageDelayedInfo *pDelayInfo = new PostMessageDelayedInfo;
	pDelayInfo->hwnd      = m_pUiController->GetSafeHwnd();
	pDelayInfo->msg       = nMsg;
	pDelayInfo->wParam    = wParam;
	pDelayInfo->lParam    = lParam;
	pDelayInfo->dwDelay   = dwDelay;
	_beginthreadex(0,0, &PostMessageDelayedReal, (void*)pDelayInfo, 0,0);
}

void CMyMacro::QuitApp( int nExitCode )
{
    AfxGetMainWnd()->SendMessage( WM_COMMAND, ID_APP_EXIT );
}

void CMyMacro::MessageLog( CString strMsg )
{
	if( m_pSzMsg == NULL )
		return ;

	CString  strCount = CUtil::PowerChar( '0', CUtil2::Int2Str( ++m_nCount ), 7 );
	strMsg	          = "[" + CUtilTime::GetLocalTime() + ":" + strCount +"] " + strMsg + "\r\n";

	for( int i = 0; i*MAX_BUF < strMsg.GetLength() ; i++ ) {
		_tcscpy( (LPTSTR)m_pSzMsg, strMsg.Mid( i*MAX_BUF, MAX_BUF ) );

		if( m_hWndMsg != NULL /*&& ::IsWindow( m_hWndDebugSelf ) */) {
            ::SendMessage( m_hWndMsg, MY_MSG_DEBUG,	NULL, (LPARAM)m_pSzMsg ); 
        }
	}
}

////////////////////////////////////////////////////////////////////////////////
// config function
////////////////////////////////////////////////////////////////////////////////
BOOL CMyMacro::IsOnBetaServer()
{
    return ((CMainFrame*)m_pMainFrame)->IsOnConfig( "ID_ON_BETA_SERVER" );
}

BOOL CMyMacro::IsOnDebug()
{
    return ((CMainFrame*)m_pMainFrame)->IsOnConfig( "ID_ON_OFF_DEBUG" );
}

BOOL CMyMacro::IsOnZoomByWheel()
{
    return ((CMainFrame*)m_pMainFrame)->IsOnConfig( "ID_ON_OFF_ZOOM_WHEEL" );
}

CString CMyMacro::GetCurrentServerUrl()
{
   if( IsOnBetaServer() ){
       return GetBetaServerUrl();
   }else{
       return GetRealServerUrl();
   }
}

CString CMyMacro::GetRealServerUrl()
{
    return ((CMainFrame*)m_pMainFrame)->GetStringOnConfig( "ID_REAL_SERVER" );
}

CString CMyMacro::GetBetaServerUrl()
{
    return ((CMainFrame*)m_pMainFrame)->GetStringOnConfig( "ID_BETA_SERVER" );
}
