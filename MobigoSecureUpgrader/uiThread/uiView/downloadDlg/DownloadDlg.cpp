////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "DownloadDlg.h"
#include "BindStatusCallback.h"
#include "UiDebugView.h"
#include "UiReleaseView.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// static
////////////////////////////////////////////////////////////////////////////////

CString CDownloadDlg::m_strCurFileName = "";

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	ON_WM_TIMER()
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDownloadDlg, CDialog)
CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
    m_pParentView        =  NULL;

	m_nDownloadPathCount =  0;
	m_nCurDownloadSize	 =  0;
	m_bDownloadSuccess	 =  TRUE;

    m_ulPreProgress      =  0;   
}

CDownloadDlg::~CDownloadDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
////////////////////////////////////////////////////////////////////////////////

void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STT_DOWNLOAD_FILENAME,	m_sttDownloadFileName	);
	DDX_Control(pDX, IDC_PG_DOWNLOAD,			m_pgDownload			);
	DDX_Control(pDX, IDC_STT_TH_TOTAL,			m_sttThTotal			);
	DDX_Control(pDX, IDC_PG_TOTAL,				m_pgTotal				);
	DDX_Control(pDX, IDC_STT_STATUS_MESSAGE,	m_sttStatusMessage		);
	DDX_Control(pDX, IDC_ED_DOWNLOAD_LOG,		m_edDownloadLog			);
}

BOOL CDownloadDlg::OnInitDialog()
{
	DEBUG_LOG(">> CDownloadDlg::OnInitDialog()");

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pgDownload.SetRange(0,100);
	m_pgTotal.SetRange(0,100);

	m_pgDownload.SetPos ( 0 );
    m_pgTotal.SetPos ( 0 );

	SetTimer( MY_TIMER_DOWNLOAD_START, 500, NULL );
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CDownloadDlg::OnTimer(UINT nIDEvent)
{
	DEBUG_LOG(">> CDownloadDlg::OnTimer()");

	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent ) {
		case MY_TIMER_DOWNLOAD_START : {
	        DEBUG_LOG("++   MY_TIMER_DOWNLOAD_START");
			KillTimer( MY_TIMER_DOWNLOAD_START );
	
			CWinThread* pWorkerThread = NULL;
			pWorkerThread = AfxBeginThread ( ThreadDownload, 
											 this,
											 THREAD_PRIORITY_NORMAL, 
											 0, 
											 CREATE_SUSPENDED );
			if ( NULL != pWorkerThread ) {
				CCallback::m_fAbortDownload = 0;
				pWorkerThread->m_bAutoDelete = TRUE;
				// Kick off the download!
				pWorkerThread->ResumeThread();
			}
		}
		break;
		case MY_TIMER_DOWNLOAD_END : {
	        DEBUG_LOG("++   MY_TIMER_DOWNLOAD_END");
			KillTimer( MY_TIMER_DOWNLOAD_END );
			OnOK();
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDownloadDlg::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	DEBUG_LOG(">> CDownloadDlg::OnWindowPosChanging()");

    CDialog::OnWindowPosChanging( lpwndpos );

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

#ifndef _DEBUG 
    // Dialog를 hide 시킨다. 
    if( lpwndpos != NULL ) {
        lpwndpos->flags &= ~SWP_SHOWWINDOW;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
// thread
////////////////////////////////////////////////////////////////////////////////

UINT CDownloadDlg::ThreadDownload( void* pv )
{
	DEBUG_LOG( ">> CDownloadDlg::ThreadDownload()" );

	CDownloadDlg *pDlg = (CDownloadDlg*)pv;

	HRESULT		hr = NULL;
	CCallback	callback;
    callback.m_pDlg = pDlg;

	DEBUG_LOG( "++   Count : " + CUtil2::Int2Str( pDlg->m_nDownloadPathCount ) );

	for( int i = 0; i < pDlg->m_nDownloadPathCount; i++ ) {

		// 0. reset progress
		pDlg->m_pgDownload.SetPos ( 0 );
        pDlg->m_ulPreProgress = 0;

		// 1. path 
		CString strSrcPath  = pDlg->m_ayStrSrcPath.GetAt( i );
		CString strDescPath = pDlg->m_ayStrDescPath.GetAt( i );
		int     nSize       = CUtil2::Str2Int( pDlg->m_ayStrSize.GetAt( i ) );
		strSrcPath.Replace( '\\', '/' );
		strDescPath.Replace( '/', '\\' );

        CUtilFile::MakeDir( strDescPath );

        DEBUG_LOG( "++   [%d] ", (i) );
		DEBUG_LOG( "++     strSrcPath  = "   + strSrcPath  );
		DEBUG_LOG( "++     strDescPath = "   + strDescPath );
		DEBUG_LOG( "++     nSize       = %d",  nSize       );
        
        callback.m_nDownloadSize = nSize;

		// 2. display file name
		m_strCurFileName = CUtilFile::ExtractFileName( strDescPath );
		DEBUG_LOG( "++     m_strCurFileName = " + m_strCurFileName );
		pDlg->m_sttDownloadFileName.SetWindowText( m_strCurFileName );	

		// 3. start download
		//	3.1. activeX가 URLDownloadToFile()의 첫번째 호출에서는 Error = 0x800C0004가 발생한다.
		//	3.2. 0x800C0004의 의미  
		//		- Server access failure.* 
		//		- The attempt to connect with the server failed.
		//		- Please verify that the server is running and available
		//  3.3. 따라서 download시도를 여러번 시도하도록 처리 했다.
		hr = ::URLDownloadToFile (	NULL,				// ptr to ActiveX container
									strSrcPath,			// URL to get
									strDescPath,		// file to store data in
									0,					// reserved
									&callback );		// ptr to IBindStatusCallback
		// 4. no cache
		// - URLDownloadToFile()를 사용하여 파일을 다운로드하면 캐시(cache)에 남게된다.
		// - 다시 동일 URL으로 파일을 다운로드하면 캐시에 있는 파일이 다운로드된다.
		// - 새로운 파일을 다운로드하려면 DeleteUrlCacheEntry()를 추가한다.
		if ( SUCCEEDED(hr) ) {
			::DeleteUrlCacheEntry( strSrcPath ); 
		}
		
        // 5. download complete
		if ( SUCCEEDED(hr) ) {

			// 5.0. log
			static char szLog[100];
			sprintf( szLog, "[%d] %s Done...!!!", i, (char*)(LPCTSTR)(m_strCurFileName)  );
			::SendMessage( pDlg->m_edDownloadLog.m_hWnd, MY_MSG_DEBUG, 0, (LPARAM)szLog );
			DEBUG_LOG( "++     %s Done...!!!", (char*)(LPCTSTR)(m_strCurFileName)  );

			// 5.1. total download rate
			int nTotalPercentDone = int(100*i/pDlg->m_nDownloadPathCount );
			pDlg->m_pgTotal.SetPos ( nTotalPercentDone );

			// 5.2. ( th / total )
			CString strThTotal;
			strThTotal.Format( "( %d / %d )", i, pDlg->m_nDownloadPathCount );
			pDlg->m_sttThTotal.SetWindowText( strThTotal );

            // 5.3. download complete
            static char szUrl[512];    
            memset( szUrl, 0, sizeof(szUrl)/sizeof(char));
            strncpy( szUrl, strSrcPath.GetString(), sizeof(szUrl)/sizeof(char) - 1 );
			DEBUG_LOG( "++     szUrl=%s", szUrl );
            SEND_UI_THREAD( MY_MSG_U_COMPLETE_DOWNLOAD_EACH_ITEM, (WPARAM)&szUrl, (LPARAM)NULL );
		} else {
			LPTSTR lpszErrorMessage;
			CString strMsg;

			if ( FormatMessage(   FORMAT_MESSAGE_ALLOCATE_BUFFER 
								| FORMAT_MESSAGE_FROM_SYSTEM 
								| FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, 
								hr, 
								MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),
								(LPTSTR) &lpszErrorMessage, 
								0, 
								NULL )) {
				strMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\n%s"), (DWORD) hr, lpszErrorMessage );
				LocalFree ( lpszErrorMessage );
			} else {
				strMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\nNo message available."), (DWORD) hr );
            }

			ERROR_LOG( "++     [ERROR]" + strMsg );
			pDlg->SetDownloadSuccess( FALSE );
		}
	}

	// 98. 마무리
	pDlg->m_pgTotal.SetPos ( 100 );

	// 99. end download
	pDlg->SetTimer( MY_TIMER_DOWNLOAD_END, 200, NULL );

    return 0;
}

void CDownloadDlg::AddDownloadPath( CString strSrc, CString strDesc, int nSize )
{
	DEBUG_LOG( ">> CDownloadDlg::AddDownloadPath()" );

	m_ayStrSrcPath.Add(                   strSrc   );
	m_ayStrDescPath.Add(                  strDesc  );
    m_ayStrSize.Add(     CUtil2::Int2Str( nSize   ));

	m_nDownloadPathCount++; 
}


////////////////////////////////////////////////////////////////////////////////
// utiliity
////////////////////////////////////////////////////////////////////////////////

void CDownloadDlg::ProgressUpdate (  LPCTSTR	szIEMsg,
									 LPCTSTR	szCustomMsg,
									 const int  nPercentDone,
									 ULONG		ulProgress )
{
	//-DEBUG_LOG( ">> CDownloadDlg::ProgressUpdate()" );

    ASSERT ( AfxIsValidString ( szIEMsg ));
    ASSERT ( AfxIsValidString ( szCustomMsg ));
    ASSERT ( nPercentDone >= 0  &&  nPercentDone <= 100 );

    static int i = 0;
    i++;
    if( i % 100 == 0 ) {
	    // 1. file download rate
        m_pgDownload.SetPos ( nPercentDone );

	    // 2. status
        m_sttStatusMessage.SetWindowText ( szCustomMsg );

        // 3. send parent 
        if( m_pParentView != NULL ) {

            ULONG ulDownloadDelta = ( ulProgress - m_ulPreProgress );
            //-DEBUG_LOG( "++   ulDownloadDelta=%d( %d - %d )", ulDownloadDelta, ulProgress, m_ulPreProgress );

#ifdef FEATURE_RELEASE_VIEW 
            ((CUiReleaseView*)m_pParentView)->GetSplash()->SetSubText( "Downloading .... " + m_strCurFileName + " .... " + szCustomMsg );
            ((CUiReleaseView*)m_pParentView)->GetSplash()->StepDownload( ulDownloadDelta );
#else
            ((CUiDebugView*)m_pParentView)->GetSplash()->SetSubText( "Downloading .... " + m_strCurFileName + " .... " + szCustomMsg );
            ((CUiDebugView*)m_pParentView)->GetSplash()->StepDownload( ulDownloadDelta );
#endif

            m_ulPreProgress = ulProgress;
        }
    }
}