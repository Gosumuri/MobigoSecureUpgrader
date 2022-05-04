#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "MyPtrArray.h"
#include "PolicyItem.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CPolicyData
{
public:
	CPolicyData();   // protected constructor used by dynamic creation
	virtual ~CPolicyData();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private:
    CString        m_strID;
    CString        m_strMakeTime; 
    CString        m_strVersion; 
    CMyPtrArray*   m_paPolicyItem;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
    CString         GetID()                                          { return m_strID;                  };
    void            SetID( CString strID )                           { m_strID = strID;                 };
    CString         GetMakeTime()                                    { return m_strMakeTime;            };
    void            SetMakeTime( CString strMakeTime )               { m_strMakeTime = strMakeTime;     };
    CString         GetVersion()                                     { return m_strVersion;             };
    void            SetVersion( CString strVersion )                 { m_strVersion = strVersion;       };

    CMyPtrArray*    GetPolicyItemList()                              { return m_paPolicyItem;           };
    void            SetPolicyItemList( CMyPtrArray* paPolicyItem )   { m_paPolicyItem = paPolicyItem;   };
// add
public :
    void            AddPolicyItem( CPolicyItem *pItem )              { m_paPolicyItem->Add(pItem);      };
// dump/toJson
public:
    void            Dump( CString strTab );
    CString         ToJson( CString strTab );
};