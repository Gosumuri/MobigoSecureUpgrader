////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UtilFTP.h"
#include "afxinet.h"
////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilFTP::CUtilFTP(void)
{
    
}

CUtilFTP::~CUtilFTP(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// FTP 로 파일 업로드 
// http://crowback.tistory.com/106
////////////////////////////////////////////////////////////////////////////////


BOOL CUtilFTP::FileUploadByFTP( CString strServerIP, CString strID, CString strPassword, CString strFilePath)
{
    CFtpConnection* ftpConn = NULL;
    CInternetSession* session;
    CString strFileName = "";
    CFileFind fFilefinder;

    if( fFilefinder.FindFile(strFilePath) ){
        fFilefinder.FindNextFile();
        strFileName = fFilefinder.GetFileName();
    }else{
        return FALSE;   
    }

    session = new CInternetSession(_T("My Session"));
    try
    {
        ftpConn = session->GetFtpConnection(strServerIP, strID, strPassword,INTERNET_DEFAULT_FTP_PORT,TRUE);
    }
    catch(CInternetException* e)
    {
     e->ReportError();
     ftpConn = NULL;
     session->Close();
     delete session ;
     e->Delete();
     return FALSE;
    }
    SYSTEMTIME st;
    ::GetLocalTime(&st);
        
    CTime      tToday(st);
    CString    strTodayFolder;

    strTodayFolder = tToday.Format("%Y%m%d");

    // 현재 연결된 계정의 FTP path
    CString strFTPCurDir;
    ftpConn->GetCurrentDirectory(strFTPCurDir);
    ftpConn->CreateDirectory(strTodayFolder);
    strFTPCurDir = strFTPCurDir + "/" + strTodayFolder;
    ftpConn->SetCurrentDirectory(strFTPCurDir);
    
    
    CString Server = strFTPCurDir + "/" + strFileName;  // 서버에서 받을 패스와 파일 이름
    CString Client = strFilePath;  // 보낼 파일의 패스와 파일 이름
    try
    {
     ftpConn->PutFile(Client, Server);
    }
    catch(CInternetException* e)
    {
     e->ReportError();
     e->Delete();
    }

    session -> Close();

    return TRUE;
}
