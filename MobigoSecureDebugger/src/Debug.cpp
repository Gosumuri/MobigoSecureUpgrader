////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

/* 
  log root directory 
    - appdata      --> C:\Users\userID\AppData\Roaming
	- localappdata --> C:\Users\userID\AppData\Local
*/

#define NEW_LOCAL_APP_DATA_DIR() \
    if( m_pstrLocalAppDataDir == NULL ) { \
		char *szAppData = ::getenv(_T("localappdata")); \
		m_pstrLocalAppDataDir = new CString(); \
		m_pstrLocalAppDataDir->SetString(szAppData); \
    } \
    if( m_pstrPathApp == NULL ) { \
        m_pstrPathApp = new CString(); \
        m_pstrPathApp->SetString( DEFAULT_PATH_APP ); \
    }


#define DELETE_LOCAL_APP_DATA_DIR() \
    if( m_pstrLocalAppDataDir != NULL ) { \
		delete m_pstrLocalAppDataDir; \
		m_pstrLocalAppDataDir = NULL; \
    } \
    if( m_pstrPathApp != NULL ) { \
		delete m_pstrPathApp; \
		m_pstrPathApp = NULL; \
    }


////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

// message
char*				CDebug::m_pSzMsg				= (char*)NULL;

// debug
int					CDebug::m_nCount				= 0; 
BOOL				CDebug::m_bConsoleLog			= FALSE;
BOOL				CDebug::m_bFileLog				= FALSE;
BOOL				CDebug::m_bPause				= FALSE;
// pattern num
int					CDebug::m_nCurPatternNum		= -1;

// app path
CString*           	CDebug::m_pstrPathApp           = NULL;

// file 
CStdioFile*			CDebug::m_psfDebug				= NULL;
CStdioFile*			CDebug::m_psfWarning			= NULL;
CStdioFile*			CDebug::m_psfError				= NULL;
CStdioFile*			CDebug::m_psfLight				= NULL;
CStdioFile*			CDebug::m_psfSysApi				= NULL;
CStdioFile*			CDebug::m_psfGsc				= NULL;
CStdioFile*			CDebug::m_psfInterface          = NULL; 
CStdioFile*			CDebug::m_psfHtml               = NULL;

// critical section 
CCriticalSection*	CDebug::m_pCS					= NULL;

// log root directory 
CString*            CDebug::m_pstrLocalAppDataDir	= NULL;

////////////////////////////////////////////////////////////////////////////////
// constructor
////////////////////////////////////////////////////////////////////////////////

CDebug::CDebug(void)
{
}

CDebug::~CDebug(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// Init/Finish function 
////////////////////////////////////////////////////////////////////////////////

void CDebug::Init()
{
	// 0. Init
	{
		NEW_LOCAL_APP_DATA_DIR();

		m_pSzMsg = (char*)NULL;

		// debug
		m_nCount = 0;

		// file 
		m_psfDebug = NULL;
		m_psfWarning = NULL;
		m_psfError = NULL;
		m_psfLight = NULL;

		// critical section 
		m_pCS = new CCriticalSection();
	}

	// 1. message
	m_pSzMsg	= (char*)malloc(MAX_BUF + 1);
	memset( m_pSzMsg, 0, sizeof(char)*( MAX_BUF + 1) );

    SYSTEMTIME st;
    ::GetLocalTime(&st);
        
    CTime      tToday(st);
    CString    strTodayFolder;

    strTodayFolder = tToday.Format("%Y%m%d");

	// 2. log folder 
	if( m_bFileLog == TRUE ) {
		CFileFind filefind;

		// 2.1. "\\mobigo"
		CString strPathApp = CDebug::GetAppPath();
		if (filefind.FindFile(strPathApp) == FALSE) {
			if (::CreateDirectory(strPathApp, NULL) == FALSE){
				::MessageBox(NULL, strPathApp + "를 생성하는 데 실패하였습니다.\r\n" + "(UAC사용을 해지하세요)", "Error", MB_ICONERROR);
			}
		}
        // 2.2. "\\MSecureExpd"
        CString strExpdInfoPath = CDebug::GetExpdInfoPath();
		if (filefind.FindFile(strExpdInfoPath) == FALSE) {
			if (::CreateDirectory(strExpdInfoPath, NULL) == FALSE){
				::MessageBox(NULL, strExpdInfoPath + "를 생성하는 데 실패하였습니다.\r\n" + "(UAC사용을 해지하세요)", "Error", MB_ICONERROR);
			}
		}
		// 2.3. "\\mobigo\\log"
		CString strDirLog = CDebug::GetLogPath();
		if (filefind.FindFile(strDirLog) == FALSE) {
			::CreateDirectory(strDirLog, NULL);
		}

		// 2.4. "\\mobigo\\log\\debug"
		CString strDebugDir = CDebug::GetDebugPath();
		if (filefind.FindFile(strDebugDir) == FALSE) {
			::CreateDirectory(strDebugDir, NULL);
		}
        strDebugDir = strDebugDir +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strDebugDir) == FALSE) {
			::CreateDirectory(strDebugDir, NULL);
		}
		// 2.5. "\\mobigo\\log\\warning"
		CString strWarningDir = CDebug::GetWarningPath();
		if (filefind.FindFile(strWarningDir) == FALSE) {
			::CreateDirectory(strWarningDir, NULL);
		}
        strWarningDir = strWarningDir +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strWarningDir) == FALSE) {
			::CreateDirectory(strWarningDir, NULL);
		}

		// 2.6. "\\mobigo\\log\\error"
		CString strErrorDir = CDebug::GetErrorPath();
		if (filefind.FindFile(strErrorDir) == FALSE) {
			::CreateDirectory(strErrorDir, NULL);
		}
        strErrorDir = strErrorDir +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strErrorDir) == FALSE) {
			::CreateDirectory(strErrorDir, NULL);
		}

		// 2.7. "\\mobigo\\log\\light"
		CString strLightDir = CDebug::GetLightPath();
		if (filefind.FindFile(strLightDir) == FALSE) {
			::CreateDirectory(strLightDir, NULL);
		}
       strLightDir = strLightDir +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strLightDir) == FALSE) {
			::CreateDirectory(strLightDir, NULL);
		}

        // 2.8. "\\mobigo\\log\\interface"
		CString strInterface = CDebug::GetInterfacePath();
		if (filefind.FindFile(strInterface) == FALSE) {
			::CreateDirectory(strInterface, NULL);
		}
        strInterface = strInterface +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strInterface) == FALSE) {
			::CreateDirectory(strInterface, NULL);
		}

        // 2.9. "\\mobigo\\log\\html"
		CString strHtml = CDebug::GetHtmlPath();
		if (filefind.FindFile(strHtml) == FALSE) {
			::CreateDirectory(strHtml, NULL);
		}
        strHtml = strHtml +"\\"+strTodayFolder; // 날짜폴더
        if (filefind.FindFile(strHtml) == FALSE) {
			::CreateDirectory(strHtml, NULL);
		}
	}

	// 3. file stdio handle
	if (m_bFileLog == TRUE) {
		// 3.1. debug stdio file  
		m_psfDebug = new CStdioFile();
		{
			CString strDebugFileName = CDebug::GetDebugPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + DEBUG_LOG_POSTFIX;
			BOOL b = m_psfDebug->Open(strDebugFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE ) {
				delete m_psfDebug;
				m_psfDebug = NULL;
			}
		}
		// 3.2. warning stdio file  
		m_psfWarning = new CStdioFile();
		{
			CString strWarningFileName = CDebug::GetWarningPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + WARNING_LOG_POSTFIX;
			BOOL b = m_psfWarning->Open(strWarningFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfWarning;
				m_psfWarning = NULL;
			}
		}
		// 3.3. error stdio file  
		m_psfError = new CStdioFile();
		{
			CString strErrorFileName = CDebug::GetErrorPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + ERROR_LOG_POSTFIX;
			BOOL b = m_psfError->Open(strErrorFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE ) {
				delete m_psfError;
				m_psfError = NULL;
			}
		}
		// 3.4. light stdio file  
		m_psfLight = new CStdioFile();
		{
			CString strLightFileName = CDebug::GetLightPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + LIGHT_LOG_POSTFIX;
			BOOL b = m_psfLight->Open(strLightFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfLight;
				m_psfLight = NULL;
			}
		}
		// 3.5. sysapi stdio file  
		m_psfSysApi = new CStdioFile();
		{
			CString strSysApiFileName = CDebug::GetSysApiPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + SYSAPI_LOG_POSTFIX;
			BOOL b = m_psfSysApi->Open(strSysApiFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfSysApi;
				m_psfSysApi = NULL;
			}
		}
		// 3.6. gsc stdio file  
		m_psfGsc = new CStdioFile();
		{
			CString strGscFileName = CDebug::GetGscPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + GSC_LOG_POSTFIX;
			BOOL b = m_psfGsc->Open(strGscFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfGsc;
				m_psfGsc = NULL;
			}
		}
		// 3.7. interface stdio file  
		m_psfInterface = new CStdioFile();
		{
			CString strInterfaceFileName = CDebug::GetInterfacePath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + INTERFACE_LOG_POSTFIX;
			BOOL b = m_psfInterface->Open(strInterfaceFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfInterface;
				m_psfInterface = NULL;
			}
		}

        // 3.8. html stdio file  
		m_psfHtml = new CStdioFile();
		{
			CString strHtmlFileName = CDebug::GetHtmlPath() + "\\" + strTodayFolder + "\\" + CUtilTime::GetLocalDateTime() + HTML_LOG_POSTFIX;
			BOOL b = m_psfHtml->Open(strHtmlFileName,
				CFile::modeCreate
				| CFile::typeText
				| CFile::modeWrite
				| CFile::modeNoTruncate
				| CFile::shareDenyNone, // 다른 프로세스에게 파일에 대한 읽기와 쓰기를 허용한다.
				NULL);
			if (b == FALSE) {
				delete m_psfHtml;
				m_psfHtml = NULL;
			}
		}
	}

	// 4. console debug 
	if (m_bConsoleLog == TRUE) {
		CConsoleDebug::Init();
		CConsoleDebug::StartConsoleWin();
	}
}

void CDebug::Exit()
{
    // 다른 thread에 의해서 message가 다 찍히길 0.5sec동안 기다린다.
    m_bPause = TRUE;

#if 0 // 20171011_jkim - App 종료시 죽는 이슈로 인해서 disable 처리
    //Sleep(500);
#endif

	// 1. message
	free( m_pSzMsg );
    m_pSzMsg = (char*)NULL;

	// 2. file
	if ( m_psfDebug != NULL) {
		m_psfDebug->Close();
		delete m_psfDebug;
		m_psfDebug = NULL;
	}
	if (m_psfWarning != NULL) {
		m_psfWarning->Close();
		delete m_psfWarning;
		m_psfWarning = NULL;
	}
	if (m_psfError != NULL) {
		m_psfError->Close();
		delete m_psfError;
		m_psfError = NULL;
	}
	if (m_psfLight != NULL) {
		m_psfLight->Close();
		delete m_psfLight;
		m_psfLight = NULL;
	}
	if (m_psfSysApi != NULL) {
		m_psfSysApi->Close();
		delete m_psfSysApi;
		m_psfSysApi = NULL;
	}
	if (m_psfGsc != NULL) {
		m_psfGsc->Close();
		delete m_psfGsc;
		m_psfGsc = NULL;
	}
    if (m_psfInterface != NULL) {
		m_psfInterface->Close();
		delete m_psfGsc;
		m_psfInterface = NULL;
	}
    if (m_psfHtml != NULL) {
		m_psfHtml->Close();
		delete m_psfHtml;
		m_psfHtml = NULL;
	}
	// 3. console debug 
	if (m_bConsoleLog == TRUE) {
		CConsoleDebug::StopConsoleWin();
		CConsoleDebug::Exit();
	}

	// 4. critical section 
	if (m_pCS != NULL) {
		delete m_pCS;
	}

    // 5. log root directory 
    DELETE_LOCAL_APP_DATA_DIR();
}

////////////////////////////////////////////////////////////////////////////////
// log function
////////////////////////////////////////////////////////////////////////////////

void CDebug::Log( CString strModuleName, CString strFile, int nLine, int nLevel, int nColor, CString strDebug )
{
	if (m_bPause == FALSE ) {
		// Level 
		CString strLevel = "X";
		switch (nLevel) {
		    case LOG_LEVEL_DEBUG:		strLevel = "D";   break;
		    case LOG_LEVEL_WARNING:		strLevel = "W";   break;
		    case LOG_LEVEL_ERROR:		strLevel = "E";   break;
		    case LOG_LEVEL_LIGHT:	    strLevel = "L";   break;
		    case LOG_LEVEL_SYSAPI:	    strLevel = "S";   break;
		    case LOG_LEVEL_GSC:	        strLevel = "G";   break;
		    case LOG_LEVEL_INTERFACE:	strLevel = "i";   break;
            case LOG_LEVEL_HTML:        strLevel = "H";   break;
		}

		// >>, ++, --
		int nType = LOG_TYPE_NONE;
		strDebug.Trim();
		if (strDebug.GetAt(0) == '>' && strDebug.GetAt(1) == '>') nType = LOG_TYPE_FUNCTION;
		else if (strDebug.GetAt(0) == '+' && strDebug.GetAt(1) == '+') nType = LOG_TYPE_VARIABLE;
		else if (strDebug.GetAt(0) == '-' && strDebug.GetAt(1) == '-') nType = LOG_TYPE_CONTROL;

		// Message 
		CString strMsg;
		strFile = CUtilFile::ExtractFileName(strFile);
		strFile = strFile.Mid(0, 20);
		strMsg.Format("%25s [%5d] [%s][%20s:%5d] %s", strModuleName, ::GetCurrentThreadId(), strLevel, strFile, nLine, strDebug);

		CDebug::Log(nLevel, nType, nColor, strMsg);
	}
}

void CDebug::Log(int nLevel, int nType, int nColor, CString strDebug)
{
	if (m_pSzMsg == NULL)
		return;

	CString  strCount = CUtil::PowerChar('0', CUtil2::Int2Str(++m_nCount), 7);
	strDebug = "[" + CUtilTime::GetLocalTime() + ":" + strCount + "] " + strDebug + "\n"; // "\r\n";

	m_pCS->Lock();
	{
		for (int i = 0; i*MAX_BUF < strDebug.GetLength(); i++) {
			_tcscpy((LPTSTR)m_pSzMsg, strDebug.Mid(i*MAX_BUF, MAX_BUF));


			// 1. console log
			if (m_bConsoleLog == TRUE && nLevel != LOG_LEVEL_HTML ) { // HTML  LOG는 콘솔창에서  보여주지 않는다.
				CConsoleDebug::Print(nLevel, nType, nColor, m_pSzMsg);
			}

			// 2. file log
			if (m_bFileLog == TRUE) {
				WriteLog(nLevel, nType, nColor, m_pSzMsg);
			}
		}
	}
	m_pCS->Unlock();
}

////////////////////////////////////////////////////////////////////////////////
// file write
////////////////////////////////////////////////////////////////////////////////

void CDebug::WriteLog(int nLevel, int nType, int nColor, CString strDebug)
{
	CStdioFile *pStdioFile = NULL;

	switch (nLevel) {
		case LOG_LEVEL_WARNING:     pStdioFile = m_psfWarning;      break;
		case LOG_LEVEL_ERROR:       pStdioFile = m_psfError;        break;
		case LOG_LEVEL_LIGHT:       pStdioFile = m_psfLight;        break;
		case LOG_LEVEL_SYSAPI:      pStdioFile = m_psfSysApi;       break;
		case LOG_LEVEL_GSC:         pStdioFile = m_psfGsc;          break;
		case LOG_LEVEL_INTERFACE:   pStdioFile = m_psfInterface;    break;
        case LOG_LEVEL_HTML:        pStdioFile = m_psfHtml;         break;
	}

	if (pStdioFile != NULL) {
		pStdioFile->SeekToEnd();
		pStdioFile->WriteString(strDebug);
	}
    if( nLevel ==  LOG_LEVEL_ERROR) // 에러일 경우에 인터페이스 로그에 남도록.. 추가요청옴.
    {
        m_psfInterface->SeekToEnd();
		m_psfInterface->WriteString(strDebug);
    }
	// debug, warning, error, light에 관계 없이 debug log에 저장한다.  
	if( nLevel !=  LOG_LEVEL_HTML){  // html (web) 쪽 로그는 debug에 남기지 않는다.
        if (m_psfDebug != NULL) {
            if(m_psfDebug->m_pStream != NULL ){
                m_psfDebug->SeekToEnd();
		        m_psfDebug->WriteString(strDebug);
            }
		
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// utility function 
////////////////////////////////////////////////////////////////////////////////

CString CDebug::Format( CString strFormat, ...)
{
	va_list args;
	CString strResult;

	va_start(args, strFormat);
	strResult.FormatV(strFormat, args);
	va_end(args);
	return strResult;
}

void CDebug::ClearScreen()
{
	CConsoleDebug::ClearScreen();
}

void CDebug::Toggle()
{
	m_bPause ^= TRUE;
	m_pCS->Lock();
	{
		Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/******************************************************************************************************************************************************/");
		Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/*                                                                                                                                                    */");
		if (m_bPause == TRUE) {
			Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/*   Pause Log... ...                                                                                                                                 */");
		}
		else {
			Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/*   Resume Log... ...                                                                                                                                */");
		}
		Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/*                                                                                                                                                    */");
		Log(LOG_LEVEL_LIGHT, LOG_TYPE_FUNCTION, LOG_COLOR_DEFAULT, "/******************************************************************************************************************************************************/");
	}
	m_pCS->Unlock();
}

void CDebug::DecreaseThreadCount()
{
	CConsoleDebug::DecreaseThreadCount();
}

////////////////////////////////////////////////////////////////////////////////
// directory function 
////////////////////////////////////////////////////////////////////////////////

void CDebug::SetAppPath( CString strPathApp )
{ 
    if( m_pstrPathApp == NULL ) {
        m_pstrPathApp = new CString();
    }
    m_pstrPathApp->SetString( strPathApp.GetString() );
}

CString CDebug::GetAppPath()
{
    NEW_LOCAL_APP_DATA_DIR();

    static CString* pstrAppPath = new CString();
    pstrAppPath->Format("%s\\%s", *m_pstrLocalAppDataDir, *m_pstrPathApp );
	return *pstrAppPath;
}


CString CDebug::GetExpdInfoPath()
{
    NEW_LOCAL_APP_DATA_DIR();

    static CString* pstrExpdInfoPath = new CString();
    pstrExpdInfoPath->Format("%s\\MsecureExpd", *m_pstrLocalAppDataDir);
	return *pstrExpdInfoPath;
}


CString CDebug::GetLogPath()
{
    static CString* pstrLogPath = new CString();
    pstrLogPath->SetString( GetAppPath() + DEFAULT_PATH_LOG );
	return *pstrLogPath;
}

CString CDebug::GetDebugPath()
{
    static CString* pstrDebugPath = new CString();
    pstrDebugPath->SetString( GetLogPath() + DEFAULT_PATH_DEBUG );
	return *pstrDebugPath;
}

CString CDebug::GetWarningPath()
{
    static CString* pstrWarningPath = new CString();
    pstrWarningPath->SetString( GetLogPath()+ DEFAULT_PATH_WARNING );
	return *pstrWarningPath;
}

CString CDebug::GetErrorPath()
{
    static CString* pstrErrorPath = new CString();
    pstrErrorPath->SetString( GetLogPath() + DEFAULT_PATH_ERROR );
	return *pstrErrorPath;
}

CString CDebug::GetLightPath()
{
    static CString* pstrLightPath = new CString();
    pstrLightPath->SetString( GetLogPath() + DEFAULT_PATH_LIGHT );
	return *pstrLightPath;
}

CString CDebug::GetSysApiPath()
{
    static CString* pstrSysApiPath = new CString();
    pstrSysApiPath->SetString( GetLogPath() + DEFAULT_PATH_SYSAPI );
	return *pstrSysApiPath;
}

CString CDebug::GetGscPath()
{
    static CString* pstrGscPath = new CString();
    pstrGscPath->SetString( GetLogPath() + DEFAULT_PATH_GSC );
	return *pstrGscPath;
}

CString CDebug::GetInterfacePath()
{
    static CString* pstrInterfacePath = new CString();
    pstrInterfacePath->SetString( GetLogPath() + DEFAULT_PATH_INTERFACE );
	return *pstrInterfacePath;
}

CString CDebug::GetHtmlPath()
{
    static CString* pstrHtmlPath = new CString();
    pstrHtmlPath->SetString( GetLogPath() + DEFAULT_PATH_HTML );
	return *pstrHtmlPath;
}
////////////////////////////////////////////////////////////////////////////////
// current file name
////////////////////////////////////////////////////////////////////////////////

CString CDebug::GetDebugFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfDebug,bFileClose);
}
CString CDebug::GetWarningFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfWarning,bFileClose);
}
CString CDebug::GetErrorFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfError,bFileClose);
}
CString CDebug::GetLightFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfLight,bFileClose);
}
CString CDebug::GetGscFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfGsc,bFileClose);
}
CString CDebug::GetInterfacFileePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfInterface,bFileClose);
}
CString CDebug::GetHtmlFilePath(BOOL bFileClose /*= FALSE*/)
{
    return GetFilePath (m_psfHtml,bFileClose);
}
CString CDebug::GetFilePath( CStdioFile *psfFile, BOOL bFileClose /*= FALSE*/)
{
    CString strFilePath = psfFile->GetFilePath();

    if(bFileClose){
        psfFile->Close();
    }
    return strFilePath;
}
////////////////////////////////////////////////////////////////////////////////
// etc
////////////////////////////////////////////////////////////////////////////////

void CDebug::SetTitle( CString strTitle )
{ 
    CConsoleDebug::SetTitle(strTitle);
}