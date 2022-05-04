// MobigoSecureUpgraderView.cpp : CSecondStepSplash 클래스의 구현
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecondStepSplash.h"
#include "UiSplashView.h"
#include "MobigoSecureUpgrader.h"
#include "MobigoSecureUpgraderDataMgr.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CSecondStepSplash::CSecondStepSplash()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CSecondStepSplash::~CSecondStepSplash()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// debug
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// splash 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// [2단계] Upgrade가 필요한 item을 Download 받는다.
////////////////////////////////////////////////////////////////////////////////

void CSecondStepSplash::Step()
{
	DEBUG_LOG(">> CSecondStepSplash::Step()");

    // splash 
	m_pSplashDlg->SetMainText("버전 다운로드 중... ... ... ");
	m_pSplashDlg->SetMainPercent("20%");
	m_pSplashDlg->SetProgressPos( SPLASH_PROGRESS_POS_UPGRADE_LIST );

    BOOL bContinue = ShowDownloadDlg();   
    if( bContinue == FALSE ) {
	    DEBUG_LOG("++   download cancel");
        return;
    }

    // Complete First Step
    POST_UI_THREAD( MY_MSG_U_COMPLETE_SECOND_STEP, NULL, NULL );
}

////////////////////////////////////////////////////////////////////////////////
// utility 
////////////////////////////////////////////////////////////////////////////////

BOOL CSecondStepSplash::ShowDownloadDlg()
{
	DEBUG_LOG(">> CSecondStepSplash::ShowDownloadDlg()");

    CDataMgr*           pDataMgr        = (CDataMgr*)GET_DATA_MGR();
    CUpgradeData*       pUpgradeData    = pDataMgr->GetUpgradeData();
    //int                 nDownloadSize   = pUpgradeData->CalcDownloadSize();

	//DEBUG_LOG("++   nDownloadSize=%d", nDownloadSize );

	// 1. dialog 생성
	CDownloadDlg dlg;
		
	// 2. 필요 변수 설정
    dlg.SetParentView( m_pWndParent );
	//dlg.SetTotalSize(  nDownloadSize );

    // 3. down할 file path 추가 
    AddUrlPathIntoDialog( pUpgradeData, &dlg );

#ifdef FEATURE_RUN_BACKGROUND
    int nTotalDownloadSize = m_pSplashDlg->GetTotalDownloadSize();
	DEBUG_LOG("++   nTotalDownloadSize=%d", nTotalDownloadSize );
    // upgrade할 항목이 있음
    if( 0 < nTotalDownloadSize ) {
        // C:\\mobigo\\MobigoSecureApp.exe"가 존재하지 않으면 무조건 업그레이드
        int nYesNo = IDYES;
        if( CUtilFile::IsExistPath( MOBIGO_SECURE_APP_EXE_PATH ) == TRUE ) {

#ifdef FEATURE_HTML_MESSAGE_BOX
            CString strTitle = "모비고 시큐어 업데이트 안내"; 
            CString strMessage = "모비고 시큐어 버전 업데이트가 필요합니다.<br>확인을 누르면 최신 버전으로 자동 업데이트 됩니다.";
		    nYesNo = CONFIRM( strMessage, strTitle );
#else 
            CString strTitle = "모비고 시큐어 업데이트 안내"; 
            CString strMessage = "모비고 시큐어 버전 업데이트가 필요합니다.\r\n\r\n확인을 누르면 최신 버전으로 자동 업데이트 됩니다.";

    #ifdef _DEBUG
            nYesNo = ::MessageBox( NULL, strMessage, strTitle, MB_ICONINFORMATION | MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST );
    #else
            nYesNo = ::MessageBox( NULL, strMessage, strTitle, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
    #endif

#endif
        }
        // upgrade를 "Yes"했으므로, MobigoSecureApp 종료 후, upgrade 시작 
        if( nYesNo == IDYES || nYesNo == IDOK ) {

            // disk 용량 체크 
            CString strDisk = CMyEnv::GetRootHardDisk();
            long lFreeSpace  = CUtilSys::GetDiskFreeSpace( strDisk, FALSE /* == MEGA */ );
            if( lFreeSpace < MINIMUM_DISK_SPACE ) {
                strDisk.Remove( '\\' );
                CString strTitle = _T("저장공간 부족"); 
                CString strMessage = _T("저장공간이 충분하지 않습니다.\r\n\r\n");
                strMessage += "모비고 시큐어가 원활하게 동작할 수 있도록 하드디스크 저장공간 확보 후 다시 시도하여 주세요.\r\n\r\n";
                strMessage += _T("( ");
                strMessage += strDisk + _T(" 남은 용량 ") + CUtil2::Int2Money(lFreeSpace) + _T("MByte");
                strMessage += _T(" 최소 1GByte의 저장공간 필요.");
                strMessage += _T(" )");

                ::MessageBox( NULL, strMessage, strTitle, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
                DEBUG_LOG("++   QUIT_APP(-1)" );
                QUIT_APP(-1);
                return FALSE;
            } else {
                m_bBackground = FALSE;
                CStepSplash::KillMobigoSecureApp();
                CUtilSys::ActivateWindow( m_pWndParent->GetSafeHwnd() );
            }

        // upgrade를 "No"했으므로 종료.
        } else {
#ifndef FEATURE_RELEASE_VIEW
            return FALSE;
#else
            DEBUG_LOG("++   QUIT_APP(-1)" );
            QUIT_APP(-1);
            return FALSE;
#endif
        }
    // upgrade할 항목이 없으므로 종료
    } else {
#ifdef FEATURE_RELEASE_VIEW
#if 0 // 업그레이드가 백그라운로 동작중임을 알릴려는 디버그용 기능이므로, Disable 처리함.
        // mobigo secure app아 아무런 변화가 없으면 심심하므로, MAXIMIZE <-> RESTORE 하자 
        HWND hWnd = CUtilSys::FindAllWindowByPartialName( MOBIGO_SECURE_APP_NAME );
        if( hWnd != NULL ) {
            LONG style = ::GetWindowLong( hWnd, GWL_STYLE );      
            if(style & WS_MAXIMIZE) {
                ::ShowWindow( hWnd, SW_RESTORE );
            } else {
                ::ShowWindow( hWnd, SW_MAXIMIZE );
            }
        } 
#endif
#endif

#ifdef FEATURE_RELEASE_VIEW
        DEBUG_LOG("++   QUIT_APP(-1)" );
        QUIT_APP(-1);
        return FALSE;
#else
        return FALSE;
#endif
    }
#endif

	// 4. download start
	UINT nOK = dlg.DoModal();

	// 5. download 완료
	if( nOK == IDOK ) {
		;
	}

    return TRUE;
}

void CSecondStepSplash::AddUrlPathIntoDialog( CUpgradeData* pUpgradeData, CDownloadDlg *pDlg )
{
	DEBUG_LOG(">> CSecondStepSplash::AddUrlPathIntoDialog()");

    CMapStringToString* pMapUrl2Path  = new CMapStringToString();
    {
        pUpgradeData->FillUrl2PathMap(pMapUrl2Path);

        POSITION iPos           = pMapUrl2Path->GetStartPosition();
        CString strUrl          = "";
        CString strPath         = "";

        int nTotalDownloadSize  = 0;
        int i                   = 0;

        while(iPos != NULL) {
            pMapUrl2Path->GetNextAssoc(iPos, strUrl, strPath);

		    // normalize path
		    strUrl.Replace( '\\', '/' );
		    strPath.Replace( '\\', '/' );

		    // add path
            int nSize = 0;
            CUpgradeItem* pUpgradeItem = pUpgradeData->FindUpgradeItemByDownloadUrl(strUrl);
            if( pUpgradeItem != NULL ) {
                nSize = pUpgradeItem->GetSize();
            }
		    pDlg->AddDownloadPath( strUrl, strPath, nSize );

            DEBUG_LOG("++   [%d]", i++ );
            if( pUpgradeItem != NULL ) {
             	DEBUG_LOG("++     id      = "   + pUpgradeItem->GetID() );
            }
        	DEBUG_LOG("++     strUrl  = "   + strUrl                );
        	DEBUG_LOG("++     strPath = "   + strPath               );
        	DEBUG_LOG("++     nSize   = %d",  nSize                 );

            nTotalDownloadSize += nSize;
        }
        DEBUG_LOG("++   nTotalDownloadSize   = %d",  nTotalDownloadSize );

        m_pSplashDlg->SetTotalDownloadSize( nTotalDownloadSize );
    }
    pMapUrl2Path->RemoveAll(); 
    delete pMapUrl2Path;
    pMapUrl2Path = NULL;
} 
