// MobigoSecureUpgraderView.cpp : CFirstStepSplash Ŭ������ ����
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FirstStepSplash.h"
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

CFirstStepSplash::CFirstStepSplash()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CFirstStepSplash::~CFirstStepSplash()
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
// [1�ܰ�] upgrade list�� ������ ���� �о� ����, ������ up �Ǿ��� �� check�Ѵ�.
////////////////////////////////////////////////////////////////////////////////

void CFirstStepSplash::Step()
{
	DEBUG_LOG(">> CFirstStepSplash::Step()");

    // request upgrade list
    RequestUpgradeList();

     // Check Version
    CheckDownloadList();

     // Check Version
    CheckUnzipList();

    // Splash 
    m_pSplashDlg->SetMainText("���� Ȯ�� �� ... ... ... ");
    m_pSplashDlg->SetMainPercent("10%");
    m_pSplashDlg->SetProgressPos( SPLASH_PROGRESS_POS_INIT );
    int nPos = m_pSplashDlg->GetProgressPos();
	DEBUG_LOG("++   nPos=%d", nPos );

    // Complete First Step
    POST_UI_THREAD( MY_MSG_U_COMPLETE_FIRST_STEP, NULL, NULL );
}

////////////////////////////////////////////////////////////////////////////////
// utility 
////////////////////////////////////////////////////////////////////////////////

void CFirstStepSplash::RequestUpgradeList()
{
	DEBUG_LOG(">> CFirstStepSplash::RequestUpgradeList()");

    CDataMgr    *pDataMgr   = (CDataMgr*)GET_DATA_MGR();
	DEBUG_LOG("++   URL_UPGRADE_LIST_JSON=%s", URL_UPGRADE_LIST_JSON );

    CString strUpgradeListJson = "";
    int nRetryCount = 0;
    for( nRetryCount = 0; nRetryCount < HTTP_RETRY_COUNT; nRetryCount++ ) {
        strUpgradeListJson = CUtilHttpGet::Request( URL_UPGRADE_LIST_JSON );
        /* Yes( Retry ) */
        if(        CUtilHttpGet::GetYesNo() == IDYES  ) {
	        DEBUG_LOG("++   strUpgradeListJson = " + strUpgradeListJson  );
            continue;
        /* No */
        } else if( CUtilHttpGet::GetYesNo() == IDNO  ) {
	        DEBUG_LOG("++   strUpgradeListJson = " + strUpgradeListJson  );
            ::MessageBox( NULL, "Network Error�� ���ؼ� ���α׷��� �����մϴ�.", "Network Error", MB_ICONSTOP | MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
            QUIT_APP(-1);	        
            DEBUG_LOG("++   QUIT_APP(-1)" );        
			return;
        /* No Error */
        } else {
            break;
        }
    } 

    // network error
    if( nRetryCount == HTTP_RETRY_COUNT ) {
        DEBUG_LOG("++   strUpgradeListJson = " + strUpgradeListJson  );
        ::MessageBox( NULL, "Network Error�� ���ؼ� ���α׷��� �����մϴ�.", "Network Error", MB_ICONSTOP | MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
	    DEBUG_LOG("++   QUIT_APP(-1)" );        
        QUIT_APP(-1);
		return ;
    }

#ifdef FEATURE_REPLACE_SAVE_PATH
    strUpgradeListJson.Remove( '\\' );

    CString strMobigoRootPath(MOBIGO_DEFAULT_ROOT_PATH);
    CString strMobigoUpgraderRootPath(MOBIGO_UPGRADE_DOWNLOD_PATH);

    strMobigoRootPath        .Replace( '\\', '/' ); // "C:\\mobigo"           -> "C:/mobigo"
    strMobigoUpgraderRootPath.Replace( '\\', '/' ); // "C:\\mobigo\\upgrader" -> "C:/mobigo/upgrader"

    strUpgradeListJson.Replace( strMobigoRootPath,           // "C:/mobigo"
                                strMobigoUpgraderRootPath ); // "C:/mobigo/upgrader"

	DEBUG_LOG("++   strUpgradeListJson=" + strUpgradeListJson );

    CUpgradeData *pUpgradeData = pDataMgr->GetUpgradeDataLoader()->Parse( strUpgradeListJson );
    pDataMgr->SetUpgradeData( pUpgradeData );
#else
    /** load */
    CUpgradeData *pUpgradeData = pDataMgr->GetUpgradeDataLoader()->Load( URL_UPGRADE_LIST_JSON );
    pDataMgr->SetUpgradeData( pUpgradeData );
#endif

#ifdef _DEBUG
    // original json file 
    CString strUpgradeListNowPath = CUtilFile::ExtractPath(MOBIGO_UPGRADE_LIST_PATH) + "\\" + CUtilTime::GetLocalDateTime() + "_" + CUtilFile::ExtractFileName(MOBIGO_UPGRADE_LIST_PATH); 
    CUtilFile::WriteString( strUpgradeListNowPath, strUpgradeListJson );
#endif

	DEBUG_LOG_G( pUpgradeData->ToJson( "" ) );
}

void CFirstStepSplash::CheckDownloadList()
{
	DEBUG_LOG(">> CFirstStepSplash::CheckDownloadList()");

    CMobigoSecureUpgraderDataMgr*   pAppDataMgr              = ((CMobigoSecureUpgraderApp*)AfxGetApp())->GetDataMgr();
    CUpgradeData*                   pAppUpgradeData          = pAppDataMgr->GetUpgradeData();

    CDataMgr*                       pCurDataMgr              = (CDataMgr*)GET_DATA_MGR();
    CUpgradeData*                   pCurUpgradeData          = pCurDataMgr->GetUpgradeData();
        
    CMapStringToPtr*                pCurMapID2Ptr            = new CMapStringToPtr();
    {
        pCurUpgradeData->FillID2PtrMap(pCurMapID2Ptr);

        POSITION       iPos             = pCurMapID2Ptr->GetStartPosition();
        CString        strID            = "";
        CUpgradeItem*  pCurUpgradeItem  = NULL;
        int            i                = 0;

        while(iPos != NULL) {
            pCurMapID2Ptr->GetNextAssoc(iPos, strID, (void*&)pCurUpgradeItem );

            DEBUG_LOG("++   [%d]", i++ );
        	DEBUG_LOG("++     strID           = " + strID );
        	DEBUG_LOG("++     pCurUpgradeItem = 0x%x", pCurUpgradeItem );

            pCurUpgradeItem->SetUpgrade( FALSE );
            pCurUpgradeItem->SetDownloadComplete( FALSE );

            BOOL bDownloadable = IsDownloadableItem(pCurUpgradeItem);
        	DEBUG_LOG("++     bDownloadable = %s", B2S( bDownloadable ) );

            if( bDownloadable == TRUE ) {
                CUpgradeItem*  pPreUpgradeItem = pAppUpgradeData->FindUpgradeItemByID( strID );
                if( pPreUpgradeItem == NULL ) {
                    DEBUG_LOG("++     [Need-Upgrade-Case-1] No Pre-Version" );
                    pCurUpgradeItem->SetUpgrade( TRUE );
                    pCurUpgradeItem->SetDownloadComplete( FALSE );
                } else {
                    DEBUG_LOG("++     pre_version=" + pPreUpgradeItem->GetVersion() );
        	        DEBUG_LOG("++     cur_version=" + pCurUpgradeItem->GetVersion() );
        	        DEBUG_LOG("++     pre_download_complete=%s", B2S( pPreUpgradeItem->IsDownloadComplete()) );

                    pCurUpgradeItem->SetDownloadComplete( pPreUpgradeItem->IsDownloadComplete() );

                    if(    pCurUpgradeItem->GetVersion().IsEmpty() == FALSE /** list have no version */
                        && pPreUpgradeItem->GetVersion().IsEmpty() == FALSE /** list have no version */
                        && 0 < pCurUpgradeItem->CompareVersion( pPreUpgradeItem ) ) {
                        DEBUG_LOG("++     [Need-Upgrade-Case-2] Version Up : " +  pPreUpgradeItem->GetVersion() + " --> " + pCurUpgradeItem->GetVersion() );
                        pCurUpgradeItem->SetUpgrade( TRUE );
                        pCurUpgradeItem->SetDownloadComplete( FALSE );
                    } else {
                        if( pCurUpgradeItem->GetDownloadUrl().IsEmpty() == FALSE ) {
                            int nLocalSize  = CUtilFile::GetSize(pCurUpgradeItem->GetSavePath()) ;
                            int nServerSize = pCurUpgradeItem->GetSize();

                            DEBUG_LOG("++     save_path=" + pCurUpgradeItem->GetSavePath() );
        	                DEBUG_LOG("++     nLocalSize=%d", nLocalSize );
        	                DEBUG_LOG("++     nServerSize=%d", nServerSize );

                            // file �������� ���� ���.
                            if( CUtilFile::IsExistPath( pCurUpgradeItem->GetSavePath()) == FALSE ) {
                                DEBUG_LOG("++     [Need-Upgrade-Case-3] Not Exist File : " + pCurUpgradeItem->GetSavePath() );
                                pCurUpgradeItem->SetUpgrade( TRUE );
                                pCurUpgradeItem->SetDownloadComplete( FALSE );
                            } else {
                                // download�� �Ϸ� ���� ���� ���( �ٿ�ε� �� ��ҵ� ��� )
                                BOOL bDownloadComplete = pPreUpgradeItem->IsDownloadComplete();
                                DEBUG_LOG("++     bDownloadComplete=%s", B2S( bDownloadComplete ) );
                                if( bDownloadComplete == FALSE ) {
                                    DEBUG_LOG("++     [Need-Upgrade-Case-4] Not Complete Download : bDownloadComplete=" + bDownloadComplete );
                                    pCurUpgradeItem->SetUpgrade( TRUE );
                                    pCurUpgradeItem->SetDownloadComplete( FALSE );
                                } else {
                                    // file �뷮�� ���� ���� ���
                                    //  - ���� ������ �ִ� ���ϰ�, �������� �˷��� ���� ũ�Ⱑ �ٸ� ���( ���� ���׷� ���� ) 
                                    if( nLocalSize != nServerSize ) {
                                        ERROR_LOG("++     [ERROR][Need-Upgrade-Case-5] Not Equal Size : nLocalSize(%d) != nServerSize(%d)", nLocalSize, nServerSize );
                                        pCurUpgradeItem->SetUpgrade( TRUE );        
                                        pCurUpgradeItem->SetDownloadComplete( FALSE );
                                    } else {
                                        DEBUG_LOG("++     [Need-Upgrade-Case-6] Need Not Upgrade" );
                                    }
                                }
                            }
                        } else {
                            DEBUG_LOG("++     [Need-Upgrade-Case-7] Download URL Empty : List have no download-url" );
                        }
                    }
                } 
            /** download ���� �ʿ� ���� */
            } else {
                DEBUG_LOG("++     [Need-Upgrade-Case-7] Need Not Upgrade" );
            }
        }
    }
    pCurMapID2Ptr->RemoveAll(); 
    delete pCurMapID2Ptr;
    pCurMapID2Ptr = NULL;
}

BOOL CFirstStepSplash::IsDownloadableItem( CUpgradeItem* pCurUpgradeItem)
{
	DEBUG_LOG(">> CFirstStepSplash::IsDownloadableItem()");

    BOOL        b64BitOS       = CUtilOS::Is64BitOS();
    CString     strSavePath    = pCurUpgradeItem->GetSavePath();
    CString     strFileName    = CUtilFile::ExtractFileName(strSavePath);
            
    if( strFileName.CompareNoCase( DRIVE_EX_64_ZIP ) == 0 ) {
		if( b64BitOS == TRUE ) {
	        DEBUG_LOG("--   return( TRUE )");
        	return TRUE;
        } else {
	        DEBUG_LOG("--   return( FALSE )");
        	return FALSE;
        }
    } else if( strFileName.CompareNoCase( DRIVE_EX_32_ZIP ) == 0 ) {
        if( b64BitOS == FALSE ) {  
	        DEBUG_LOG("--   return( TRUE )");
        	return TRUE;
        } else {
	        DEBUG_LOG("--   return( FALSE )");
        	return FALSE;
        }
    }

	DEBUG_LOG("--   return( TRUE )");
    return TRUE;
}

void CFirstStepSplash::CheckUnzipList()
{
	DEBUG_LOG(">> CFirstStepSplash::CheckUnzipList()");

    CMobigoSecureUpgraderDataMgr*   pAppDataMgr              = ((CMobigoSecureUpgraderApp*)AfxGetApp())->GetDataMgr();
    CUpgradeData*                   pAppUpgradeData          = pAppDataMgr->GetUpgradeData();

    CDataMgr*                       pCurDataMgr              = (CDataMgr*)GET_DATA_MGR();
    CUpgradeData*                   pCurUpgradeData          = pCurDataMgr->GetUpgradeData();
        
    CMapStringToPtr*                pCurMapID2Ptr            = new CMapStringToPtr();
    {
        pCurUpgradeData->FillID2PtrMap(pCurMapID2Ptr);

        POSITION       iPos             = pCurMapID2Ptr->GetStartPosition();
        CString        strID            = "";
        CUpgradeItem*  pCurUpgradeItem  = NULL;
        int            i                = 0;
        while(iPos != NULL) {
            pCurMapID2Ptr->GetNextAssoc(iPos, strID, (void*&)pCurUpgradeItem );

            DEBUG_LOG("++   [%d]", i++ );
        	DEBUG_LOG("++     strID           = " + strID );
        	DEBUG_LOG("++     pCurUpgradeItem = 0x%x", pCurUpgradeItem );

            pCurUpgradeItem->SetUnzipComplete( TRUE );

            // zip ���� ���� Ȯ��
            if(    pCurUpgradeItem->IsZipFile() == TRUE 
#ifdef FEATURE_FILL_SYSTEM_APK_VERSION
                || pCurUpgradeItem->IsApkFile() == TRUE 
#endif
              ) {
                // upgrade�� ��쿡�� unzip�Ѵ�. 
                if( pCurUpgradeItem->IsUpgrade() == TRUE ) {
                    pCurUpgradeItem->SetUnzipComplete( FALSE );
                } else {
                    CUpgradeItem*  pPreUpgradeItem = pAppUpgradeData->FindUpgradeItemByID( strID );
                    // ���� upgrade ������ ������ unzip�Ѵ�. 
                    if( pPreUpgradeItem == NULL ) {
                        pCurUpgradeItem->SetUnzipComplete( FALSE );
                    } else {
                        // Unzip�� �Ϸ���� �ʾ����� unzip�Ѵ�. 
                        if( pPreUpgradeItem->IsUnzipComplete() == FALSE ) {
                            pCurUpgradeItem->SetUnzipComplete( FALSE );
                        } else {
                            CString strUnzipPath = pCurUpgradeItem->GetUnzipPath();
                            // Unzip�ؾ��� Dir�� ������ unzip�Ѵ�. 
                            if( CUtilFile::IsExistPath(strUnzipPath) == FALSE ) {
                                pCurUpgradeItem->SetUnzipComplete( FALSE );
                            } else {
                                int nSizeOfUnzipPath = CUtilFile::GetDirectorySize(strUnzipPath);
                                int nServerSize      = pCurUpgradeItem->GetSize();

        	                    DEBUG_LOG("++     nSizeOfUnzipPath=%d", nSizeOfUnzipPath );
        	                    DEBUG_LOG("++     nServerSize=%d",      nServerSize );
                                // Unzip�ؾ��� Dir�� size�� Server���� �˷��� size���� �۰ų� ���� ���
                                if( nSizeOfUnzipPath <= nServerSize ) {
                                    pCurUpgradeItem->SetUnzipComplete( FALSE );
                                }
                            }
                        }
                    }
                }
            } 
        }
    }
    pCurMapID2Ptr->RemoveAll(); 
    delete pCurMapID2Ptr;
    pCurMapID2Ptr = NULL;
}
