#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CAppItem 
{
public:
	CAppItem();   // protected constructor used by dynamic creation
	virtual ~CAppItem();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected:
    CString        m_strNum; 
    CString        m_strFileApp; 
    CString        m_strShortLnk; 
    CString        m_strActivation;
    CString        m_strFrun;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
    CString         GetNum()                                 { return m_strNum;                 };
    void            SetNum( CString strNum )                 { m_strNum = strNum;               };
    CString         GetFileApp()                             { return m_strFileApp;             };
    void            SetFileApp( CString strFileApp )         { m_strFileApp = strFileApp;       };
    CString         GetShortLnk()                            { return m_strShortLnk;            };
    void            SetShortLnk( CString strShortLnk )       { m_strShortLnk = strShortLnk;     };

    CString         GetActivation()                          { return m_strActivation;          };
    void            SetActivation( CString strActivation )   { m_strActivation = strActivation; };
    CString         GetFrun()                                { return m_strFrun;                };
    void            SetFrun( CString strFrun )               { m_strFrun = strFrun;             };

// dump/toJson
public:
    void            Dump( CString strTab );
    CString         ToJson( CString strTab );

};