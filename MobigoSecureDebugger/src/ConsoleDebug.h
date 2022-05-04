#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include <afxmt.h>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define     CONSOLE_WINDOW_NAME         "Mobigo Secure Debugger"

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class  CConsoleDebug
{
public:
	CConsoleDebug();
	virtual ~CConsoleDebug();

public: /** Init/Exit */
	static void             Init();
	static void             Exit();
public:
	// Console window의 폭과 높이를 조절할 수 있다. 
	// File로 Debug 정보가 출력되기를 원하다면,  fname에 file pointer를 설정하여라.
	// Console window는 Application이 종료하면 같이 종료한다.
	static void		    StartConsoleWin( int width=120, int height=25, char* fname = NULL);
	static void		StopConsoleWin();
	// 사용 방법은 TRACE0, TRACE1... 와 비슷하다. 
	// arguments는 printf와 같다.
	static int	 	        Print(					int nLevel, int nType, int nColor, CString strLog );
	static int	 	        Printf(					int nLevel, int nType, int nColor, char *fmt, ...);
	static void		        SetColor(				int nLevel, int nType, int nColor);
	static void		        SetColorDebug(			int nLevel, int nType, int nColor);
	static void		        SetColorDebugEachThread(int nLevel, int nType, int nColor);
	static void		        SetColorWarning(		int nLevel, int nType, int nColor);
	static void		        SetColorError(			int nLevel, int nType, int nColor);
	static void             SetColorLight(			int nLevel, int nType, int nColor);
	static void             SetColorSysApi(         int nLevel, int nType, int nColor);
	static void             SetColorGsc(			int nLevel, int nType, int nColor);
	static void             SetColorInterface(		int nLevel, int nType, int nColor);

    static void             SetTitle(               CString strTitle );
public: /** utility */
	static WORD             NextThreadColor();
	static void             ClearScreen();
	static void		        DecreaseThreadCount();
    static LPCSTR           GetConsoleWindowName();

private : /** maximize secondary window */
    static BOOL CALLBACK    MonitorEnumProc(    HMONITOR    hMonitor,       // handle to display monitor
                                                HDC         hdc1,           // handle to monitor DC
                                                LPRECT      lprcMonitor,    // monitor intersection rectangle
                                                LPARAM      data);          // data
    static void             MaximizeSecondaryMonitor(); 

private :
	static HANDLE							m_hStdOut;
	static CMap<int, int, WORD, WORD>		*m_pMap;
	static int								m_nThreadColorIndex;
	static CCriticalSection					*m_pCS;
    static CRect                            m_rectSecondaryMonitor;
    static CString*                         m_pstrTitle;    

	enum {
		CONSOLE_BLACK		= 0,
		CONSOLE_GRAY		= FOREGROUND_INTENSITY,

		CONSOLE_GREEN       = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CONSOLE_GREEN_DIM   = FOREGROUND_GREEN,

		CONSOLE_BLUE		= FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		CONSOLE_BLUE_DIM    =                        FOREGROUND_BLUE,

		CONSOLE_RED         = FOREGROUND_INTENSITY | FOREGROUND_RED,
		CONSOLE_RED_DIM     = FOREGROUND_RED,

		CONSOLE_CYAN		= FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CONSOLE_CYAN_DIM    =                        FOREGROUND_GREEN | FOREGROUND_BLUE,

		CONSOLE_MAGENTA		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		CONSOLE_MAGENTA_DIM =                        FOREGROUND_RED | FOREGROUND_BLUE,

		CONSOLE_YELLOW		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		CONSOLE_YELLOW_DIM  =                        FOREGROUND_RED | FOREGROUND_GREEN,

		CONSOLE_WHITE		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CONSOLE_WHITE_DIM   =                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

};


