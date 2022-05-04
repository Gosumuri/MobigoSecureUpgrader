#pragma once

#include <afxmt.h>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

// Buffer Size
#define		MAX_BUF								256

// Folder( Debug¿ë )
#define		DEFAULT_PATH_APP                 	CString("\\mobigo")
#define		DEFAULT_PATH_LOG					CString("\\log")
#define		DEFAULT_PATH_DEBUG					CString("\\debug")
#define		DEFAULT_PATH_WARNING				CString("\\warning")
#define		DEFAULT_PATH_ERROR					CString("\\error")
#define		DEFAULT_PATH_LIGHT					CString("\\light")
#define		DEFAULT_PATH_SYSAPI					CString("\\sysapi")
#define		DEFAULT_PATH_GSC					CString("\\gsc")
#define		DEFAULT_PATH_INTERFACE       		CString("\\interface")
#define		DEFAULT_PATH_HTML                   CString("\\html")

////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class  CDebug
{
public:
	CDebug(void);
	~CDebug(void);

	// message
	static char				*m_pSzMsg; 

	// count
	static int				m_nCount; 
	static int				m_nCurPatternNum;

    // log file path
    static CString*         m_pstrPathApp;

    // file handler
	static CStdioFile		*m_psfDebug;
	static CStdioFile		*m_psfWarning;
	static CStdioFile		*m_psfError;
	static CStdioFile		*m_psfLight;
	static CStdioFile		*m_psfSysApi;
	static CStdioFile		*m_psfGsc;
	static CStdioFile		*m_psfInterface;
    static CStdioFile		*m_psfHtml;

	// flag
	static BOOL				m_bConsoleLog;
	static BOOL				m_bFileLog;
	static BOOL				m_bPause;

	// critical section
	static CCriticalSection *m_pCS;

	// log root directory 
	static CString			*m_pstrLocalAppDataDir;

	// init/finish
	static void				Init();
	static void				Exit();

	// debug/warning/error
	static void				Log( int nLevel, int nType, int nColor, CString strDebug );
	static void				Log( CString strModuleName, CString strFile, int nLine, int nLevel, int nColor, CString strDebug);
	// debug/warning/error 
	static void				WriteLog(int nLevel, int nType, int nColor, CString strDebug);
	// etc
	static void				ClearScreen();
	static void				Toggle();
	static void				DecreaseThreadCount();
/** get/set */
public:
    static void             SetAppPath(    CString strPathApp    );      
	static void			    SetTitle(      CString strTitle      );
	static BOOL				IsConsoleLog()                              { return m_bConsoleLog;                           }
	static void				SetConsoleLog(BOOL bConsoleLog)             { m_bConsoleLog = bConsoleLog;                    }
	static BOOL				IsFileLog()                                 { return m_bFileLog;                              }
	static void				SetFileLog(BOOL bFileLog)                   { m_bFileLog = bFileLog;                          }
	static void				SetCurPatternNum(int nCurPatternNum)        { m_nCurPatternNum = nCurPatternNum;              }
	static int				GetCurPatternNum()                          { return m_nCurPatternNum;                        }
/* directory */
public :
	static CString			GetAppPath();
    static CString			GetExpdInfoPath();
	static CString			GetLogPath();
	static CString			GetDebugPath();
	static CString			GetWarningPath();
	static CString			GetErrorPath();
	static CString			GetLightPath();
	static CString			GetSysApiPath();
	static CString			GetGscPath();
    static CString          GetInterfacePath();
    static CString          GetHtmlPath();

/* Current File Name */
public :
	static CString			GetDebugFilePath(BOOL bFileClose = FALSE);
	static CString			GetWarningFilePath(BOOL bFileClose = FALSE);
	static CString			GetErrorFilePath(BOOL bFileClose = FALSE);
	static CString			GetLightFilePath(BOOL bFileClose = FALSE);
	static CString			GetGscFilePath(BOOL bFileClose = FALSE);
    static CString          GetInterfacFileePath(BOOL bFileClose = FALSE);
    static CString          GetHtmlFilePath(BOOL bFileClose = FALSE);

private :
    static CString          GetFilePath( CStdioFile *psfFile, BOOL bFileClose = FALSE);
/** utility */
public:
	static CString			Format(CString strFormat, ...);


};

