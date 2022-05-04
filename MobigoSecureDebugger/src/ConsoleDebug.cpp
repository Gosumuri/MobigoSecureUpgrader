// Debug.cpp: implementation of the CDebug class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConsoleDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

HANDLE							CConsoleDebug::m_hStdOut			= NULL;
CMap<int, int, WORD, WORD>*     CConsoleDebug::m_pMap				= NULL;
int								CConsoleDebug::m_nThreadColorIndex	= -1;
CCriticalSection*               CConsoleDebug::m_pCS				= NULL;
CRect                           CConsoleDebug::m_rectSecondaryMonitor( 0, 0, 0, 0);
CString*                        CConsoleDebug::m_pstrTitle          = NULL;

////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////////////////

CConsoleDebug::CConsoleDebug()
{
}

CConsoleDebug::~CConsoleDebug()
{
}

////////////////////////////////////////////////////////////////////////////////
// Init/Exit
////////////////////////////////////////////////////////////////////////////////

void CConsoleDebug::Init()
{
	m_hStdOut = NULL;
	m_pMap = new CMap<int, int, WORD, WORD>;
	m_nThreadColorIndex = -1;
	m_pCS = new CCriticalSection();
}

void CConsoleDebug::Exit()
{
	if (m_pMap != NULL) {
		delete m_pMap;
	}

	if (m_pCS != NULL) {
		delete m_pCS;
	}
}

////////////////////////////////////////////////////////////////////////////////
// public function 
////////////////////////////////////////////////////////////////////////////////

void CConsoleDebug::StartConsoleWin( int nWidth, int nHeight, char* fname)
{
	::AllocConsole();
    if( m_pstrTitle == NULL ) {	
	    ::SetConsoleTitle( CONSOLE_WINDOW_NAME );
    } else {
        ::SetConsoleTitle( *m_pstrTitle );
    }
	m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD co = { nWidth, nHeight };
	::SetConsoleScreenBufferSize(m_hStdOut, co);

    Sleep(500);
    MaximizeSecondaryMonitor();
}

void CConsoleDebug::StopConsoleWin()
{
	FreeConsole();
	m_hStdOut = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// print function 
////////////////////////////////////////////////////////////////////////////////

int CConsoleDebug::Print(int nLevel, int nType, int nColor, CString strDebug )
{
	DWORD cCharsWritten;

	if (m_hStdOut != NULL) {
		SetColor(nLevel, nType, nColor);
		::WriteConsole(m_hStdOut, strDebug, strDebug.GetLength(), &cCharsWritten, NULL);
	}

	return strDebug.GetLength();
}

// Use dprintf like TRACE0, TRACE1, ... (The arguments are the same as printf)
int CConsoleDebug::Printf( int nLevel, int nType, int nColor, char *fmt, ...)
{
	static char s[MAX_DEBUG_BUF_SIZE];
	va_list argptr;
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);

	DWORD cCharsWritten;

	if ( m_hStdOut != NULL ) {
		SetColor(nLevel, nType, nColor);
		::WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL);
	}

	return(cnt);
}

////////////////////////////////////////////////////////////////////////////////
// color function 
////////////////////////////////////////////////////////////////////////////////

void CConsoleDebug::SetColor(int nLevel, int nType, int nColor )
{
	switch (nLevel) {
		case LOG_LEVEL_DEBUG:   {
			if ( nColor == LOG_COLOR_DEFAULT ) {
				SetColorDebugEachThread(nLevel, nType, nColor);
			}
			else {
				SetColorDebug(nLevel, nType, nColor);
			}
		}
   	    break;
		case LOG_LEVEL_WARNING:     SetColorWarning(    nLevel,   nType, nColor); break;
		case LOG_LEVEL_ERROR:       SetColorError(      nLevel,   nType, nColor); break;
		case LOG_LEVEL_LIGHT:       SetColorLight(      nLevel,   nType, nColor); break;
		case LOG_LEVEL_SYSAPI:      SetColorSysApi(     nLevel,   nType, nColor); break;
		case LOG_LEVEL_GSC:         SetColorGsc(        nLevel,   nType, nColor); break;
		case LOG_LEVEL_INTERFACE:   SetColorInterface(  nLevel,   nType, nColor); break;
	}
}

WORD CConsoleDebug::NextThreadColor()
{
	WORD wColor = CONSOLE_RED;
	m_pCS->Lock();
	{
		m_nThreadColorIndex++;
		switch (m_nThreadColorIndex ) {
			case 0: wColor = CONSOLE_WHITE;   break;
			case 1: wColor = CONSOLE_MAGENTA; break;
			case 2: wColor = CONSOLE_CYAN;    break;
			case 3: wColor = CONSOLE_GREEN;   break;
			case 4: wColor = CONSOLE_BLUE;    break;
		}
	}
	m_pCS->Unlock();

	return wColor;
}

void CConsoleDebug::SetColorDebugEachThread(int nLevel, int nType, int nColor)
{
	WORD wColor = CONSOLE_RED;
	BOOL b = m_pMap->Lookup(::GetCurrentThreadId(), wColor );
	if (b == FALSE ) {
		wColor = NextThreadColor();
		m_pMap->SetAt(::GetCurrentThreadId(), wColor);
	}

	switch (wColor) {
		case CONSOLE_WHITE: {
			// type
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_WHITE);      break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GRAY);       break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GRAY);       break;
			}
		}
		break;
		case CONSOLE_BLUE: {
			// type
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_BLUE);      break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_BLUE_DIM);  break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_BLUE_DIM);  break;
			}
		}
		break;
		case CONSOLE_CYAN: {
			// type
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_CYAN);      break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_CYAN_DIM);  break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_CYAN_DIM);  break;
			}
		}
		break;
		case CONSOLE_GREEN: {
			// type
			switch (nType) {
			case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GREEN);      break;
			case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GREEN_DIM);  break;
			case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GREEN_DIM);  break;
			}
		}
		break;
		case CONSOLE_MAGENTA: {
			// type
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_MAGENTA);      break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_MAGENTA_DIM);  break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_MAGENTA_DIM);  break;
			}
		}
	    break;
		default: {
			// type
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_RED);      break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_RED_DIM);  break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_RED_DIM);  break;
			}
		}
	}
}

void CConsoleDebug::SetColorDebug(int nLevel, int nType, int nColor)
{
	switch (nColor) {
		case LOG_COLOR_DEFAULT : 
			switch (nType) {
				case LOG_TYPE_FUNCTION: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_WHITE); break;
				case LOG_TYPE_VARIABLE: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GRAY);  break;
				case LOG_TYPE_CONTROL:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GRAY);  break;
			}
			break;
		case LOG_COLOR_BLUE:    ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_BLUE);    break;
		case LOG_COLOR_CYAN:    ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_CYAN);    break;
		case LOG_COLOR_GREEN:   ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_GREEN);   break;
		case LOG_COLOR_MAGENTA: ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_MAGENTA); break;
		case LOG_COLOR_WHITE:   ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_WHITE);   break;
		case LOG_COLOR_RED:     ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_RED);     break;
		case LOG_COLOR_YELLOW:  ::SetConsoleTextAttribute(m_hStdOut, CONSOLE_YELLOW);  break;
	}
}

void CConsoleDebug::SetColorWarning(int nLevel, int nType, int nColor)
{
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_YELLOW);
}

void CConsoleDebug::SetColorError(int nLevel, int nType, int nColor)
{
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_RED);
}

void CConsoleDebug::SetColorLight(int nLevel, int nType, int nColor)
{
	// black on yellow
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_BLACK | BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED);
}

void CConsoleDebug::SetColorSysApi(int nLevel, int nType, int nColor)
{
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_YELLOW_DIM);
}

void CConsoleDebug::SetColorGsc(int nLevel, int nType, int nColor)
{
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_YELLOW_DIM);
}

void CConsoleDebug::SetColorInterface(int nLevel, int nType, int nColor)
{
	::SetConsoleTextAttribute(m_hStdOut, CONSOLE_MAGENTA_DIM );
}

////////////////////////////////////////////////////////////////////////////////
// utility function 
////////////////////////////////////////////////////////////////////////////////

void CConsoleDebug::ClearScreen() 
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(console, topLeft);
}

void CConsoleDebug::DecreaseThreadCount()
{
	m_pCS->Lock();
	{
		m_nThreadColorIndex--;
	}
	m_pCS->Unlock();
}

////////////////////////////////////////////////////////////////////////////////
// maximize secondary monitor 
////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CConsoleDebug::MonitorEnumProc(   HMONITOR    hMonitor,       // handle to display monitor
                                                HDC         hdc1,           // handle to monitor DC
                                                LPRECT      lprcMonitor,    // monitor intersection rectangle
                                                LPARAM      data)           // data
{
    RECT rc = *lprcMonitor;

    MONITORINFOEX mInfo;
    mInfo.cbSize = sizeof(mInfo);
    ::GetMonitorInfo(hMonitor, &mInfo);

    if (mInfo.dwFlags != MONITORINFOF_PRIMARY)
    {
        // for purposes of example we always take the first secondary monitor 
        // we find.  I leave it as an exercise for the programmer to change 
        // the code to support multiple secondary displays (and therefore 
        // pick the desired monitor
        m_rectSecondaryMonitor = mInfo.rcWork;
        // return FALSE to stop the enumeration
        return FALSE;
    }
    // if we get here, the current monitor isn't a secondary monitor
    return TRUE;
}

void CConsoleDebug::MaximizeSecondaryMonitor() 
{
    // 1. init the memebr rectangle
    m_rectSecondaryMonitor.SetRect(0,0,0,0);
    
    // 2. enumerate monitor 
    ::EnumDisplayMonitors(NULL, NULL, CConsoleDebug::MonitorEnumProc, 0);

    // 3. console debug window를 second monitor에 maiximize시킨다. 
    HWND hWndDebug  = NULL; 
    if( m_pstrTitle == NULL ) {	
	    hWndDebug = ::FindWindow( NULL, CONSOLE_WINDOW_NAME );
    } else {
	    hWndDebug = ::FindWindow( NULL, *m_pstrTitle );
    }

    LONG lStyles    = ::GetWindowLong(hWndDebug, GWL_STYLE);
    ::SetWindowLong(hWndDebug, GWL_STYLE, lStyles | WS_MAXIMIZE);

    // 4. if we don't have a secondary monitor, go ahead and maximize the window
    if (m_rectSecondaryMonitor.Width() + m_rectSecondaryMonitor.Height() == 0) {
        ::ShowWindow(hWndDebug, SW_MAXIMIZE );
    } else {
        ::MoveWindow(   hWndDebug, 
                        m_rectSecondaryMonitor.left, 
                        m_rectSecondaryMonitor.top, 
                        m_rectSecondaryMonitor.Width(), 
                        m_rectSecondaryMonitor.Height(), 
                        TRUE);
    }
}

////////////////////////////////////////////////////////////////////////////////
// etc function 
////////////////////////////////////////////////////////////////////////////////

void CConsoleDebug::SetTitle( CString strTitle )
{
    if( m_pstrTitle == NULL ) {
        m_pstrTitle = new CString();
    }
    m_pstrTitle->Format( "%s", strTitle );
	::SetConsoleTitle( *m_pstrTitle );
}
