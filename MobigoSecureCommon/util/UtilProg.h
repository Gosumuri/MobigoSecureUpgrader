#pragma once

// progress
#define		MY_MSG_PROGRESS_START				( WM_USER + 1 )
#define		MY_MSG_PROGRESS_STEP				( WM_USER + 2 )
#define		MY_MSG_PROGRESS_END					( WM_USER + 3 )

class AFX_EXT_CLASS CUtilProg
{
public:
	CUtilProg(void);
	~CUtilProg(void);

	// window handle
	static char		*m_pSzMsg; 
	static HWND		m_hWndMainFrm; 

	static void		Init( HWND hWnd );
	static void		Exit();
	static void		ProgressStart( CString strLabel, int nTotal );
	static void		ProgressStep( int nPos );
	static void		ProgressEnd();

};
