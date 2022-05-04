#include "StdAfx.h"
#include "mystringarray.h"

CMyStringArray::CMyStringArray(void)
{
	RemoveAll();
}

CMyStringArray::~CMyStringArray(void)
{
}

CString CMyStringArray::GetAt( int i )
{
	ASSERT( -1 < i && i < m_nCnt );

	return m_ayStr[i];
}

void CMyStringArray::SetAt( int i, CString str )
{
	ASSERT( -1 < i && i < MYSTRINGARRAY_SIZE );

	m_ayStr[i] = str;

	if( m_nCnt <= i )
		m_nCnt = i + 1;
}

void CMyStringArray::Add( CString str )
{
	ASSERT( m_nCnt < MYSTRINGARRAY_SIZE );

	m_ayStr[m_nCnt] = str;

	m_nCnt++;
}

int CMyStringArray::GetCount()
{
	return m_nCnt;
}

void CMyStringArray::RemoveAll()
{
	m_nCnt = 0;

	for( int i = 0; i < MYSTRINGARRAY_SIZE; i++ )
	{
		m_ayStr[i] = "";
	}
}
