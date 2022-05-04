#pragma once


// CLogEdit
// debug message
#define		MY_MSG_DEBUG						( WM_USER + 100 )
#define		MY_MSG_WARNING						( WM_USER + 101 )
#define		MY_MSG_ERROR						( WM_USER + 102 )
#define		MY_MSG_WATCH						( WM_USER + 103 )

class CLogEdit : public CEdit
{
	DECLARE_DYNAMIC(CLogEdit)

public:
	CLogEdit();
	virtual ~CLogEdit();

protected:
	DECLARE_MESSAGE_MAP()

	void			AddString( CString lParamString );
	afx_msg LRESULT OnMsgDebugHandler(	 WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgWarningHandler( WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgErrorHandler(	 WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgWatchHandler(	 WPARAM wParam, LPARAM lParam ); 
};


