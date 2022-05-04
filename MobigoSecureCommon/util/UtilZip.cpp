////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilzip.h"
#include "utilfile.h"

#include "zip.h"
#include "unzip.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

BOOL     CUtilZip::m_bStopZip       = FALSE;
BOOL     CUtilZip::m_bStopUnzip     = FALSE;

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilZip::CUtilZip(void)
{
}

CUtilZip::~CUtilZip(void)
{
}
////////////////////////////////////////////////////////////////////////////////
// Zip( File -> Zip )
////////////////////////////////////////////////////////////////////////////////
CString CUtilZip::File2Zip( CString strPath, CString strZipPath, HWND hWndProgress )
{
    /* Example*/
    /*
        HZIP hz = CreateZip("simple1.zip",0);
        ZipAdd(hz,"znsimple.bmp",  "simple.bmp");
        ZipAdd(hz,"znsimple.txt",  "simple.txt");
        CloseZip(hz);
    */
    if(strPath.IsEmpty() == TRUE){
        return "";
    }

    HZIP       hz;
    CFileFind  fFileFind;
    CString    strExtension ="";
    CString    strNewFile="";
    CString    strOriginFile="";
    CString    strFilePath = strPath;

    BOOL bFind = fFileFind.FindFile(strPath);

    if( bFind == FALSE ){
        return "";
    }
    // zip 경로를 추가적으로 적지 않았을 경우에는 strPath에서 확장자만 zip으로 바꾼 경로를 zip path로 사용한다.
    if( strZipPath.IsEmpty() == TRUE ){
        if( bFind ==TRUE ){
            fFileFind.FindNextFile();
            strOriginFile = fFileFind.GetFileName();
            strNewFile = fFileFind.GetFileTitle() + ".zip";
            strFilePath.Replace( strOriginFile, strNewFile );
            hz = CreateZip( strFilePath, 0);
            ZipAdd(hz, strOriginFile ,  strPath );
            CloseZip(hz);
            
         }
    }else{

         if( bFind ==TRUE ){
            fFileFind.FindNextFile();
            strOriginFile = fFileFind.GetFileName();
            hz = CreateZip( strZipPath, 0);
            ZipAdd(hz, strOriginFile ,  strPath );
            CloseZip(hz);
         }

    }
    return strFilePath;
}

////////////////////////////////////////////////////////////////////////////////
// Zip( Dir -> Zip )
////////////////////////////////////////////////////////////////////////////////

// Example 1
// > input
//		strDirPath - "c:\\res"
//							- a.txt		
//							- b.txt
//		strZipPath - ""
//
// > result 
//		"c:\\res.zip\"
//			- a.txt
//			- b.txt
//
// -----------------------------------------------------------------------------
//
// Example 2
// > input
//		strDirPath - "c:\\res"
//							- a.txt		
//							- b.txt
//		strZipPath - "c:\\temp\res.zip"
//
// > result 
//		"c:\\temp\res.zip"
//						- a.txt
//						- b.txt

void CUtilZip::Dir2Zip( CString strPath, CString strZipPath, HWND hWndProgress )
{
    HZIP hz = CreateZip("simple1.zip",0);
    ZipAdd(hz,"znsimple.bmp",  "simple.bmp");
    ZipAdd(hz,"znsimple.txt",  "simple.txt");
    CloseZip(hz);
}

////////////////////////////////////////////////////////////////////////////////
// Unzip( Zip -> Dir )
////////////////////////////////////////////////////////////////////////////////


// Example 1
// > input
//		strZipPath - "c:\\res.zip"
//							- a.txt		
//							- b.txt
//		strDirPath - ""
//
// > result 
//		"c:\\res\"
//			- a.txt
//			- b.txt
//
// -----------------------------------------------------------------------------
//
// Example 2
// > input
//		strZipPath - "c:\\res.zip"
//							- a.txt		
//							- b.txt
//		strDirPath - "c:\\temp\zip"
//
// > result 
//		"c:\\temp\zip\"
//				- a.txt
//				- b.txt

/////////////////////////////////////////////////////////////////////////////
// send msg Method

void CUtilZip::Zip2Dir( CString strZipPath, CString strDirPath, HWND hWndProgress )
{
    m_bStopUnzip = FALSE;

    HZIP hZip = OpenZip( (const TCHAR *)strZipPath, (const TCHAR *)strDirPath, 0);
    
    ZIPENTRY ze; 
    ::GetZipItem(hZip, -1 /* -1 gives overall information about the zipfile */, &ze); 
    int nZipCount = ze.index;
    
	// 1. send file count
	if( hWndProgress != NULL ) {
		static CString strMsg;
		strMsg.Format( "%d", nZipCount );
		::SendMessage( hWndProgress, MY_MSG_PRORGESS_ZIP_INIT,  NULL, (LPARAM)&strMsg ); 
	}

    for( int i = 0; i < nZipCount; i++) { 
        ZIPENTRY ze; 
        ::GetZipItem(hZip,i, &ze);      // fetch individual details
        ::UnzipItem(hZip, i, ze.name);  // e.g. the item's name.

        // 2. check stop 
        if( m_bStopUnzip == TRUE ) {
            return;
        }

		// 3. send message 
		if( hWndProgress != NULL ) {
			static CString strMsg;
			strMsg.Format( "%s", ze.name ) ;
			::SendMessage( hWndProgress, MY_MSG_PRORGESS_ZIPPING,  NULL, (LPARAM)&strMsg ); 
		}
    }
    CloseZip(hZip);
}


