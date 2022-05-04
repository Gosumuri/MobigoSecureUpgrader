////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxdialogex.h"
#include "HtmlMessageBoxDialog.h"
#include "buildtime.h"
#include "Util2.h"
#include "UtilSys.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define     MY_TIMER_DISPLAY_MESSAGE    1

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

#define DEFAULT_ZOOM            100

IMPLEMENT_DYNCREATE(CHtmlMessageBoxDialog, CDHtmlDialog)

////////////////////////////////////////////////////////////////////////////////
// dispatch map
////////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CHtmlMessageBoxDialog, CDHtmlDialog)
    // DISP_FUNCTION
    DISP_FUNCTION(CHtmlMessageBoxDialog, "OnJs",         OnJs,          VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION(CHtmlMessageBoxDialog, "OnUserAgent",  OnUserAgent,   VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION(CHtmlMessageBoxDialog, "OnResolution", OnResolution,  VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////////
// event map
////////////////////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CHtmlMessageBoxDialog, CDHtmlDialog)
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_TIMER()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CHtmlMessageBoxDialog::CHtmlMessageBoxDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHtmlMessageBoxDialog::IDD, NULL, pParent)
{
    m_strDialogTitle            = _T("");
    m_strModuleName             = _T("");
    m_strUserAgent              = _T("");
    m_strResolution             = _T("");

    m_strMessageBoxUrl          = _T("");
    m_strMessageBoxTitle        = _T("");
    m_strMessageBoxContent      = _T("");
    m_nResult                   = HTML_MESSAGE_BOX_RESULT_NONE; 
    m_nZoom                     = DEFAULT_ZOOM;

 // init zoom variable
    double dLegibleRatio = CUtilSys::GetLegibleRatioOfMonitor(); 
    int    nLegibleZoom  = (int)(dLegibleRatio*(double)m_nZoom);
    m_nZoom = nLegibleZoom;
}

CHtmlMessageBoxDialog::~CHtmlMessageBoxDialog()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual function 
////////////////////////////////////////////////////////////////////////////////

void CHtmlMessageBoxDialog::OnOK()
{
    // OnOK를 override하지 않으면, Dialog가 닫혀서 화면이 하얀게 보이는 현상이 발생한다 
    // 아래 url은 해결책을 제시해 주지는 않았지만, hint를 제시해 주었다. 
    // - http://microsoft.public.win32.programmer.ui.narkive.com/Erz2SNx3/trapping-enter-in-cdhtmldialog
}

void CHtmlMessageBoxDialog::OnCancel()
{
}

BOOL CHtmlMessageBoxDialog::OnInitDialog()
{
    CDHtmlDialog::OnInitDialog();
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

   EnableAutomation();
   SetExternalDispatch(GetIDispatch(TRUE));

#if 0 스크롤바 보이도록 설정
    m_dwHostFlags &= ~DOCHOSTUIFLAG_SCROLL_NO;
    m_dwHostFlags &= ~DOCHOSTUIFLAG_NO3DOUTERBORDER;
    m_dwHostFlags &= ~DOCHOSTUIFLAG_NO3DBORDER;
#endif
    // html load
    LoadHtml( m_strMessageBoxUrl );
    
    return TRUE;
}

void CHtmlMessageBoxDialog::OnBeforeNavigate( LPDISPATCH pDisp, LPCTSTR szUrl )
{
    CDHtmlDialog::OnBeforeNavigate( pDisp, szUrl );
}

void CHtmlMessageBoxDialog::OnNavigateComplete( LPDISPATCH pDisp, LPCTSTR szUrl )
{
    CDHtmlDialog::OnNavigateComplete( pDisp, szUrl );

    if( m_strMessageBoxUrl.CompareNoCase( szUrl ) == 0 ) {
        SetTimer( MY_TIMER_DISPLAY_MESSAGE, 100, NULL ); 
    }
}

////////////////////////////////////////////////////////////////////////////////
// event handler
///////////////////////////////////////////////////////////////////////////////

void CHtmlMessageBoxDialog::OnTimer( UINT_PTR nIDEvent )
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch( nIDEvent ) {
        case MY_TIMER_DISPLAY_MESSAGE : {
            KillTimer( MY_TIMER_DISPLAY_MESSAGE );
            Zoom();
            FillTitle();
            ToJs();
        }
        break;
    }

    CDHtmlDialog::OnTimer( nIDEvent );
}

////////////////////////////////////////////////////////////////////////////////
// On Javascript
// http://kooyk.blogspot.kr/2016/05/javascript-mfc.html
///////////////////////////////////////////////////////////////////////////////

void CHtmlMessageBoxDialog::OnJs(LPCTSTR szID )
{
    CString strID (szID);

    if(        strID.CompareNoCase( "OK" )  == 0 ) {
        m_nResult = HTML_MESSAGE_BOX_RESULT_OK; 
    } else if( strID.CompareNoCase( "YES" ) == 0 ) {
        m_nResult = HTML_MESSAGE_BOX_RESULT_YES; 
    } else if( strID.CompareNoCase( "NO" )  == 0 ) {
        m_nResult = HTML_MESSAGE_BOX_RESULT_NO; 
    } 
    CDHtmlDialog::OnOK();    
}

void CHtmlMessageBoxDialog::OnUserAgent(LPCTSTR szUserAgent )
{
    m_strUserAgent.SetString(szUserAgent);
}

void CHtmlMessageBoxDialog::OnResolution(LPCTSTR szResolution )
{
    m_strResolution.SetString(szResolution);
}

////////////////////////////////////////////////////////////////////////////////
// To Javascript 
// https://tangcruise.gitbooks.io/mfc-and-cdhtmldialog/content/chapter2/c++-calling-javascript.html
///////////////////////////////////////////////////////////////////////////////

void CHtmlMessageBoxDialog::ToJs()
{
    VARIANT  vempty = {0};
    CString  szFuncName;
    CComBSTR langstr, funcName;
    CComPtr<IHTMLWindow2> pHTML2Wind;

    szFuncName.Format("setMessage('%s', '%s' )", m_strMessageBoxTitle, m_strMessageBoxContent );
    langstr  = L"javascript";
    funcName = szFuncName.AllocSysString();
    m_spHtmlDoc->get_parentWindow(&pHTML2Wind);
    pHTML2Wind->execScript(funcName, langstr, &vempty);
}

////////////////////////////////////////////////////////////////////////////////
// utility
////////////////////////////////////////////////////////////////////////////////

void CHtmlMessageBoxDialog::LoadHtml( CString strUrl )
{
	DWORD vFlags = navNoHistory | navNoReadFromCache | navNoWriteToCache;

	Navigate(   strUrl,         // lpszURL
                vFlags,         // dwFlags             = 0;
                NULL,           // lpszTargetFrameName = NULL
                NULL,           // lpszHeaders         = NULL
                NULL,           // lpvPostData         = NULL
                0 );            // dwPostDataLen       = 0
}

void CHtmlMessageBoxDialog::Zoom()
{
	CComVariant vZoom = m_nZoom;
	m_pBrowserApp->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &vZoom, NULL);
}

void CHtmlMessageBoxDialog::FillTitle()
{
    CString strUserAgent = "";
    if( m_strUserAgent.IsEmpty() == FALSE ) {
        int nIndex = m_strUserAgent.Find( "(" );
        if( 0 < nIndex ) {
            strUserAgent = m_strUserAgent.Mid( 0, nIndex - 1);
        }
        strUserAgent.Trim();
    }

    m_strDialogTitle.Format("%s( %s )", m_strModuleName, BUILD_TIME());
	m_strDialogTitle = m_strDialogTitle + " - " + strUserAgent;
	m_strDialogTitle = m_strDialogTitle + " - " + m_strResolution + " - ( " + CUtil2::Int2Str(m_nZoom) + "% )";
	this->SetWindowText(m_strDialogTitle);
}