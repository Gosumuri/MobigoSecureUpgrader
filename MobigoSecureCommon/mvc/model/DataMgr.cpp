
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataMgr.h"
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

CDataMgr::CDataMgr() 
{
	//-DEBUG_LOG(">> CDataMgr::CDataMgr()");

    m_pUpgradeData = NULL;
    m_pPolicyData  = NULL;

    m_pUpgradeDataLoader = NULL;
    m_pPolicyDataLoader  = NULL;
}

CDataMgr::~CDataMgr() 
{ 
	//-DEBUG_LOG(">> CDataMgr::~CDataMgr()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CDataMgr::Init()
{
	//-DEBUG_LOG(">> CDataMgr::Init()");

    // loader 
    if( m_pUpgradeDataLoader == NULL ) {
        m_pUpgradeDataLoader = new CUpgradeDataLoader();
        m_pUpgradeDataLoader->Init();
    }

    if( m_pPolicyDataLoader == NULL ) {
        m_pPolicyDataLoader = new CPolicyDataLoader();
        m_pPolicyDataLoader->Init();
    }

#if 0 // loader에 의해서 생성되므로, 여기서 생성할 필요 없다. 
    if( m_pUpgradeData == NULL ) {
        m_pUpgradeData = new CUpgradeData();
        m_pUpgradeData->Init();
    }

    if( m_pPolicyData == NULL ) {
        m_pPolicyData = new CPolicyData();
        m_pPolicyData->Init();
    }
#endif
}

void CDataMgr::Exit()
{
	//-DEBUG_LOG(">> CDataMgr::Exit()");

    // loader 
    if( m_pUpgradeDataLoader != NULL ) {
        m_pUpgradeDataLoader->Exit();
        delete m_pUpgradeDataLoader;
        m_pUpgradeDataLoader = NULL;
    }

    if( m_pPolicyDataLoader != NULL ) {
        m_pPolicyDataLoader->Exit();
        delete m_pPolicyDataLoader;
        m_pPolicyDataLoader = NULL;
    }

    // data 
    if( m_pUpgradeData != NULL ) {
        m_pUpgradeData->Exit();
        delete m_pUpgradeData;
        m_pUpgradeData = NULL;
    }

    if( m_pPolicyData != NULL ) {
        m_pPolicyData->Exit();
        delete m_pPolicyData;
        m_pPolicyData = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// save
///////////////////////////////////////////////////////////////////////////////

void CDataMgr::SaveUpgradeData( CString strUpgradeListPath )
{
    m_pUpgradeData->SetDownloadComplete( TRUE );
    CString strModifyUpgradeListJson = m_pUpgradeData->ToJson( "" );
    CUtilFile::WriteString( strUpgradeListPath, strModifyUpgradeListJson );
}

////////////////////////////////////////////////////////////////////////////////
// publci 
///////////////////////////////////////////////////////////////////////////////

CString CDataMgr::ToJson()
{
    CString strModifyUpgradeListJson = m_pUpgradeData->ToJson( "" );
    return strModifyUpgradeListJson;
}

BOOL CDataMgr::IsValid()
{
    if( m_pUpgradeData == NULL ) {
        return FALSE;
    } else {
        CString strMakeTime = m_pUpgradeData->GetMakeTime();
        if( strMakeTime.IsEmpty() == TRUE ) {
            return FALSE;
        }
    }
    return TRUE;
}
