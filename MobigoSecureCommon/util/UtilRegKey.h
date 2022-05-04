#pragma once

class AFX_EXT_CLASS CUtilRegKey
{
public:
	CUtilRegKey(void);
	~CUtilRegKey(void);

	static BOOL		GetValueStr(  HKEY hRoot, CString strKey, CString strSubKey, CString &strValue );
	static BOOL		SetValueStr(  HKEY hRoot, CString strKey, CString strSubKey, CString strValue  );
    static BOOL     GetValueDword(HKEY hRoot, CString strKey, CString strSubKey, DWORD &dwValue    );
    static BOOL     SetValueDword(HKEY hRoot, CString strKey, CString strSubKey, DWORD dwValue     );
};
