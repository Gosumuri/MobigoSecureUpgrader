// MobigoSecureUpgraderView.cpp : CSecondStepSplash Ŭ������ ����
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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
// [2�ܰ�] Upgrade�� �ʿ��� item�� Download �޴´�.
////////////////////////////////////////////////////////////////////////////////

void CSecondStepSplash::Step()
{
	DEBUG_LOG(">> CSecondStepSplash::Step()");

    // splash 
	m_pSplashDlg->SetMainText("���� �ٿ�ε� ��... ... ... ");
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

	// 1. dialog ����
	CDownloadDlg dlg;
		
	// 2. �ʿ� ���� ����
    dlg.SetParentView( m_pWndParent );
	//dlg.SetTotalSize(  nDownloadSize );

    // 3. down�� file path �߰� 
    AddUrlPathIntoDialog( pUpgradeData, &dlg );

#ifdef FEATURE_RUN_BACKGROUND
    int nTotalDownloadSize = m_pSplashDlg->GetTotalDownloadSize();
	DEBUG_LOG("++   nTotalDownloadSize=%d", nTotalDownloadSize );
    // upgrade�� �׸��� ����
    if( 0 < nTotalDownloadSize ) {
        // C:\\mobigo\\MobigoSecureApp.exe"�� �������� ������ ������ ���׷��̵�
        int nYesNo = IDYES;
        if( CUtilFile::IsExistPath( MOBIGO_SECURE_APP_EXE_PATH ) == TRUE ) {

#ifdef FEATURE_HTML_MESSAGE_BOX
            CString strTitle = "���� ��ť�� ������Ʈ �ȳ�"; 
            CString strMessage = "���� ��ť�� ���� ������Ʈ�� �ʿ��մϴ�.<br>Ȯ���� ������ �ֽ� �������� �ڵ� ������Ʈ �˴ϴ�.";
		    nYesNo = CONFIRM( strMessage, strTitle );
#else 
            CString strTitle = "���� ��ť�� ������Ʈ �ȳ�"; 
            CString strMessage = "���� ��ť�� ���� ������Ʈ�� �ʿ��մϴ�.\r\n\r\nȮ���� ������ �ֽ� �������� �ڵ� ������Ʈ �˴ϴ�.";

    #ifdef _DEBUG
            nYesNo = ::MessageBox( NULL, strMessage, strTitle, MB_ICONINFORMATION | MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST );
    #else
            nYesNo = ::MessageBox( NULL, strMessage, strTitle, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
    #endif

#endif
        }
        // upgrade�� "Yes"�����Ƿ�, MobigoSecureApp ���� ��, upgrade ���� 
        if( nYesNo == IDYES || nYesNo == IDOK ) {

            // disk �뷮 üũ 
            CString strDisk = CMyEnv::GetRootHardDisk();
            long lFreeSpace  = CUtilSys::GetDiskFreeSpace( strDisk, FALSE /* == MEGA */ );
            if( lFreeSpace < MINIMUM_DISK_SPACE ) {
                strDisk.Remove( '\\' );
                CString strTitle = _T("������� ����"); 
                CString strMessage = _T("��������� ������� �ʽ��ϴ�.\r\n\r\n");
                strMessage += "���� ��ť� ��Ȱ�ϰ� ������ �� �ֵ��� �ϵ��ũ ������� Ȯ�� �� �ٽ� �õ��Ͽ� �ּ���.\r\n\r\n";
                strMessage += _T("( ");
                strMessage += strDisk + _T(" ���� �뷮 ") + CUtil2::Int2Money(lFreeSpace) + _T("MByte");
                strMessage += _T(" �ּ� 1GByte�� ������� �ʿ�.");
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

        // upgrade�� "No"�����Ƿ� ����.
        } else {
#ifndef FEATURE_RELEASE_VIEW
            return FALSE;
#else
            DEBUG_LOG("++   QUIT_APP(-1)" );
            QUIT_APP(-1);
            return FALSE;
#endif
        }
    // upgrade�� �׸��� �����Ƿ� ����
    } else {
#ifdef FEATURE_RELEASE_VIEW
#if 0 // ���׷��̵尡 ��׶��� ���������� �˸����� ����׿� ����̹Ƿ�, Disable ó����.
        // mobigo secure app�� �ƹ��� ��ȭ�� ������ �ɽ��ϹǷ�, MAXIMIZE <-> RESTORE ���� 
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

	// 5. download �Ϸ�
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
