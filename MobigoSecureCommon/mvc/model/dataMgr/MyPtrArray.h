#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MYPTRARRAY_INIT_SIZE		10
#define MYPTRARRAY_GROW_BY			10

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CMyPtrArray : public CPtrArray
{
public:
	CMyPtrArray();
	virtual ~CMyPtrArray();
public:
	void			RemoveAt(  int n );
	int				GetCountWidthoutNull();
	void			SetAt( int nIndex, void* vp );
};

