
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpgradeItem.h"
#include "UtilFile.h"
#include "Util2.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// get/set 
///////////////////////////////////////////////////////////////////////////////

BOOL CUpgradeItem::IsZipFile() 
{
    CString strExt = CUtilFile::ExtractFileExt(m_strSavePath); 
    if( strExt.CompareNoCase( "zip" ) == 0  ) {
        return TRUE;
    }
    return FALSE;
}

BOOL CUpgradeItem::IsApkFile() 
{
    CString strExt = CUtilFile::ExtractFileExt(m_strSavePath); 
    if( strExt.CompareNoCase( "apk" ) == 0  ) {
        return TRUE;
    }
    return FALSE;
}

CString CUpgradeItem::GetUnzipPath()
{
    return CUtilFile::ExtractPath(m_strSavePath); 
}

CString CUpgradeItem::GetUnapkPath()
{
    CString strPath     = CUtilFile::ExtractPath(              m_strSavePath);
    CString strFileName = CUtilFile::ExtractFileNameWithoutExt(m_strSavePath);

    return strPath + "\\" + strFileName; 
}

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUpgradeItem::CUpgradeItem() 
{
	//-DEBUG_LOG(">> CUpgradeItem::CUpgradeItem()");

    m_paUpgradeItem = NULL;
}

CUpgradeItem::~CUpgradeItem() 
{ 
	//-DEBUG_LOG(">> CUpgradeItem::~CUpgradeItem()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CUpgradeItem::Init()
{
	//-DEBUG_LOG(">> CUpgradeItem::Init()");

    m_strID             = ""; 
    m_strDownloadUrl    = ""; 
    m_strSavePath       = ""; 
    m_nSize             = 0;
    m_bEncyption        = FALSE;
    m_bEncyption        = FALSE;
    m_strVersion        = "";
    m_strApkVersion     = "";
    
    m_strIcon           = "";
    m_strCRC            = "";

    if( m_paUpgradeItem == NULL ) {
        m_paUpgradeItem = new CMyPtrArray();
    }  

    m_bUpgrade          = FALSE;
}

void CUpgradeItem::Exit()
{
	//-DEBUG_LOG(">> CUpgradeItem::Exit()");

    if( m_paUpgradeItem != NULL ) {
        for( int i = m_paUpgradeItem->GetCount() - 1 ; 0 <= i ; i-- ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->Exit();
                delete pItem;
            }
        }
        m_paUpgradeItem->RemoveAll();
        delete m_paUpgradeItem;
        m_paUpgradeItem = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// public funtion
///////////////////////////////////////////////////////////////////////////////

int CUpgradeItem::CalcTotalSize()
{
	//-DEBUG_LOG(">> CUpgradeItem::CalcTotalSize()");

    int nTotalSize = m_nSize;

    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                nTotalSize += pItem->CalcTotalSize();
            } 
        }
    }

    return nTotalSize;
}

int CUpgradeItem::CalcDownloadSize()
{
	//-DEBUG_LOG(">> CUpgradeItem::CalcDownloadSize()");

    int nTotalSize = m_nSize;

    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                if( pItem->IsDownloadComplete() == FALSE ) {
                    nTotalSize += pItem->CalcTotalSize();
                }
            } 
        }
    }

    return nTotalSize;
}

void CUpgradeItem::FillUrl2PathMap( CMapStringToString* pMapUrl2Path )
{
	//-DEBUG_LOG(">> CUpgradeItem::FillUrl2PathMap()");

    if( m_bUpgrade == TRUE ) {
        // add <url,path> map
        if( m_strDownloadUrl.IsEmpty() == FALSE ) {
            pMapUrl2Path->SetAt( m_strDownloadUrl, m_strSavePath );
        }

        // add <icon,path> map
        if( m_strIcon.IsEmpty() == FALSE ) {
            CString strIconPath = CUtilFile::ExtractPath(m_strSavePath) + "\\" + CUtilFile::ExtractFileName(m_strIcon);
            pMapUrl2Path->SetAt( m_strIcon, strIconPath );
        }
    }
    
    // recursive <url,path> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillUrl2PathMap( pMapUrl2Path );
            } 
        }
    } 
}

void CUpgradeItem::FillZip2DirMap( CMapStringToString* pMapZip2Dir )
{
	//-DEBUG_LOG(">> CUpgradeItem::FillZip2DirMap()");

    // add <zip,dir> map
    if( m_strSavePath.IsEmpty() == FALSE ) {
        if( m_bUnzipComplete == FALSE ) {
            BOOL bZipFile = IsZipFile();
            if( bZipFile == TRUE ) {
                CString strDirPath = GetUnzipPath();
                pMapZip2Dir->SetAt( m_strSavePath, strDirPath );
            }
        }
    }

    // recursive <zip,dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillZip2DirMap( pMapZip2Dir );
            } 
        }
    } 
}

//Deprecated
void CUpgradeItem::FillSystemApk2DirMap( CMapStringToString* pMapApk2Dir, CString strSystemApkItemID, BOOL bSystemApk )
{
	//-DEBUG_LOG(">> CUpgradeItem::FillSystemApk2DirMap()");

    if( bSystemApk == FALSE ) {
        if( m_strID.CompareNoCase( strSystemApkItemID ) == 0 ) {
            bSystemApk = TRUE;
        }
    }

    // add <apk,dir> map
    if( bSystemApk == TRUE ) {
        if( m_strSavePath.IsEmpty() == FALSE ) {
            if( m_bUnzipComplete == FALSE ) {
                BOOL bApkFile = IsApkFile();
                if( bApkFile == TRUE ) {
                    CString strDirPath = GetUnapkPath();
                    pMapApk2Dir->SetAt( m_strSavePath, strDirPath );
                }
            }
        }
    }

    // recursive <zip,dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillSystemApk2DirMap( pMapApk2Dir, strSystemApkItemID, bSystemApk );
            } 
        }
    } 
}

void CUpgradeItem::FillSystemApk2IDMap( CMapStringToString* pMapApk2ID, CString strSystemApkItemID, BOOL bSystemApk )
{
	//-DEBUG_LOG(">> CUpgradeItem::FillSystemApk2DirMap()");

    if( bSystemApk == FALSE ) {
        if( m_strID.CompareNoCase( strSystemApkItemID ) == 0 ) {
            bSystemApk = TRUE;
        }
    }

    // add <apk,dir> map
    if( bSystemApk == TRUE ) {
        if( m_strSavePath.IsEmpty() == FALSE ) {
            if( m_bUnzipComplete == FALSE ) {
                BOOL bApkFile = IsApkFile();
                if( bApkFile == TRUE ) {
                    pMapApk2ID->SetAt( m_strSavePath, m_strID );
                }
            }
        }
    }

    // recursive <zip,dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillSystemApk2IDMap( pMapApk2ID, strSystemApkItemID, bSystemApk );
            } 
        }
    } 
}

void CUpgradeItem::FillID2PtrMap( CMapStringToPtr* pMapID2Ptr )
{
	//-DEBUG_LOG(">> CUpgradeItem::FillID2PtrMap()");

    // add <id,dir> map
    if( m_strID.IsEmpty() == FALSE ) {
        pMapID2Ptr->SetAt( m_strID, this );
    }

    // recursive <id,dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillID2PtrMap( pMapID2Ptr );
            } 
        }
    } 
}

CUpgradeItem* CUpgradeItem::FindUpgradeItemByApkFileName( CString strApkFileName )
{
	//-DEBUG_LOG(">> CUpgradeItem::FindUpgradeItemByApkFileName()");

    // check apk-file-name
    CString strFileName = CUtilFile::ExtractFileName( m_strSavePath );
    if( strFileName.CompareNoCase( strApkFileName ) == 0 ) {
        return (CUpgradeItem*)this;
    } 

    // recursive
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                CUpgradeItem *pUpgradeItem = pItem->FindUpgradeItemByApkFileName( strApkFileName );
                if( pUpgradeItem != NULL ) {
                    return pUpgradeItem;
                }
            } 
        }
    }

    return NULL;
}

CUpgradeItem* CUpgradeItem::FindUpgradeItemByID( CString strID )
{
	//-DEBUG_LOG(">> CUpgradeItem::FindUpgradeItemByID()");

    // check id
    if( m_strID.CompareNoCase( strID ) == 0 ) {
        return (CUpgradeItem*)this;
    } 

    // recursive
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                CUpgradeItem *pUpgradeItem = pItem->FindUpgradeItemByID( strID );
                if( pUpgradeItem != NULL ) {
                    return pUpgradeItem;
                }
            } 
        }
    }

    return NULL;
}

CUpgradeItem* CUpgradeItem::FindUpgradeItemByDownloadUrl( CString strDownloadURL )
{
	//-DEBUG_LOG(">> CUpgradeItem::FindUpgradeItemByDownloadUrl()");

    // check download url
    if( m_strDownloadUrl.CompareNoCase( strDownloadURL ) == 0 ) {
        return (CUpgradeItem*)this;
    } 

    // recursive
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                CUpgradeItem *pUpgradeItem = pItem->FindUpgradeItemByDownloadUrl( strDownloadURL );
                if( pUpgradeItem != NULL ) {
                    return pUpgradeItem;
                }
            } 
        }
    }

    return NULL;
}

CUpgradeItem* CUpgradeItem::FindUpgradeItemBySavePath( CString strSavePath )
{
	//-DEBUG_LOG(">> CUpgradeItem::FindUpgradeItemBySavePath()");

    // check download url
    if( m_strSavePath.CompareNoCase( strSavePath ) == 0 ) {
        return (CUpgradeItem*)this;
    } 

    // recursive
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                CUpgradeItem *pUpgradeItem = pItem->FindUpgradeItemBySavePath( strSavePath );
                if( pUpgradeItem != NULL ) {
                    return pUpgradeItem;
                }
            } 
        }
    }

    return NULL;
}


CString CUpgradeItem::ReadStringByID( CString strID )
{
    CUpgradeItem* pUpgradeItem = FindUpgradeItemByID( strID );
    if( pUpgradeItem != NULL ) {
        CString strLanguagePath = pUpgradeItem->GetSavePath();
        if( CUtilFile::IsExistPath( strLanguagePath ) == TRUE ) {
            CString strLanguage     = CUtilFile::ReadString( strLanguagePath );
            return strLanguage;
        }
    }

    return "";
}

int CUpgradeItem::CompareVersion( CUpgradeItem *pUpgradeItem )
{
    return m_strVersion.Compare( pUpgradeItem->GetVersion() );
}

////////////////////////////////////////////////////////////////////////////////
// dump functin
///////////////////////////////////////////////////////////////////////////////

void CUpgradeItem::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CUpgradeItem::Dump()");
	//-DEBUG_LOG("++   %sm_strID                = %s", (LPCSTR)strTab, (LPCSTR)m_strID              );
	//-DEBUG_LOG("++   %sm_strDownloadUrl       = %s", (LPCSTR)strTab, (LPCSTR)m_strDownloadUrl     );
	//-DEBUG_LOG("++   %sm_strSavePath          = %s", (LPCSTR)strTab, (LPCSTR)m_strSavePath        );
	//-DEBUG_LOG("++   %sm_nSize                = %d", (LPCSTR)strTab,         m_nSize              );
	//-DEBUG_LOG("++   %sm_bEncyption           = %s", (LPCSTR)strTab, B2S(    m_bEncyption)        );
	//-DEBUG_LOG("++   %sm_bMandatory           = %s", (LPCSTR)strTab, B2S(    m_bMandatory)        );
	//-DEBUG_LOG("++   %sm_strVersion           = %s", (LPCSTR)strTab, (LPCSTR)m_strVersion         );
	//-DEBUG_LOG("++   %sm_strApkVersion        = %s", (LPCSTR)strTab, (LPCSTR)m_strApkVersion      );

	//-DEBUG_LOG("++   %sm_strIcon              = %s", (LPCSTR)strTab, (LPCSTR)m_strIcon            );
	//-DEBUG_LOG("++   %sm_strCRC               = %s", (LPCSTR)strTab, (LPCSTR)m_strCRC             );
	//-DEBUG_LOG("++   %sm_bUpgrade             = %s", (LPCSTR)strTab, B2S(    m_bUpgrade)          );
	//-DEBUG_LOG("++   %sm_bDownloadComplete    = %s", (LPCSTR)strTab, B2S(    m_bDownloadComplete) );
	//-DEBUG_LOG("++   %sm_bUnzipComplete       = %s", (LPCSTR)strTab, B2S(    m_bUnzipComplete)    );


    if( m_paUpgradeItem != NULL ) {
	    //-DEBUG_LOG("++   %sm_paUpgradeItem->GetCount() = %d", (LPCSTR)strTab, m_paUpgradeItem->GetCount() );
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
	            //-DEBUG_LOG("++   %s[%d] CUpgradeItem", (LPCSTR)strTab, i );
                pItem->Dump( strTab + _T("    ") );
            } else {
	            //-DEBUG_LOG("++   %s[%d] CUpgradeItem = NULL", (LPCSTR)strTab, i );
            }
        }
    } else {
	    //-DEBUG_LOG("++   %sm_paUpgradeItem  = NULL", (LPCSTR)strTab                     );
    }
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CUpgradeItem::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CUpgradeItem::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                                                                   + "\r\n";
    if( m_strID.IsEmpty()          == FALSE ) strJson += strTab + "    \"id\"                : " + "\""+                      m_strID                 + "\"," + "\r\n";
    if( m_strIcon.IsEmpty()        == FALSE ) strJson += strTab + "    \"icon\"              : " + "\""+                      m_strIcon               + "\"," + "\r\n";
    if( m_strDownloadUrl.IsEmpty() == FALSE ) strJson += strTab + "    \"download_url\"      : " + "\""+                      m_strDownloadUrl        + "\"," + "\r\n";
    if( m_strSavePath.IsEmpty()    == FALSE ) strJson += strTab + "    \"save_path\"         : " + "\""+                      m_strSavePath           + "\"," + "\r\n";
    if( m_strCRC.IsEmpty()         == FALSE ) strJson += strTab + "    \"crc\"               : " + "\""+                      m_strCRC                + "\"," + "\r\n";
    if( m_bEncyption               == TRUE  ) strJson += strTab + "    \"encryption\"        : " +       CUtil2::Bool2Str(    m_bEncyption )          + "  ," + "\r\n";
    if( m_bMandatory               == TRUE  ) strJson += strTab + "    \"option\"            : " + "\""+ CUtil2::Bool2Option( m_bMandatory )          + "\"," + "\r\n";
    if( m_strVersion.IsEmpty()     == FALSE ) strJson += strTab + "    \"version\"           : " + "\""+                      m_strVersion            + "\"," + "\r\n";
    if( m_strApkVersion.IsEmpty()  == FALSE ) strJson += strTab + "    \"apk_version\"       : " + "\""+                      m_strApkVersion         + "\"," + "\r\n";  
    if( m_bDownloadComplete        == TRUE  ) strJson += strTab + "    \"download_complete\" : " +       CUtil2::Bool2Str(    m_bDownloadComplete )   + "  ," + "\r\n";
    if( m_bUnzipComplete           == TRUE  ) strJson += strTab + "    \"unzip_complete\"    : " +       CUtil2::Bool2Str(    m_bUnzipComplete )      + "  ," + "\r\n";

    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        strJson += strTab + "    \"list\"                    : "                                                                                              + "\r\n";
        strJson += strTab + "    [" +                                                                                                                         + "\r\n";

        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
	            CString strItemJson = pItem->ToJson( strTab + _T("        ") );
                strJson += strItemJson;
                if( i == m_paUpgradeItem->GetCount() - 1 ) strJson += strTab + _T("        ")                                                                 + "\r\n";
                else                                       strJson += strTab + _T("        ,")                                                                + "\r\n";
            } 
        }
        strJson += strTab + "    ]" +                                                                                                                         + "\r\n";
    } else {
        strJson += strTab + "    \"size\"              : " + CUtil2::Int2Str(  m_nSize )                                                                      + "\r\n";
    }
    strJson += strTab +  "}" +                                                                                                                                + "\r\n";

    return strJson;
}

