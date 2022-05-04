// MobigoSecureUpgraderView.cpp : CThirdStepSplash 클래스의 구현
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThirdStepSplash.h"
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

CThirdStepSplash::CThirdStepSplash()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CThirdStepSplash::~CThirdStepSplash()
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
// [3단계] download 받은 item은 zip은 압축을 해제한다. 
////////////////////////////////////////////////////////////////////////////////

void CThirdStepSplash::Step()
{
	DEBUG_LOG(">> CThirdStepSplash::Step()");

    // splash 
	m_pSplashDlg->SetMainText("드라이버 업데이트 중... ... ... ");
    m_pSplashDlg->SetMainPercent("70%");
	m_pSplashDlg->SetProgressPos( SPLASH_PROGRESS_POS_DOWNLOAD );

    ShowZipDlg();

    #ifdef FEATURE_FILL_SYSTEM_APK_VERSION
        FillSystemApkVersion();
    #endif

    // Complete First Step
    POST_UI_THREAD( MY_MSG_U_COMPLETE_THIRD_STEP, NULL, NULL );
}

////////////////////////////////////////////////////////////////////////////////
// utility 
////////////////////////////////////////////////////////////////////////////////

void CThirdStepSplash::ShowZipDlg()
{
	DEBUG_LOG(">> CThirdStepSplash::ShowZipDlg()");

    CDataMgr*           pDataMgr      = (CDataMgr*)GET_DATA_MGR();
    CUpgradeData*       pUpgradeData  = pDataMgr->GetUpgradeData();

	// 1. dialog 생성
	CZip2DirDlg dlg;
		
	// 2. 필요 변수 설정
    dlg.SetParentView( m_pWndParent );

    // 3. unzip할 file path 추가 
    AddZipDirIntoDialog( pUpgradeData, &dlg );

    // 4. zip file count 
    int nZipFileCount = dlg.m_ayZipPath.GetCount();
	DEBUG_LOG("++   nZipFileCount=%d", nZipFileCount );

	m_pSplashDlg->SetZipFileCount( nZipFileCount );

	// 5. zipping start
	UINT nOK = dlg.DoModal();

	// 6. zipping 완료
	if( nOK == IDOK ) {
		;
	}
}

void CThirdStepSplash::AddZipDirIntoDialog( CUpgradeData* pUpgradeData, CZip2DirDlg *pDlg )
{
	DEBUG_LOG(">> CThirdStepSplash::AddZipDirIntoDialog()");

    CUpgradeItem        *pSetupItem     = pUpgradeData->FindUpgradeItemByID( ITEM_ID_MOBIGO_SECURE_SETUP );
    BOOL                b64BitOS        = CUtilOS::Is64BitOS();
    CMapStringToString* pMapZip2Dir     = new CMapStringToString();
    {

        pUpgradeData->FillZip2DirMap(pMapZip2Dir);

        POSITION    iPos        = pMapZip2Dir->GetStartPosition();
        CString     strZipPath  = "";
        CString     strDirPath  = "";
        int         i           = 0;

        while(iPos != NULL) {
            pMapZip2Dir->GetNextAssoc(iPos, strZipPath, strDirPath);

		    // normalize path
		    strZipPath.Replace( '\\', '/' );
		    strDirPath.Replace( '\\', '/' );

#ifdef FEATURE_REPLACE_DRIVE_EX_PATH
            strDirPath.Replace( "upgrader/download/DriverEx/Common", "DriverEx" );
#endif
#ifdef FEATURE_REPLACE_MOBIGO_SECURE_SETUP
            // C:/mobigo/upgrader/download/download -> C:/mobigo
            if( pSetupItem != NULL ) { 
                CString strSavePath = pSetupItem->GetSavePath();
                strSavePath.Replace( '\\', '/' );
        	    DEBUG_LOG("++     strSavePath=" + strSavePath );
        	    DEBUG_LOG("++     strZipPath="  + strZipPath );
                if( strSavePath == strZipPath ) {
                    strDirPath = MOBIGO_ROOT_PATH;   
                } 
            }
#endif
            DEBUG_LOG("++   [%d]", i++ );
        	DEBUG_LOG("++     strZipPath=" + strZipPath );
        	DEBUG_LOG("++     strDirPath=" + strDirPath );

            CString strZipFileName = CUtilFile::ExtractFileName(strZipPath);
            
            if( strZipFileName.CompareNoCase( DRIVE_EX_64_ZIP ) == 0 ) {
		        if( b64BitOS == TRUE ) {
		            pDlg->AddZip2Dir( strZipPath, strDirPath );
        	        DEBUG_LOG("++     Add to Zip Dialog" );
                } else {
        	        DEBUG_LOG("++     Skip to Zip Dialog" );
                }
            } else if( strZipFileName.CompareNoCase( DRIVE_EX_32_ZIP ) == 0 ) {
                if( b64BitOS == FALSE ) {  
		            pDlg->AddZip2Dir( strZipPath, strDirPath );
        	        DEBUG_LOG("++     Add to Zip Dialog" );
                } else {
        	        DEBUG_LOG("++     Skip to Zip Dialog" );
                }
            } else {
		        // add path
		        pDlg->AddZip2Dir( strZipPath, strDirPath );
        	    DEBUG_LOG("++     Add to Zip Dialog" );
            }
        }
    }
    pMapZip2Dir->RemoveAll(); 
    delete pMapZip2Dir;
    pMapZip2Dir = NULL;
}

void CThirdStepSplash::FillSystemApkVersion()
{
	DEBUG_LOG_Y(">> CThirdStepSplash::FillSystemApkVersion()");

    CDataMgr*               pDataMgr        = (CDataMgr*)GET_DATA_MGR();
    CUpgradeData*           pUpgradeData    = pDataMgr->GetUpgradeData();
    CMapStringToString*     pMapZip2ID      = new CMapStringToString();

#ifdef FEATURE_USE_LOCAL_SYSTEM_APK
    // C:/mobigo/SmartFXAD.apk -> D:/mobigo/SmartFXAD.apk
    CUpgradeItem* pUpgradeItem = pUpgradeData->FindUpgradeItemByID( "system_app4" );
    CString strSmartFXADPath = MOBIGO_ROOT_PATH;
    strSmartFXADPath += "/";
    strSmartFXADPath += "SmartFXAD.apk";
    strSmartFXADPath.Replace( '\\', '/' );
    pUpgradeItem->SetSavePath( strSmartFXADPath );

    // C:/mobigo/MyApp.apk -> D:/mobigo/MyApp.apk
    pUpgradeItem = pUpgradeData->FindUpgradeItemByID( "system_app5" );
    CString strMyAppPath = MOBIGO_ROOT_PATH;
    strMyAppPath += "/";
    strMyAppPath += "MyApp.apk";
    strMyAppPath.Replace( '\\', '/' );
    pUpgradeItem->SetSavePath( strMyAppPath );
#endif

    pUpgradeData->FillSystemApk2IDMap(pMapZip2ID, ITEM_ID_SYSTEM_APK );

    POSITION                iPos            = pMapZip2ID->GetStartPosition();
    CString                 strApk          = ""; // "C:/mobigo/SmartFXAD.apk"
    CString                 strID           = ""; // "system_app4" 
    int                     i               = 0;
    while(iPos != NULL) {
    	DEBUG_LOG_Y("++   [%d]", i++ );

        pMapZip2ID->GetNextAssoc(iPos, strApk, strID);

        // run_SmartFXAD_3094I5JDKF.bat
        CString strBatFilePath = ""; 
        CString strOutFilePath = "";
        {
            CString strRandomKey = CUtil::Random( 10 );

            // bat file name
            strBatFilePath += MOBIGO_TEMP_PATH; 
            strBatFilePath += "/";
            strBatFilePath += "run_";
            strBatFilePath += CUtilFile::ExtractFileName(strApk);
            strBatFilePath += "_";
            strBatFilePath += strRandomKey;
            strBatFilePath += ".bat";
    		strBatFilePath.Replace( '\\', '/' );

    	    DEBUG_LOG_Y("++     strBatFilePath=" + strBatFilePath );

            // out file name
            strOutFilePath += MOBIGO_TEMP_PATH; 
            strOutFilePath += "/";
            strOutFilePath += "run_";
            strOutFilePath += CUtilFile::ExtractFileName(strApk);
            strOutFilePath += "_";
            strOutFilePath += strRandomKey;
            strOutFilePath += ".out";
    		strOutFilePath.Replace( '\\', '/' );

    	    DEBUG_LOG_Y("++     strOutFilePath=" + strOutFilePath );

            // content
            CString strBatContent = "";
            strBatContent += "aapt dump badging ";
            strBatContent += "\"";
            strBatContent += strApk;
            strBatContent += "\"";
            strBatContent += " > ";
            strBatContent += "\"";
            strBatContent += strOutFilePath;
            strBatContent += "\"";

    	    DEBUG_LOG_Y("++     strBatContent=" + strBatContent );

            CUtilFile::WriteString( strBatFilePath, strBatContent );
        }

        CUtilSys::Run( strBatFilePath, NULL, MOBIGO_TEMP_PATH, TRUE, TRUE );
        Sleep( 1000 ); // 파일로 저장되는 약간의 시간을 기달리자.

        CString strOutContent = CUtilFile::ReadString( strOutFilePath );
    	DEBUG_LOG_Y("++     strOutContent=" + strOutContent );

        // extract versionName( ex, versionName='1.0.0' -> "1.0.0" )
        CString strVersionKey("versionName");
        int nStart = strOutContent.Find( strVersionKey );
        if( nStart != -1 ) {
            int nNext = nStart + strVersionKey.GetLength() + 2; // 2 is "='"
            int nEnd  = strOutContent.Find( "'", nNext ); 
            if( nEnd != -1 ) {
                int nCount = nEnd - nNext;
                CString strVersionValue = strOutContent.Mid( nStart + strVersionKey.GetLength() + 2, nCount ); 
    	        DEBUG_LOG_Y("++     strVersionValue=" + strVersionValue );

                CUpgradeItem* pUpgradeItem = pUpgradeData->FindUpgradeItemByID( strID );
                pUpgradeItem->SetApkVersion( strVersionValue );
            }
        }
    }
    pMapZip2ID->RemoveAll(); 
    delete pMapZip2ID;
    pMapZip2ID = NULL;

    // save upgrade list( 저장하지 않을 경우, 계속해서 upgrade 절차를 타게 된다 ) 
	DEBUG_LOG_Y("++   pDataMgr->SaveUpgradeData( \"%s\" )", MOBIGO_UPGRADE_LIST_PATH );
    CString strJson = pDataMgr->ToJson();
	DEBUG_LOG_Y("++   " + strJson );

    pDataMgr->SaveUpgradeData( MOBIGO_UPGRADE_LIST_PATH );
}