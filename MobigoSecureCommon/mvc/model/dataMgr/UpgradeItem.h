#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "MyPtrArray.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CUpgradeItem 
{
public:
	CUpgradeItem();   // protected constructor used by dynamic creation
	virtual ~CUpgradeItem();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected:
    CString        m_strID; 
    CString        m_strDownloadUrl; 
    CString        m_strSavePath; 
    int            m_nSize;
    BOOL           m_bEncyption;
    BOOL           m_bMandatory;
    CString        m_strVersion;
    CString        m_strApkVersion;
    CString        m_strIcon;
    CString        m_strCRC;

    CMyPtrArray*   m_paUpgradeItem;

protected :
    BOOL           m_bUpgrade;
    BOOL           m_bDownloadComplete;
    BOOL           m_bUnzipComplete;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
    CString         GetID()                                          { return m_strID;                          };
    void            SetID( CString strID )                           { m_strID = strID;                         };
    CString         GetDownloadUrl()                                 { return m_strDownloadUrl;                 };
    void            SetDownloadUrl( CString strDownloadUrl )         { m_strDownloadUrl = strDownloadUrl;       };
    CString         GetSavePath()                                    { return m_strSavePath;                    };
    void            SetSavePath( CString strSavePath )               { m_strSavePath = strSavePath;             };
    int             GetSize()                                        { return m_nSize;                          };
    void            SetSize( int nSize )                             { m_nSize = nSize;                         };
    BOOL            IsEncryption()                                   { return m_bEncyption;                     };
    void            SetEncryption( BOOL bEncryption )                { m_bEncyption = bEncryption;              };
    BOOL            IsMandatory()                                    { return m_bMandatory;                     };
    void            SetMandatory( BOOL bMandatory )                  { m_bMandatory = bMandatory;               };
    CString         GetVersion()                                     { return m_strVersion;                     };
    void            SetVersion( CString strVersion )                 { m_strVersion = strVersion;               };
    CString         GetApkVersion()                                  { return m_strApkVersion;                  };
    void            SetApkVersion( CString strApkVersion )           { m_strApkVersion = strApkVersion;         };
    CString         GetIcon()                                        { return m_strIcon;                        };
    void            SetIcon( CString strIcon )                       { m_strIcon = strIcon;                     };
    CString         GetCRC()                                         { return m_strCRC;                         };
    void            SetCRC( CString strCRC )                         { m_strCRC = strCRC;                       };

    CMyPtrArray*    GetUpgradeItemList()                             { return m_paUpgradeItem;                  };
    void            SetUpgradeItemList( CMyPtrArray* paUpgradeItem ) { m_paUpgradeItem = paUpgradeItem;         };

    BOOL            IsUpgrade()                                      { return m_bUpgrade;                       };
    void            SetUpgrade( BOOL bUpgrade )                      { m_bUpgrade = bUpgrade;                   };
    BOOL            IsDownloadComplete()                             { return m_bDownloadComplete;              };
    void            SetDownloadComplete( BOOL bDownloadComplete )    { m_bDownloadComplete = bDownloadComplete; };
    BOOL            IsUnzipComplete()                                { return m_bUnzipComplete;                 };
    void            SetUnzipComplete( BOOL bUnzipComplete )          { m_bUnzipComplete = bUnzipComplete;       };
    BOOL            IsZipFile();
    BOOL            IsApkFile();
    CString         GetUnzipPath();
    CString         GetUnapkPath();
// add
public :
    void            AddUpgradeItem( CUpgradeItem *pItem )            { m_paUpgradeItem->Add(pItem);             };

// public :
public :
    int             CalcTotalSize();
    int             CalcDownloadSize();
    void            FillUrl2PathMap( CMapStringToString* pMapUrl2Path );
    void            FillZip2DirMap( CMapStringToString* pMapZip2Dir );
    void            FillSystemApk2DirMap( CMapStringToString* pMapApk2Dir, CString strSystemApkItemID, BOOL bSystemApk );
    void            FillSystemApk2IDMap( CMapStringToString* pMapApk2ID, CString strSystemApkItemID, BOOL bSystemApk );
    void            FillID2PtrMap( CMapStringToPtr* pMapID2Ptr );
    CUpgradeItem*   FindUpgradeItemByApkFileName( CString strApkFileName );
    CUpgradeItem*   FindUpgradeItemByID( CString strID );
    CUpgradeItem*   FindUpgradeItemByDownloadUrl( CString strID );
    CUpgradeItem*   FindUpgradeItemBySavePath( CString strSavePath );
    CString         ReadStringByID( CString strID );
    int             CompareVersion( CUpgradeItem *pUpgradeItem );
// dump/toJson
public:
    void            Dump( CString strTab );
    CString         ToJson( CString strTab );

};