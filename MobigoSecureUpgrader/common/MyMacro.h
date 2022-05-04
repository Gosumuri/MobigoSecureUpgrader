#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// extern
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

// post/send
#define POST_UI_THREAD(          __msg__, __wparam__, __lparam__            )   CMyMacro::PostUiThread(           __msg__, __wparam__, __lparam__ )
#define SEND_UI_THREAD(          __msg__, __wparam__, __lparam__            )   CMyMacro::SendUiThread(           __msg__, __wparam__, __lparam__ )
#define POST_UI_THREAD_DELAY(    __msg__, __wparam__, __lparam__, __delay__ )   CMyMacro::PostUiThreadDelay(      __msg__, __wparam__, __lparam__, __delay__ )
#define QUIT_APP(                __exit_code__ )                                CMyMacro::QuitApp(                __exit_code__ )

// get/set
#define GET_UI_CONTROLLER()                                                     CMyMacro::GetUiController()
#define SET_UI_CONTROLLER( __p__ )                                              CMyMacro::SetUiController( __p__ )

#define GET_DATA_CONTROLLER()                                                   CMyMacro::GetDataController()
#define SET_DATA_CONTROLLER( __p__ )                                            CMyMacro::SetDataController( __p__ )
#define GET_DATA_MGR()                                                          CMyMacro::GetDataMgr()
#define SET_DATA_MGR( __p__ )                                                   CMyMacro::SetDataMgr( __p__ )

#define SET_MSG_WND( __p__ )                                                    CMyMacro::SetMsgWnd( __p__ )

#define SET_MAIN_FRAME( __main_frame__ )                                        CMyMacro::SetMainFrame( __main_frame__ )
#define GET_MAIN_FRAME()                                                        CMyMacro::GetMainFrame()

// message
#ifdef _DEBUG
    #define MESSAGE_LOG( __str__ )                                              CMyMacro::MessageLog( __str__ )
    #define PROGRESS_START( __str__, __pos__ )                                  CUtilProg::ProgressStart( __str__, __pos__ ) 
    #define PROGRESS_STEP( __pos__ )                                            CUtilProg::ProgressStep(  __pos__ )
    #define PROGRESS_END()                                                      CUtilProg::ProgressEnd()
#else 
    #define MESSAGE_LOG( __str__ )                                              while(0){}
    #define PROGRESS_START( __str__, __pos__ )                                  while(0){}
    #define PROGRESS_STEP( __pos__ )                                            while(0){}
    #define PROGRESS_END()                                                      while(0){}
#endif

#define DELETE_MY_OBJECT( __object__ ) \
    {                                  \
        if( __object__ != NULL ) {     \
            __object__->Exit();        \
            delete __object__;         \
            __object__ = NULL;         \
        }                              \
    }

#define IS_ON_BETA_SERVER()                                                     CMyMacro::IsOnBetaServer()
#define IS_ON_DEBUG()                                                           CMyMacro::IsOnDebug()
#define IS_ON_ZOOM_BY_WHEEL()                                                   CMyMacro::IsOnZoomByWheel()
#ifdef _DEBUG
    #define GET_CURRENT_SERVER_URL()                                            CString("http://secure.mobigo.co.kr")//CMyMacro::GetCurrentServerUrl()
#else
    #define GET_CURRENT_SERVER_URL()                                            CMyMacro::GetCurrentServerUrl()
#endif
#define GET_REAL_SERVER_URL()                                                   CMyMacro::GetRealServerUrl();
#define GET_BETA_SERVER_URL()                                                   CMyMacro::GetBetaServerUrl();

////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CMyMacro
{
public:
	CMyMacro(void);
	~CMyMacro(void);

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////
private :
    static CWnd     *m_pMainFrame;
    // for Kernel Thread
    static CWnd*    m_pUiController;
    static CWnd*    m_pDataController;
    static void*    m_pDataMgr;

    // for MESSAGE_LOG
    static HWND     m_hWndMsg;
	static char*	m_pSzMsg; 
	static CString* m_pStrMsg;
	static int		m_nCount; 
////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

//get/set
public : 
    static void         SetMainFrame(      CWnd *pMainFrame )           { m_pMainFrame      = pMainFrame;      }
    static CWnd*        GetMainFrame()                                  { return m_pMainFrame;                 }
    static void*        GetUiController()                               { return m_pUiController;              }
    static void         SetUiController( CWnd *pUiController )          { m_pUiController = pUiController;     }

    static void*        GetDataController()                             { return m_pDataController;            }
    static void         SetDataController( CWnd *pDataController )      { m_pDataController = pDataController; }
    static void*        GetDataMgr()                                    { return m_pDataMgr;                   }
    static void         SetDataMgr( void* pDataMgr )                    { m_pDataMgr = pDataMgr;               }

    static void         SetMsgWnd( HWND hWndMsg  )                      { m_hWndMsg = hWndMsg;                 }

//life cycle 
public : 
    static void Init();
    static void Exit();

// utility
public : 
    static void                             PostUiThread(        int nMsg, WPARAM wParam, LPARAM lParam );
    static void                             SendUiThread(        int nMsg, WPARAM wParam, LPARAM lParam );
    static void                             PostUiThreadDelay(   int nMsg, WPARAM wParam, LPARAM lParam, DWORD dwDelay);
    static void                             QuitApp(             int nExitCode );

    static void                             MessageLog( CString strMsg );

// config
public :
    static BOOL                             IsOnBetaServer();
    static BOOL                             IsOnDebug();
    static BOOL                             IsOnZoomByWheel();

    static CString                          GetCurrentServerUrl();
    static CString                          GetRealServerUrl();
    static CString                          GetBetaServerUrl();
};
