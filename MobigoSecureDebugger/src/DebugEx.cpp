#include "stdafx.h"
#include "debugex.h"

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor
////////////////////////////////////////////////////////////////////////////////

CDebugEx::CDebugEx(void)
{
}

CDebugEx::~CDebugEx(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// Init/Exit
////////////////////////////////////////////////////////////////////////////////

void CDebugEx::Init()
{
	CDebug::Init();
}

void CDebugEx::Exit()
{
	CDebug::Exit();
}

////////////////////////////////////////////////////////////////////////////////
// Get/Set
////////////////////////////////////////////////////////////////////////////////

void CDebugEx::SetTitle( LPCSTR lpstrTitle )
{
	CDebug::SetTitle( lpstrTitle );
}

void CDebugEx::SetConsoleLog(BOOL b)
{
	CDebug::SetConsoleLog(b);
}

void CDebugEx::SetFileLog(BOOL b)
{
	CDebug::SetFileLog(b);
}

void CDebugEx::SetAppPath( LPCSTR strAppPath )
{
	CDebug::SetAppPath(strAppPath);
}

LPCSTR CDebugEx::GetLogPath()
{
	return (LPCSTR)CDebug::GetLogPath();
}

LPCSTR CDebugEx::GetDebugPath()
{
	return (LPCSTR)CDebug::GetDebugPath().GetString();
}

LPCSTR CDebugEx::GetWarningPath()
{
	return (LPCSTR)CDebug::GetWarningPath().GetString();
}

LPCSTR CDebugEx::GetErrorPath()
{
	return (LPCSTR)CDebug::GetErrorPath().GetString();
}

LPCSTR CDebugEx::GetLightPath()
{
	return (LPCSTR)CDebug::GetLightPath().GetString();
}
LPCSTR CDebugEx::GetInterfacePath()
{
	return (LPCSTR)CDebug::GetInterfacePath().GetString();
}
LPCSTR CDebugEx::GetHtmlPath()
{
	return (LPCSTR)CDebug::GetHtmlPath().GetString();
}
LPCSTR CDebugEx::GetExpdInfoPath()
{
	return (LPCSTR)CDebug::GetExpdInfoPath().GetString();
}
void CDebugEx::SetCurPatternNum(int nCurPatternNum)
{
	CDebug::SetCurPatternNum(nCurPatternNum);
}

CString CDebugEx::GetDebugFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetDebugFilePath(bFileClose);
}
CString CDebugEx::GetWarningFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetWarningFilePath(bFileClose);
}
CString CDebugEx::GetErrorFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetErrorFilePath(bFileClose);
}
CString CDebugEx::GetLightFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetLightFilePath(bFileClose);
}
CString CDebugEx::GetGscFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetGscFilePath(bFileClose);
}
CString CDebugEx::GetInterfacFileePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetInterfacFileePath(bFileClose);
}
CString CDebugEx::GetHtmlFilePath(BOOL bFileClose /*= FALSE*/)
{
    return CDebug::GetHtmlFilePath(bFileClose);
}
int CDebugEx::GetCurPatternNum()
{
	return CDebug::GetCurPatternNum();
}

LPCSTR CDebugEx::PatternNum2Str( int nPatternNum )
{
	return CGscConst2Str::PatternNum2Str(nPatternNum);
}

LPCSTR CDebugEx::Operation2Str(int nOpertation)
{
	return CGscConst2Str::Operation2Str(nOpertation);
}

LPCSTR CDebugEx::VendorID2Str(int nVendorID)
{
	return CGscConst2Str::VendorID2Str(nVendorID);
}

////////////////////////////////////////////////////////////////////////////////
// log function
////////////////////////////////////////////////////////////////////////////////

void CDebugEx::LogEx( LPCSTR lpstrModuleName, LPCSTR lpstrFile, int nLine, int nLevel, int nColor, LPCSTR lpstrFormat, ...)
{
	va_list args;
	CString strResult;

	/** log안에 "%SystemRoot%"가 있을 경우, strResult.FormatV()에서 죽는다. %S에 대한 argument를 찾는 듯 ... */ 
	//-CString strFormat(lpstrFormat);
	//-strFormat.Replace( "%SystemRoot%", "_SYSTEM_ROOT_");

	va_start(args, lpstrFormat);
	strResult.FormatV(lpstrFormat, args);
	va_end(args);
	
	CDebug::Log(lpstrModuleName, lpstrFile, nLine, nLevel, nColor, strResult);
}

////////////////////////////////////////////////////////////////////////////////
// utility function 
////////////////////////////////////////////////////////////////////////////////

void CDebugEx::ClearScreen()
{
	CDebug::ClearScreen();
}

void CDebugEx::Toggle()
{
	CDebug::Toggle();
}

void CDebugEx::DecreaseThreadCount()
{
	CDebug::DecreaseThreadCount();
}

CString CDebugEx::ChangeFileName( CString strFilePath, CString strFilePrefix, CString strFileSuffix)
{
    CString strChangeFileName;
    CString strChangeFilePath = "";
    CString strOriginFileName ="";
    CString strExtension ="";
    CFileFind fFilefinder;
    BOOL bTEST = fFilefinder.FindFile(strFilePath);
    if( bTEST ==TRUE ){
        fFilefinder.FindNextFile();
        
        strExtension = fFilefinder.GetFileName();
        strOriginFileName = fFilefinder.GetFileTitle();

        strExtension.Replace(strOriginFileName, ""); 

        strChangeFileName = strFilePrefix + "_"+ strOriginFileName + "_" + strFileSuffix + strExtension;
        strChangeFilePath = strFilePath;
        strChangeFilePath.Replace( fFilefinder.GetFileName() , strChangeFileName);
        CFile::Rename(strFilePath, strChangeFilePath);
    }

    return strChangeFilePath;
}