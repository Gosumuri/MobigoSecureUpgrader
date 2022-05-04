#pragma once

CString operator+( char*, CString );

class AFX_EXT_CLASS CUtil
{
public:
	CUtil(void);
	~CUtil(void);

// etc
public:
	static  CString			Random( int nLength ) ;
	static  CString			Normal( CString );
	static  int				Round( float f );
	static  CString			RemoveComment( CString str );
	static  CString			NewDocName( CString str );
	static  CString			PowerChar( char ch, CString strPost, int nPower );
// Thread
public:
	static void				TerminateThread( HANDLE hThread );
};


