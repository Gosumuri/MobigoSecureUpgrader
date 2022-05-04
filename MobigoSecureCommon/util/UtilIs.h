#pragma once

class AFX_EXT_CLASS CUtilIs
{
public:
	CUtilIs(void);
	~CUtilIs(void);
// success
public:
	static BOOL	IsSuccess(          CString strXML );
// ip 
public:
	static BOOL IsValidIP(			CString strIP   );
	static BOOL IsValidIP(			char	*szIP   );
	static BOOL IsValidPort(	    CString strPort );	
// private
public:
	static int	CountDot(			char	*sz );
	static BOOL IsDigit(			CString str );
	static BOOL	IsDigit(			char	*sz );
// Etc
public :
	static BOOL IsReservedSubTreeID( CString strEleID );
	static BOOL IsSystemEleID( CString strEleID );
};
