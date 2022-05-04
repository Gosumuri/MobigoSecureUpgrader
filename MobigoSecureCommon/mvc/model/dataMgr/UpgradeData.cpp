
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpgradeData.h"
#include "Util.h"
#include "UtilFile.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUpgradeData::CUpgradeData() : CUpgradeItem() 
{
	//-DEBUG_LOG(">> CUpgradeData::CUpgradeData()");

    m_strMakeTime  = "";
    m_pMapUrl2Path = NULL;
}

CUpgradeData::~CUpgradeData() 
{ 
	//-DEBUG_LOG(">> CUpgradeData::~CUpgradeData()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CUpgradeData::Init()
{
	//-DEBUG_LOG(">> CUpgradeData::Init()");
    CUpgradeItem::Init();
}

void CUpgradeData::Exit()
{
	//-DEBUG_LOG(">> CUpgradeData::Exit()");
    CUpgradeItem::Exit();
}

////////////////////////////////////////////////////////////////////////////////
// public funtion
///////////////////////////////////////////////////////////////////////////////

// 모든 파일의 size
int CUpgradeData::CalcTotalSize()
{
	//-DEBUG_LOG(">> CUpgradeData::CalcTotalSize()");

    int nTotalSize = 0;

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

// download되어야할 파일들의 모든 size 
int CUpgradeData::CalcDownloadSize()
{
	//-DEBUG_LOG(">> CUpgradeData::CalcDownloadSize()");

    int nTotalSize = 0;

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

void CUpgradeData::FillUrl2PathMap( CMapStringToString* pMapUrl2Path )
{
	//-DEBUG_LOG(">> CUpgradeData::FillUrl2PathMap()");

    // generate <url,path> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillUrl2PathMap( pMapUrl2Path );
            } 
        }
    }
}

void CUpgradeData::FillZip2DirMap( CMapStringToString* pMapZip2Dir )
{
	//-DEBUG_LOG(">> CUpgradeData::FillZip2DirMap()");

    // generate <zip, dir> map 
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
void CUpgradeData::FillSystemApk2DirMap( CMapStringToString* pMapApk2Dir, CString strSystemApkItemID )
{
	//-DEBUG_LOG(">> CUpgradeData::FillSystemApk2DirMap()");

    // generate <apk, dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillSystemApk2DirMap( pMapApk2Dir, strSystemApkItemID, FALSE );
            } 
        }
    }
}

void CUpgradeData::FillSystemApk2IDMap( CMapStringToString* pMapApk2ID, CString strSystemApkItemID )
{
	//-DEBUG_LOG(">> CUpgradeData::FillSystemApk2IDMap()");

    // generate <apk, dir> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillSystemApk2IDMap( pMapApk2ID, strSystemApkItemID, FALSE );
            } 
        }
    }
}

void CUpgradeData::FillID2PtrMap( CMapStringToPtr* pMapID2Ptr )
{
	//-DEBUG_LOG(">> CUpgradeData::FillID2PtrMap()");

    // generate <url,path> map 
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->FillID2PtrMap( pMapID2Ptr );
            } 
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// private function
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// dump funtion
///////////////////////////////////////////////////////////////////////////////

void CUpgradeData::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CUpgradeData::Dump()");
	//-DEBUG_LOG("++   %sm_strMakeTime    = %s", (LPCSTR)strTab, (LPCSTR)m_strMakeTime    );
    CUpgradeItem::Dump( strTab );
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CUpgradeData::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CUpgradeData::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                     + "\r\n";
    strJson += strTab + "    \"make_time\"               : " + "\""+                   m_strMakeTime    + "\"," + "\r\n";
    strJson += strTab + "    \"id\"                      : " + "\""+                   m_strID          + "\"," + "\r\n";
    if( m_paUpgradeItem != NULL && 0 < m_paUpgradeItem->GetCount() ) {    
        strJson += strTab + "    \"list\"                    : " +                                              + "\r\n";
        strJson += strTab + "    [" +                                                                           + "\r\n";

        for( int i = 0; i < m_paUpgradeItem->GetCount() ; i++ ) {
            CUpgradeItem *pItem = (CUpgradeItem*)m_paUpgradeItem->GetAt( i );
            if( pItem != NULL ) {
	            CString strItemJson = pItem->ToJson( strTab + _T("        ") );
                strJson += strItemJson;
                if( i == m_paUpgradeItem->GetCount() - 1 ) strJson += strTab + _T("        ")                   + "\r\n";
                else                                       strJson += strTab + _T("       ,")                   + "\r\n";
            } 
        }
        strJson += strTab + "    ]" +                                                                           + "\r\n";
    }
    strJson += strTab + "}" +                                                                                   + "\r\n";

    return strJson;
}


