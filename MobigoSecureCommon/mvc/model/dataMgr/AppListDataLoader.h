#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "AppListData.h"
#include "AppItem.h"

#include "JsonBox.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CAppListDataLoader
{
public:
	CAppListDataLoader();   // protected constructor used by dynamic creation
	virtual ~CAppListDataLoader();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private:

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:

// load
public:

    CAppListData*       Load( CString strUrl );
    CAppListData*       Parse( CString strAppListJson );
    CAppItem*           ParseItem( CString strTab, JsonBox::Object joItem );
};