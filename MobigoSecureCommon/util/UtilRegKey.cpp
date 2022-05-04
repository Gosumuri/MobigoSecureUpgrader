#include "StdAfx.h"
#include "utilregkey.h"

#ifndef MAX_BUF
	#define MAX_BUF 256
#endif

CUtilRegKey::CUtilRegKey(void)
{
}

CUtilRegKey::~CUtilRegKey(void)
{
}

BOOL CUtilRegKey::GetValueStr( HKEY hRoot, CString strKey, CString strSubKey, CString &strValue )
{
	HKEY	hKey;
    DWORD	dwType;
    DWORD	dwSize = MAX_BUF;
    char	szBuf[MAX_BUF];
	
	memset( szBuf, 0, MAX_BUF ); 

	// 1. Key를 찾는다.
    if (RegOpenKeyEx(hRoot, (LPSTR)(char*)(LPCTSTR)strKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return FALSE;

    // 2. strSubKey의 값을 얻는다.
    if (RegQueryValueEx (hKey, (LPSTR)(char*)(LPCTSTR)strSubKey, NULL,  &dwType, (BYTE *)szBuf, &dwSize)!=ERROR_SUCCESS)
        return FALSE;

	// 3. close
    RegCloseKey(hKey);

	// 4. save value
    strValue.SetString( szBuf );

    return TRUE;
}

BOOL CUtilRegKey::SetValueStr( HKEY hRoot, CString strKey, CString strSubKey, CString strValue )
{
    HKEY hKey;
	LONG lResult;
    RegOpenKeyEx(hRoot, strKey, 0, KEY_ALL_ACCESS, &hKey);

    // Set Registry Key & Value
    lResult = RegCreateKeyEx(hRoot, strKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (lResult != ERROR_SUCCESS) { return FALSE; }

    lResult = RegSetValueEx(hKey, strSubKey, 0, REG_SZ, (BYTE*)strValue.GetString(), strlen(strValue));
    if (lResult != ERROR_SUCCESS) { return FALSE; }

    RegCloseKey(hKey);
    return TRUE;
}

BOOL CUtilRegKey::GetValueDword(HKEY hRoot, CString strKey, CString strSubKey, DWORD &dwValue)
{
    HKEY	hKey;
    DWORD	dwType;
    DWORD   dwData;
    DWORD	dwSize = MAX_BUF;

    // 1. Key를 찾는다.
    if (RegOpenKeyEx(hRoot, (LPSTR)(char*)(LPCTSTR)strKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return FALSE;

    // 2. strSubKey의 값을 얻는다.
    if (RegQueryValueEx(hKey, (LPSTR)(char*)(LPCTSTR)strSubKey, NULL, &dwType, (LPBYTE)&dwData, &dwSize) != ERROR_SUCCESS)
        return FALSE;

    // 3. close
    RegCloseKey(hKey);

    // 4. save value
    dwValue = dwData;

    return TRUE;
}

BOOL CUtilRegKey::SetValueDword(HKEY hRoot, CString strKey, CString strSubKey, DWORD dwValue)
{
    HKEY hKey;
    LONG lResult;
    RegOpenKeyEx(hRoot, strKey, 0, KEY_ALL_ACCESS, &hKey);

    // Set Registry Key & Value
    lResult = RegCreateKeyEx(hRoot, strKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (lResult != ERROR_SUCCESS) { return FALSE; }

    lResult = RegSetValueEx(hKey, strSubKey, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue));
    if (lResult != ERROR_SUCCESS) { return FALSE; }

    RegCloseKey(hKey);
    return TRUE;
}