#include "StdAfx.h"
#include "utilfile.h"

#ifndef MAX_BUF
	#define MAX_BUF 256
#endif

CUtilFile::CUtilFile(void)
{
}

CUtilFile::~CUtilFile(void)
{
}

void CUtilFile::DeleteDir( CString strDirPath )
{
	CUtilFile::DeleteFolder( (char*)(LPCTSTR)strDirPath);

}

void CUtilFile::DeleteFolder(LPTSTR lpFolder)
{
    WIN32_FIND_DATA  FindData;
    HANDLE    FindHandle;
    char temp1[MAX_PATH];
    int lpFolder_Len;

    if(lpFolder==NULL)
    {
        return;
    }
    if(*lpFolder=='\0')
    {
        return;
    }
    lpFolder_Len=strlen(lpFolder);
    if(lpFolder[lpFolder_Len-1]=='\\')
    {
        sprintf(temp1, "%s*.*", lpFolder);
    }
    else
    {
        sprintf(temp1, "%s\\*.*", lpFolder);
    }
    FindHandle=FindFirstFile(temp1, &FindData);
    if(INVALID_HANDLE_VALUE!=FindHandle)
    {
        while(1)
        {
            if(lpFolder[lpFolder_Len-1]=='\\')
            {
                sprintf(temp1, "%s%s", lpFolder, FindData.cFileName);
            }
            else
            {
                sprintf(temp1, "%s\\%s", lpFolder, FindData.cFileName);
            }
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //Dir
                if(strcmp(FindData.cFileName, ".")==0)
                {
                    //.    -->> skip
                }
                else if(strcmp(FindData.cFileName, "..")==0)
                {
                    //..   -->> skip
                }
                else
                {
                    DeleteFolder(temp1);
                    if((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)||
                        (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                    {
                        SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                    }
                    RemoveDirectory(temp1);
                }
            }
            else
            {
                //File
                if((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)||
                    (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                {
                    SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                }
                DeleteFile(temp1);
            }
            if(!FindNextFile(FindHandle, &FindData))
            {
                break;
            }
        }
    }
    FindClose(FindHandle);

    SetFileAttributes(lpFolder, FILE_ATTRIBUTE_NORMAL);
    RemoveDirectory(lpFolder);
}

void CUtilFile::DeleteFile(   CString strFilePath )
{
	::DeleteFile( strFilePath );
}

void CUtilFile::WriteString( CString strFilePath, CString str )
{
    // check path 
    CString strPath = CUtilFile::ExtractPath( strFilePath );
    if( CUtilFile::IsExistPath( strPath ) == FALSE ) {
        CUtilFile::MakeDir( strPath + "\\" );
    }
    
    // write string 
	TRY {
		CStdioFile sf;
		sf.Open( strFilePath, 
				CFile::modeCreate 
				| CFile::typeText 
				| CFile::modeWrite,  
				NULL);

		sf.WriteString( str );
		sf.Close(); 
	}
	CATCH( CFileException, e ){
		char szErrMsg[MAX_BUF];
		memset( szErrMsg, 0, MAX_BUF );
        e->GetErrorMessage(szErrMsg, MAX_BUF, NULL);
		AfxMessageBox( szErrMsg );
    }
    END_CATCH
}

#include <io.h>

BOOL CUtilFile::IsExistPath( CString strFilePath )
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;

    if((hFind = ::FindFirstFile( (char*)(LPCTSTR)strFilePath, &fd)) != INVALID_HANDLE_VALUE)  
		return TRUE;

	 return FALSE;
}

void CUtilFile::MakeDir( CString strDirPath  )
{
	strDirPath.Replace( '/', '\\' );

	CFileFind ff;
	int nStart = 0;
	while( true ) { 
		
		nStart = strDirPath.Find( '\\', nStart + 1 );
		if( nStart == -1 )
			break;

		CString strSubPath = strDirPath.Mid( 0, nStart );
		if( ff.FindFile( strSubPath ) == FALSE )
			::CreateDirectory( strSubPath, NULL );
	}
}
 
// "c:\\foldername\\filename.ext" --> "c:\\foldername"
CString CUtilFile::ExtractPath( CString strFilePath )
{
	strFilePath.Replace( '/', '\\' );

	int nIndex  = strFilePath.ReverseFind( '\\' );

	if( nIndex == -1 ) {
		return strFilePath;
	}

	return strFilePath.Mid( 0, nIndex );
}


CString CUtilFile::ExtractFileName( CString strFilePath)
{
	strFilePath.Replace( '/', '\\' );

	int nFirstIndex = strFilePath.ReverseFind( '\\' );
	if( nFirstIndex == -1 ) 
		return strFilePath;
					
	return strFilePath.Mid( nFirstIndex + 1 );
}

CString CUtilFile::ExtractFileExt( CString strFilePath)
{
	strFilePath.Replace( '/', '\\' );

	int nFirstIndex  = strFilePath.ReverseFind( '\\' );
	int nSecondIndex = strFilePath.ReverseFind( '.' );

	// 1. "\\folername\\fileName" 
	if( nSecondIndex == -1 ) 
		return "";
	// 2. "\\foler.name\\fileName" 
	else if( nFirstIndex > nSecondIndex )
		return "";
	
	return strFilePath.Mid( nSecondIndex + 1 );
}


CString CUtilFile::ExtractFileNameWithoutExt( CString strFilePath )
{
	strFilePath.Replace( '/', '\\' );

	CString strFileName = CUtilFile::ExtractFileName( strFilePath );

	int nIndex = strFileName.ReverseFind( '.' );

	if( nIndex == -1 ) {
		return strFileName;
	}

	return strFileName.Mid( 0, nIndex );
}
CString CUtilFile::ReadString( CString strFilePath )
{
	CString strRet = "" ;

	TRY {
		// 0. create
		CStdioFile sf;

		// 1. open
		BOOL b = sf.Open(	strFilePath, 
							CFile::typeText 
							| CFile::modeRead, 
							NULL);

		if( b == FALSE ) {
			AfxMessageBox( "Not Exist File : " + strFilePath );
			return "";
		}

		// 2. read
		CString strBuf = "";
		while( sf.ReadString( strBuf ) == TRUE ) {
			strRet += strBuf + "\r\n";
		}
		strRet += strBuf; 

		// 3. close
		sf.Close();
	}
	CATCH( CFileException, e ){
		char szErrMsg[MAX_BUF];
		memset( szErrMsg, 0, MAX_BUF );
        e->GetErrorMessage(szErrMsg, MAX_BUF, NULL);
		AfxMessageBox( szErrMsg );
    }
    END_CATCH

	// 4. return
	return strRet;
}


int CUtilFile::GetDirectorySize( CString strDirectoryPath )
{
	int nSize = 0;

	strDirectoryPath.TrimRight(_T("\\"));
	CFileFind	ff;
	BOOL		b = ff.FindFile(strDirectoryPath + _T("\\*"));
	while (b) {
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory()) {
			nSize += GetDirectorySize( ff.GetFilePath() );
		}
		else {
			CFileStatus fs;
			if (CFile::GetStatus(ff.GetFilePath(), fs)) {
				nSize += fs.m_size;
			}
		}
	}

	return nSize;
}

int CUtilFile::GetSize( CString strPath )
{
	if( CUtilFile::IsDirectory( strPath ) ) {
		return CUtilFile::GetDirectorySize( strPath );
    } else {
		return CUtilFile::GetFileSize( strPath );
    }
}

int CUtilFile::GetFileSize( CString strFilePath )
{
    if( CUtilFile::IsExistPath( strFilePath ) == TRUE ) {
	    CFileFind		ff;
	    ff.FindFile(strFilePath);
	    ff.FindNextFile();	
	    return ff.GetLength(); 	
    } else {
        return -1;
    }
}

CString CUtilFile::GetNewFilePath( CString strFilePath )
{
	CFileFind filefind;
	CString   strNewFileName = strFilePath ;

	if( filefind.FindFile(strNewFileName) == FALSE )
		return strNewFileName;

	// 1. path
	int n1 = strFilePath.ReverseFind( '\\' );	
	CString strPath = strFilePath.Mid( 0, n1 );
	
	// 2. fileName
	int n2 = strFilePath.ReverseFind( '.' );	
	CString strFileName = strFilePath.Mid( n1 + 1, n2 - ( n1 + 1 ) );
	
	// 3. ext
	CString strExt = strFilePath.Mid( n2 + 1 );

	int nCount = 1;
	while( true ) {
		CString strCount;
		strCount.Format( "%d", nCount++ );
		strNewFileName = strPath + "\\" + strFileName + "(" + strCount + ")." + strExt;

		if(filefind.FindFile(strNewFileName) == FALSE )		
			break;
	}

	return strNewFileName ;
}

BOOL CUtilFile::IsDirectory( CString strFilePath )
{
	if( CUtilFile::IsExistPath( strFilePath ) == FALSE )
		return FALSE;

	CFileFind		ff;
	ff.FindFile(strFilePath);
	
	ff.FindNextFile();	

	return ff.IsDirectory(); 	
}