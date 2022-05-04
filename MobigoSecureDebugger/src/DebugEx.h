#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "comutil.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MAX_DEBUG_BUF_SIZE  4096    

// log level
#define LOG_LEVEL_DEBUG      0
#define LOG_LEVEL_WARNING    1
#define LOG_LEVEL_ERROR      2
#define LOG_LEVEL_LIGHT      3
#define LOG_LEVEL_SYSAPI     4
#define LOG_LEVEL_GSC        5
#define LOG_LEVEL_INTERFACE  7
#define LOG_LEVEL_HTML       8

// log type
#define LOG_TYPE_NONE        0
#define LOG_TYPE_FUNCTION    1
#define LOG_TYPE_VARIABLE    2
#define LOG_TYPE_CONTROL     3

// log color
#define LOG_COLOR_DEFAULT   0
#define LOG_COLOR_BLUE      1
#define LOG_COLOR_CYAN      2
#define LOG_COLOR_GREEN     3
#define LOG_COLOR_MAGENTA   4
#define LOG_COLOR_WHITE     5
#define LOG_COLOR_RED       6
#define LOG_COLOR_YELLOW    7

// debug file extention 
#define DEBUG_LOG_POSTFIX        CString("_debug.c")
#define WARNING_LOG_POSTFIX      CString("_warning.c")
#define ERROR_LOG_POSTFIX        CString("_error.c")
#define LIGHT_LOG_POSTFIX        CString("_light.c")
#define SYSAPI_LOG_POSTFIX       CString("_sysapi.c")
#define GSC_LOG_POSTFIX          CString("_gsc.c")
#define INTERFACE_LOG_POSTFIX    CString("_interface.c")
#define HTML_LOG_POSTFIX         CString("_html.c")

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

// utf-8 -> ascii
static char* U2A(const char *pszCode)
{
    BSTR            bstrWide;
    static char        szAnsi[MAX_DEBUG_BUF_SIZE];
    int                nLength;

    memset(szAnsi, 0, sizeof(szAnsi));

    nLength = ::MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, NULL, NULL);
    bstrWide = ::SysAllocStringLen(NULL, nLength);

    ::MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, bstrWide, nLength);

    nLength = ::WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);

    if (nLength < sizeof(szAnsi) ) {
        ::WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, szAnsi, nLength, NULL, NULL);
    } else {
        ::WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, szAnsi, sizeof(szAnsi)-1, NULL, NULL);
    }
    ::SysFreeString(bstrWide);

    return szAnsi;
}

#ifndef GET_MODULE_NAME
#define GET_MODULE_NAME
static CString GetModuleName()
{
    CString strModulePath = "";
    CHAR path[MAX_PATH];
    HMODULE hm;
    if (::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPWSTR)&::GetModuleName, &hm)) {
        ::GetModuleFileName(hm, path, sizeof(path));
        strModulePath = CString(path);
        strModulePath.Replace('/', '\\');

        int nFirstIndex = strModulePath.ReverseFind('\\');
        if (nFirstIndex == -1) {
            return strModulePath;
        }

        return strModulePath.Mid(nFirstIndex + 1);
    }
    return strModulePath;
}
#endif

static CString NormalizeGscMsg(const char *pszMsg )
{
    static CString strNormal;

#if 0 // 일본향과는 다르게 국내향에서는 한글이 안깨진다.
    char* pszBuf = U2A(pszMsg);
    strNormal.SetString(pszBuf);
#else 
    strNormal.SetString(pszMsg);
#endif
    // 문자열에 '%' 있으면 죽는다.
    strNormal.Remove('%');

    return strNormal;
}

static LPCSTR Bstr2Str(BSTR bstr)
{
    static CString s;

    _bstr_t bstrStart(bstr);

    s.Format(_T("%s"), (LPCTSTR)bstrStart);

    return (LPCSTR)s;
}

static LPCSTR WString2Str(const wchar_t *psz)
{
    CComBSTR bstr(psz);
    return Bstr2Str(bstr);
}

#define SYM2STR_CASE_RETURN(arg)                case arg:return #arg
// normalize gsc message 
#define NGM(__msg__)                            NormalizeGscMsg(__msg__)
// boolean -> string
#define B2S(__b__)                              ( __b__ ? "TRUE" : "FALSE" )
// BSTR -> string 
#define BS2S(__bstr__)                          Bstr2Str(__bstr__)
// CString -> string 
#define STR2S(__str__)                          Bstr2Str(((BSTR)__str__.GetString()))
// std:wstring -> string 
#define WS2S(__wstr__)                          WString2Str(__wstr__.c_str())
// pattern num -> string 
#define PN2S(__num__)                           CDebugEx::PatternNum2Str(__num__) 
// operation -> string 
#define OP2S(__operation__)                     CDebugEx::Operation2Str(__operation__) 
// vendor id -> string 
#define VID2S(__vendor_id__)                    CDebugEx::VendorID2Str(__vendor_id__) 

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

#ifdef FEATURE_MOBIGO_DEBUGGER
    #define DEBUG_LOG(     __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define DEBUG_LOG_B(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_BLUE,     __args__, __VA_ARGS__ )
    #define DEBUG_LOG_C(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_CYAN,     __args__, __VA_ARGS__ ) 
    #define DEBUG_LOG_G(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_GREEN,    __args__, __VA_ARGS__ ) 
    #define DEBUG_LOG_M(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_MAGENTA,  __args__, __VA_ARGS__ ) 
    #define DEBUG_LOG_K(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_GREEN,    __args__, __VA_ARGS__ )
    #define DEBUG_LOG_R(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_RED,      __args__, __VA_ARGS__ )
    #define DEBUG_LOG_S(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_MAGENTA,  __args__, __VA_ARGS__ )
	#define DEBUG_LOG_Y(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_YELLOW,   __args__, __VA_ARGS__ )
    #define DEBUG_LOG_W(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,      LOG_COLOR_WHITE,    __args__, __VA_ARGS__ )


    #define WARNING_LOG(   __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_WARNING,    LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define ERROR_LOG(     __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_ERROR,      LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define LIGHT_LOG(     __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_LIGHT,      LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define INTERFACE_LOG( __args__, ... ) CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_INTERFACE,  LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define HTML_LOG( __args__, ... )       CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_HTML,  LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ )
    #define SYSAPI_LOG(    __args__, ... ) \
        { \
            int nCurPatternNum = CDebugEx::GetCurPatternNum(); \
            CString strModuleName; \
            strModuleName.Format( "SysApi(%d)", nCurPatternNum); \
            CDebugEx::LogEx( strModuleName, __FILE__,__LINE__, LOG_LEVEL_SYSAPI,       LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ ); \
        }
    #define GSC_LOG_EX( __args__, ... ) \
        { \
            int nCurPatternNum = CDebugEx::GetCurPatternNum(); \
            CString strModuleName; \
            strModuleName.Format( "GSC(%d)", nCurPatternNum); \
            CDebugEx::LogEx( strModuleName, __FILE__,__LINE__, LOG_LEVEL_GSC,       LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ ); \
        }

    #define GSC_LOG( __file__, __line__, __args__, ... ) \
        { \
            int nCurPatternNum = CDebugEx::GetCurPatternNum(); \
            CString strModuleName; \
            strModuleName.Format( "GSC(%d)", nCurPatternNum); \
            CDebugEx::LogEx( strModuleName, __file__,__line__, LOG_LEVEL_GSC,       LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ ); \
        }

    #define GSC_LOG_SET_CUR_PATTERN_NUM( __pattern_num__ ) \
        { \
            CDebugEx::SetCurPatternNum( __pattern_num__ ); \
        }

    #define DEBUG_CLEAR()                   CDebugEx::ClearScreen()
    #define DEBUG_TOGGLE()                  CDebugEx::Toggle()
    #define DEBUG_DECREASE_THREAD_COUNT()   CDebugEx::DecreaseThreadCount()    
    #define DEBUG_INIT()                    CDebugEx::Init()
    #define DEBUG_EXIT()                    CDebugEx::Exit()
    #define DEBUG_SET_TITLE( __title__ )    CDebugEx::SetTitle(__title__)
    #define DEBUG_SET_CONSOLE_LOG( __b__ )  CDebugEx::SetConsoleLog(__b__)
    #define DEBUG_SET_FILE_LOG( __b__ )     CDebugEx::SetFileLog(__b__)
    #define DEBUG_SET_APP_PATH( __path__ )  CDebugEx::SetAppPath(__path__)
    #define DEBUG_GET_LOG_PATH()            CDebugEx::GetLogPath()
    #define DEBUG_GET_DEBUG_PATH()          CDebugEx::GetDebugPath()
    #define DEBUG_GET_ERROR_PATH()          CDebugEx::GetErrorPath()
    #define DEBUG_GET_WARNING_PATH()        CDebugEx::GetWarningPath()
    #define DEBUG_GET_LIGHT_PATH()          CDebugEx::GetLightPath()

    #define SERVER_LOG(     __args__, ... ) \
    {\
        CDebugEx::LogEx( ::GetModuleName(), __FILE__, __LINE__, LOG_LEVEL_DEBUG,   LOG_COLOR_DEFAULT,  __args__, __VA_ARGS__ );\
        CKernelCallerUtil::KernelPrintLog("test");\
    }
#else 
    #define DEBUG_LOG(    __args__, ...)                    while(0){}
    #define DEBUG_LOG_B(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_C(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_G(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_M(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_K(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_R(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_Y(  __args__, ...)                    while(0){}
    #define DEBUG_LOG_W(  __args__, ...)                    while(0){}

    #define WARNING_LOG(  __args__, ...)                    while(0){}
    #define ERROR_LOG(    __args__, ...)                    while(0){}
    #define LIGHT_LOG(    __args__, ...)                    while(0){}
    #define INTERFACE_LOG(__args__, ...)                    while(0){}
    #define SYSAPI_LOG(   __args__, ...)                    while(0){}
    #define HTML_LOG(   __args__, ...)                      while(0){}

    #define GSC_LOG(    __file__, __line__, __args__, ...)  while(0){}
    #define GSC_LOG_EX( __args__, ...)                      while(0){}
    #define GSC_LOG_SET_CUR_PATTERN_NUM( __pattern_num__ )  while(0){}
    #define DEBUG_CLEAR()                                   while(0){}
    #define DEBUG_TOGGLE()                                  while(0){}
    #define DEBUG_DECREASE_THREAD_COUNT()                   while(0){}
    #define DEBUG_INIT()                                    while(0){}
    #define DEBUG_EXIT()                                    while(0){}
    #define DEBUG_SET_TITLE( __title__ )                    while(0){}
    #define DEBUG_SET_CONSOLE_LOG( __b__ )                  while(0){}
    #define DEBUG_SET_FILE_LOG( __b__ )                     while(0){}
    #define DEBUG_SET_APP_PATH( __path__ )                  while(0){}
    #define DEBUG_GET_LOG_PATH()                            while(0){}
    #define SERVER_LOG(     __args__, ... )                 while(0){}
#endif

////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CDebugEx
{
private : 
    static int          m_nCurPatternNum;
public:
    CDebugEx(void);
    ~CDebugEx(void);
public: /** init/exit */
    static void         Init();
    static void         Exit();
public: /** debug/warning/error */
    static void         LogEx( LPCSTR lpstrModuleName, LPCSTR lpstrFile, int nLine, int nLevel, int nColor, LPCSTR  lpstrFormat, ...);
    static void         ClearScreen();
    static void         Toggle();
    static void         DecreaseThreadCount();
public: /** get/set */
    static void         SetAppPath( LPCSTR strPathRoot );
    static LPCSTR       GetLogPath();
    static LPCSTR       GetDebugPath();
    static LPCSTR       GetWarningPath();
    static LPCSTR       GetErrorPath();
    static LPCSTR       GetLightPath();
    static LPCSTR       GetInterfacePath();
    static LPCSTR       GetHtmlPath();
    static LPCSTR       GetExpdInfoPath();

    static CString		GetDebugFilePath(BOOL bFileClose = FALSE);
	static CString		GetWarningFilePath(BOOL bFileClose = FALSE);
	static CString		GetErrorFilePath(BOOL bFileClose = FALSE);
	static CString		GetLightFilePath(BOOL bFileClose = FALSE);
	static CString		GetGscFilePath(BOOL bFileClose = FALSE);
    static CString      GetInterfacFileePath(BOOL bFileClose = FALSE);
    static CString      GetHtmlFilePath(BOOL bFileClose = FALSE);

    static void         SetTitle( LPCSTR lpstrTitle );
    static void         SetConsoleLog( BOOL b );
    static void         SetFileLog( BOOL b );
    static void         SetCurPatternNum(int nPatternNum);
    static int          GetCurPatternNum();

public: /** utility */
    static LPCSTR       PatternNum2Str(int nPatternNum);
    static LPCSTR       Operation2Str(int nOperation);
    static LPCSTR       VendorID2Str(int nVendorID);

    static CString      ChangeFileName(  CString strFilePath, CString strFilePrefix, CString strFileSuffix );
};

