#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "MyStringArray.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CPolicyItem 
{
public:
	CPolicyItem();   // protected constructor used by dynamic creation
	virtual ~CPolicyItem();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected:
    CString         m_strDelayTime; 
    CString         m_strCheckTime;
    CString         m_strCheckVersion;
    CString         m_strUpgradeAlert;
    CMyStringArray* m_paUpgradeID;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
    CString         GetDelayTime()                                   { return m_strDelayTime;               };
    void            SetDelayTime( CString strDelayTime )             { m_strDelayTime = strDelayTime;       };
    CString         GetCheckTime()                                   { return m_strCheckTime;               };
    void            SetCheckTime( CString strCheckTime )             { m_strCheckTime = strCheckTime;       };
    CString         GetCheckVersion()                                { return m_strCheckVersion;            };
    void            SetCheckVersion( CString strCheckVersion )       { m_strCheckVersion = strCheckVersion; };
    CString         GetUpgradeAlert()                                { return m_strUpgradeAlert;            };
    void            SetUpgradeAlert( CString strUpgradeAlert )       { m_strUpgradeAlert = strUpgradeAlert; };

    CMyStringArray* GetUpgradeIDList()                               { return m_paUpgradeID;                };
    void            SetUpgradeIDList( CMyStringArray* paUpgradeID )  { m_paUpgradeID = paUpgradeID;         };

// add
public :
    void            AddUpgradeID( CString strID )                    { m_paUpgradeID->Add(strID);        };

// dump/toJson
public:
    void            Dump( CString strTab );
    CString         ToJson( CString strTab );

};