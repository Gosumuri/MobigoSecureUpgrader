////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilhttpget.h"
#include <afxinet.h>
#include <winreg.h>
#include <Wininet.h>


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

// Buffer Size
#define		MAX_BUF								256
#define     TIME_OUT                            5000 
////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static 
////////////////////////////////////////////////////////////////////////////////

int CUtilHttpGet::m_nYesNo = -1;

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilHttpGet::CUtilHttpGet(void)
{
}

CUtilHttpGet::~CUtilHttpGet(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// membmer function 
////////////////////////////////////////////////////////////////////////////////


// https://m.blog.naver.com/rantris/30106660689
CString CUtilHttpGet::Request( CString strRequest )
{
	CString             strResponse = "";
	CInternetSession	internetSession;
    internetSession.SetOption( INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT , TIME_OUT );
    internetSession.SetOption( INTERNET_OPTION_CONTROL_SEND_TIMEOUT ,    TIME_OUT );
    internetSession.SetOption( INTERNET_OPTION_DATA_SEND_TIMEOUT ,       TIME_OUT );
    internetSession.SetOption( INTERNET_OPTION_DATA_RECEIVE_TIMEOUT ,    TIME_OUT );
    m_nYesNo = -1;          
	try {

		CInternetFile*		internetFile;

		char* pSzBur = (char*)malloc( sizeof(char)*MAX_BUF);
		memset( pSzBur, 0, sizeof(char) * MAX_BUF );
		internetFile = (CInternetFile *) internetSession.OpenURL( strRequest, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE );

		int n = 0;
		while( ( n = internetFile->Read( pSzBur, sizeof( MAX_BUF ))) ) {
			strResponse.Append( pSzBur, n );
			memset( pSzBur, 0, sizeof(char) * MAX_BUF );
		}

		free( pSzBur );
		internetFile->Close();
		delete internetFile;

	} catch (CInternetException* pEx ) {

		strResponse = "\"" + strRequest + "\"을 열수 없습니다.";
        TCHAR pszError[128];
        memset( pszError, 0, sizeof(pszError)/sizeof(TCHAR));
        pEx->GetErrorMessage(pszError, sizeof(pszError) - 1);
        strResponse += "\r\n"; 
        strResponse += "\r\n"; 
        strResponse += pszError; 
        strResponse += "\r\n"; 
        strResponse += "다시 시도하시겠습니까?"; 
		m_nYesNo = ::MessageBox( NULL, strResponse, "Network Error", MB_ICONERROR | MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST );
    } catch (...) {
		strResponse = "\"" + strRequest + "\"을 열수 없습니다.\r\n";
        strResponse += "\r\n"; 
        strResponse += "\r\n";
        strResponse += "다시 시도하시겠습니까?"; 
		m_nYesNo = ::MessageBox( NULL, strResponse, "Network Error", MB_ICONERROR | MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST );
	}

    internetSession.Close();

	return strResponse;
}