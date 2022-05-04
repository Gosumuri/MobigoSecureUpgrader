//////////////////////////////////////////////////////////////////////
//
// Written by Michael Dunn (mdunn at inreach dot com).  Copyright and all
// that stuff.  Use however you like but give me credit where it's due.  I'll
// know if you don't. bwa ha ha ha ha!
// 
// Release history:
//   December 24, 1999: Version 1.0.  First release.
//
//////////////////////////////////////////////////////////////////////

// BindStatusCallback.cpp: implementation of the CBindStatusCallback class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BindStatusCallback.h"
#include <shlwapi.h>                    // for StrFormatByteSize()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LONG CCallback::m_fAbortDownload = 0;

CCallback::CCallback() : m_pDlg(NULL)
{
    m_pDlg          = NULL;
    m_nDownloadSize = 0;
}

CCallback::~CCallback()
{
}

HRESULT CCallback::OnProgress ( ULONG ulProgress,   
                                ULONG ulProgressMax,
                                ULONG ulStatusCode, 
                                LPCWSTR wszStatusText )
{
	//-DEBUG_LOG(">> CCallback::OnProgress()");

    // Local variables are declared static so they don't have to be reallocated on
    // the stack every time.  This is safe in this app since I know I'll only have
    // one thread downloading.
    static CString sIEStatusMsg;
    static TCHAR   szCustomStatusMsg [256];
    static TCHAR   szAmtDownloaded [256], szTotalSize [256];

    UNREFERENCED_PARAMETER(ulStatusCode);

    // Did the user hit the Stop button?
    if ( 0 != CCallback::m_fAbortDownload ) {
        return E_ABORT;
    }

    // Use CString to convert IE's status message to a TCHAR string.
    if ( NULL != wszStatusText ) {
        sIEStatusMsg = wszStatusText;
    } else {
        sIEStatusMsg.Empty();
    }

    // http reponse에 size 표시가 안된 경우
    if( ulProgress == ulProgressMax ) {
        ulProgressMax = m_nDownloadSize;
    }

    // ulProgress가 ulProgressMax보다 클 수 없다. 
    // - png icon에 대한 size 관련 정보가 없으므로, warning 대상이 아니다 */
    if( 0 < ulProgressMax && 0 < ulProgress && ulProgressMax < ulProgress ) {
        ulProgress = ulProgressMax;
        WARNING_LOG( "++   [WARNING] ulProgressMax(%d) < ulProgress(%d)", ulProgressMax, ulProgress );
    } 

    // Make our own progress message - we'll show the amount downloaded and
    // the total file size (if known).
    StrFormatByteSize ( ulProgress, szAmtDownloaded, 256 );
    StrFormatByteSize ( ulProgressMax, szTotalSize, 256 );

    if ( 0 != ulProgressMax ) {
        wsprintf (	szCustomStatusMsg, 
					_T(" %s of %s"),
					szAmtDownloaded, 
					szTotalSize );
    } else {
        wsprintf (	szCustomStatusMsg, 
					_T("Downloaded %s (total size unknown)"),
					szAmtDownloaded );
    }

    // Report the progress back to the main window.

    if ( 0 != ulProgressMax ) {
        m_pDlg->ProgressUpdate (	sIEStatusMsg, 
									szCustomStatusMsg,
									int( 100.0 * ulProgress / ulProgressMax), 
									ulProgress );
	} else {
        m_pDlg->ProgressUpdate (	sIEStatusMsg, 
									szCustomStatusMsg, 
									0,
									ulProgress );
	}

    return S_OK;
}
