#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MYSTRINGARRAY_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CMyStringArray
{
public:
	CMyStringArray(void);
	~CMyStringArray(void);
private :
	CString		m_ayStr[ MYSTRINGARRAY_SIZE ];
	int			m_nCnt;
public:
	CString		GetAt( int );
	void		SetAt( int, CString );
	void		Add( CString str );
	int			GetCount();
	void		RemoveAll();
};
