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

class AFX_EXT_CLASS CUtilOS
{
public:
	CUtilOS(void);
	~CUtilOS(void);

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public : 
    static BOOL         Is64BitOS(); 
    static BOOL         IsTablet();
    static CString      GetOSVersion();
    static CString      GetOSName();


private :
    static int          GetMajorVersionNumberFromRegistry();
};
