////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UiController.h"
#include "UiTddController.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUiTddController::CUiTddController()
    : CUiController()
{
	DEBUG_LOG(">> CUiTddController::CUiTddController()");
}

CUiTddController::~CUiTddController()
{
	DEBUG_LOG(">> CUiTddController::~CUiTddController()");
}

////////////////////////////////////////////////////////////////////////////////
// message map / data exchange 
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CUiTddController, CUiController)

BEGIN_MESSAGE_MAP(CUiTddController, CUiController)
	ON_MESSAGE( MY_MSG_U_TDD,                   OnTdd                   )
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// life cycle 
////////////////////////////////////////////////////////////////////////////////

void	CUiTddController::Init()
{
	DEBUG_LOG(">> CUiTddController::Init()");

    CUiController::Init();
}

void	CUiTddController::Exit()
{
	DEBUG_LOG(">> CUiTddController::Exit()");

    CUiController::Exit();
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////


LRESULT CUiTddController::OnTdd(WPARAM wParam, LPARAM lParam)
{
	DEBUG_LOG(">> CUiTddController::OnTdd()");

    CDataMgr    *pDataMgr   = (CDataMgr*)GET_DATA_MGR();
    CString     strTdd      = CString( (LPCSTR)wParam );
	DEBUG_LOG("++   strTdd=" + strTdd );

    if(        strTdd == "TDD_UPGRADE_LIST" ) {
        DEBUG_LOG("++   URL_UPGRADE_LIST_JSON=%s", URL_UPGRADE_LIST_JSON );

        CString strUpgradeListJson = CUtilHttpGet::Request( URL_UPGRADE_LIST_JSON );
    	DEBUG_LOG("++   strUpgradeListJson=" + strUpgradeListJson );

        CString strHtmlSourcePath(MOBIGO_TEMP_PATH);
        strHtmlSourcePath += CString("\\") + CString("upgrade_list_json_") + CUtilTime::GetLocalDateTime() + ".json";
        CUtilFile::WriteString( strHtmlSourcePath, strUpgradeListJson );
        CUtilSys::OpenFileByEditor( strHtmlSourcePath );
    } else if(        strTdd == "TDD_UNZIP_BY_UTIL" ) {
        DEBUG_LOG("++   TDD_UNZIP_BY_UTIL" );

        CString strZipPath = "C:\\mobigo\\download\\device_driver_pack\\kernel\\Kernel_DrvPack.zip";
        CString strDirPath = "C:\\mobigo\\download\\device_driver_pack\\kernel\\DriverEx";

	    DEBUG_LOG("++   strZipPath=" + strZipPath );
	    DEBUG_LOG("++   strDirPath=" + strDirPath );

        CUtilZip::Zip2Dir( strZipPath, strDirPath, NULL );
    } else if(        strTdd == "TDD_SHOW_SPLASH" ) {
        DEBUG_LOG("++   TDD_SHOW_SPLASH" );
        m_uiReleaseView.ShowSplash( &m_uiReleaseView, 0, 4 );

        // 1. "버전 확인 중... ..."
        {
		    m_uiReleaseView.GetSplash()->SetMainText("버전 확인 중... ... ... ");
            m_uiReleaseView.GetSplash()->SetMainPercent("25%");
		    m_uiReleaseView.GetSplash()->StepIt();
            ::Sleep(1000);
        }

        // 2. "버전 업데이트 중... ..."
        {
		    m_uiReleaseView.GetSplash()->SetMainText("버전 업데이트 중... ... ... ");
            m_uiReleaseView.GetSplash()->SetMainPercent("50%");
		    m_uiReleaseView.GetSplash()->StepIt();
            ::Sleep(1000);
        }

        // 3. "드라이버 업데이트 중... ..."
        {
		    m_uiReleaseView.GetSplash()->SetMainText("드라이버 업데이트 중... ... ... ");
            m_uiReleaseView.GetSplash()->SetMainPercent("75%");
		    m_uiReleaseView.GetSplash()->StepIt();
            ::Sleep(1000);
        }

        // 4. "업데이트 완료... ..."
        {
		    m_uiReleaseView.GetSplash()->SetMainText("업데이트 완료... ... ... ");
            m_uiReleaseView.GetSplash()->SetMainPercent("100%");
		    m_uiReleaseView.GetSplash()->StepIt();
            ::Sleep(1000);
        }

        m_uiReleaseView.GetSplash()->EndDialog(IDOK);
        m_uiReleaseView.EndDialog(IDOK);  
    } else if(        strTdd == "ID_FUNC_SYSTEM_ALERT" ) {
        DEBUG_LOG("++   ID_FUNC_SYSTEM_ALERT" );
        int nOK = CUtilMessageBox::Alert( "", "", "" );
    } else if(        strTdd == "ID_FUNC_SYSTEM_CONFIRM" ) {
        DEBUG_LOG("++   ID_FUNC_SYSTEM_CONFIRM" );
        int nConfirm = CUtilMessageBox::Confirm( "", "", "", "" );
    } else if(        strTdd == "ID_FUNC_HTML_ALERT" ) {
        DEBUG_LOG("++   ID_FUNC_HTML_ALERT" );
        int nOK = ALERT( "alert_message(한글)", "alert_title(한글)" );
    } else if(        strTdd == "ID_FUNC_HTML_CONFIRM" ) {
        DEBUG_LOG("++   ID_FUNC_HTML_CONFIRM" );
        int nOK = CONFIRM( "confirm_message(한글)", "confirm_title(한글)" );
    } else if(        strTdd == "ID_SERVICE_INSTALL" ) {
        DEBUG_LOG("++   ID_SERVICE_INSTALL" );
        GET_MAIN_FRAME()->SendMessage(WM_COMMAND,MAKELONG(ID_SERVICE_INSTALL,0),NULL);
    } else if(        strTdd == "ID_SERVICE_START" ) {
        DEBUG_LOG("++   ID_SERVICE_START" );
        GET_MAIN_FRAME()->SendMessage(WM_COMMAND,MAKELONG(ID_SERVICE_START,0),NULL);
    } else if(        strTdd == "ID_SERVICE_STOP" ) {
        DEBUG_LOG("++   ID_SERVICE_STOP" );
        GET_MAIN_FRAME()->SendMessage(WM_COMMAND,MAKELONG(ID_SERVICE_STOP,0),NULL);
    } else if(        strTdd == "ID_SERVICE_UNINSTALL" ) {
        DEBUG_LOG("++   ID_SERVICE_UNINSTALL" );
        GET_MAIN_FRAME()->SendMessage(WM_COMMAND,MAKELONG(ID_SERVICE_UNINSTALL,0),NULL);
    }

	return TRUE;
}
