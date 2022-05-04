#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "UpgradeDataLoader.h"
#include "PolicyDataLoader.h"

#include "UpgradeData.h"
#include "PolicyData.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CDataMgr 
{
public :
	CDataMgr();   // protected constructor used by dynamic creation
	virtual ~CDataMgr();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected:
    CUpgradeDataLoader      *m_pUpgradeDataLoader;
    CPolicyDataLoader       *m_pPolicyDataLoader;

    CUpgradeData            *m_pUpgradeData;
    CPolicyData             *m_pPolicyData;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
    CUpgradeDataLoader*   GetUpgradeDataLoader()                        { return m_pUpgradeDataLoader;   }
    CPolicyDataLoader*    GetPolicyDataLoader()                         { return m_pPolicyDataLoader;    }
    CUpgradeData*         GetUpgradeData()                              { return m_pUpgradeData;         }
    void                  SetUpgradeData( CUpgradeData* pUpgradeData )  { m_pUpgradeData = pUpgradeData; }
    CPolicyData*          GetPolicyData()                               { return m_pPolicyData;          }
    void                  SetPolicyData( CPolicyData* pPolicyData )     { m_pPolicyData = pPolicyData;   }
// save
public :
    void                  SaveUpgradeData( CString strUpgradeListPath );

// public 
public :
    CString               ToJson();
    BOOL                  IsValid();

};