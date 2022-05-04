////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLTreeView.h"
#include "macroXML.h"
#include "UtilXML.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CXMLTreeView, CMyTreeView)

BEGIN_MESSAGE_MAP(CXMLTreeView, CMyTreeView)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CXMLTreeView::CXMLTreeView()
{
	m_nIndex			= 0;
	m_hSettingsItem		= NULL;
}

CXMLTreeView::~CXMLTreeView()
{
}

////////////////////////////////////////////////////////////////////////////////
// debug & diagnostics
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CXMLTreeView::AssertValid() const
{
	CMyTreeView::AssertValid();
}

void CXMLTreeView::Dump(CDumpContext& dc) const
{
	CMyTreeView::Dump(dc);
}
#endif //_DEBUG


////////////////////////////////////////////////////////////////////////////////
// Tree -> XML
////////////////////////////////////////////////////////////////////////////////

BOOL CXMLTreeView::Load( CString strXMLFilePath )
{
	if( CUtilFile::IsExistPath( strXMLFilePath  ) == TRUE ) {
		CString strXML = CUtilFile::ReadString( strXMLFilePath );
		XML2Tree( strXML );
		return TRUE;
	}
	else {
		AfxMessageBox( "No File : " + strXMLFilePath );
		return FALSE;
	}
}

BOOL CXMLTreeView::XML2Tree( CString strXML )
{
	BOOL bSuccess = FALSE;

	BEGIN_XML_PARSER( strXML )
	{
		// 0. init
		m_nIndex = 0;

		// 1. travel  
		TravelDOM( pEle, NULL );

		// 2. expand
		ExpandChildItem( 2 );

		//3. return value
		bSuccess = TRUE;
	}
	END_XML_PARSER( TRUE )

	return bSuccess;
}

void CXMLTreeView::TravelDOM( IXMLDOMElement* pEle, const HTREEITEM &hParent )
{
	BEGIN_XML_TRAVEL( pEle, pEleChild )
		HTREEITEM hChild = Setting( pEleChild, hParent );
	MIDDLE_XML_TRAVEL( pEleChild, hParent );
		TravelDOM( (IXMLDOMElement*)pEleChild, hChild );
	END_XML_TRAVEL( FALSE )
}

int CXMLTreeView::IconString2IconIndex( CString strIcon )
{
	#define TREE_ICON_MOBIGO         0
	#define TREE_ICON_BUG            1
	#define TREE_ICON_CHECKBOX_GRAY  2
	#define TREE_ICON_CHECKBOX_OFF   3
	#define TREE_ICON_CHECKBOX_ON    4
	#define TREE_ICON_CLEAR          5
	#define TREE_ICON_DICE_1         6
	#define TREE_ICON_DICE_2         7
	#define TREE_ICON_DICE_3         8
	#define TREE_ICON_DICE_4         9
	#define TREE_ICON_DICE_5         10
	#define TREE_ICON_DICE_6         11
	#define TREE_ICON_FOLDER         12
	#define TREE_ICON_GREEN_BALL     13
	#define TREE_ICON_GRAY_BALL      14
	#define TREE_ICON_LG             15
	#define TREE_ICON_PATTERN        16
	#define TREE_ICON_SAMSUNG        17
	#define TREE_ICON_SETTING        18
	#define TREE_ICON_STOP           19
	#define TREE_ICON_TEST_TUBE      20
	#define TREE_ICON_TRANSFER       21
	#define TREE_ICON_HTM            22
	#define TREE_ICON_JAVA           23
	#define TREE_ICON_FEATURE_PHONE  24
	#define TREE_ICON_SMART_PHONE    25
	#define TREE_ICON_MESSAGEBOX     26
	#define TREE_ICON_BUILD          27
    #define TREE_ICON_LOGROOT        28 
    #define TREE_ICON_LOGFILE        29
    #define TREE_ICON_UPGRADE        30
    #define TREE_ICON_WATCH          31
    #define TREE_ICON_SAVE           32
    #define TREE_ICON_PORT           33
    #define TREE_ICON_USB            34
    #define TREE_ICON_MOUSE          35
    #define TREE_ICON_MONITOR        36
    #define TREE_ICON_WOWMAN         37
    #define TREE_ICON_SEARCH         38
    #define TREE_ICON_MAN            39
    #define TREE_ICON_PERCENT        40
    #define TREE_ICON_PLUS           41
    #define TREE_ICON_MINUS          42
	#define TREE_ICON_UNKNOWN        43

	if (strIcon == "Mobigo")		return TREE_ICON_MOBIGO;
	if (strIcon == "Bug")			return TREE_ICON_BUG;
	if (strIcon == "CheckBoxGray")	return TREE_ICON_CHECKBOX_GRAY;
	if (strIcon == "CheckBoxOff")	return TREE_ICON_CHECKBOX_OFF;
	if (strIcon == "CheckBoxOn")	return TREE_ICON_CHECKBOX_ON;
	if (strIcon == "Clear")			return TREE_ICON_CLEAR;
	if (strIcon == "Dice_1")		return TREE_ICON_DICE_1;
	if (strIcon == "Dice_2")		return TREE_ICON_DICE_2;
	if (strIcon == "Dice_3")		return TREE_ICON_DICE_3;
	if (strIcon == "Dice_4")		return TREE_ICON_DICE_4;
	if (strIcon == "Dice_5")		return TREE_ICON_DICE_5;
	if (strIcon == "Dice_6")		return TREE_ICON_DICE_6;
	if (strIcon == "Folder")		return TREE_ICON_FOLDER;
	if (strIcon == "GreenBall")		return TREE_ICON_GREEN_BALL;
	if (strIcon == "GrayBall")		return TREE_ICON_GRAY_BALL;
	if (strIcon == "LG")			return TREE_ICON_LG;
	if (strIcon == "Pattern")		return TREE_ICON_PATTERN;
	if (strIcon == "Samsung")		return TREE_ICON_SAMSUNG;
	if (strIcon == "Setting")		return TREE_ICON_SETTING;
	if (strIcon == "Stop")			return TREE_ICON_STOP;
	if (strIcon == "TestTube")		return TREE_ICON_TEST_TUBE;
	if (strIcon == "Transfer")		return TREE_ICON_TRANSFER;
	if (strIcon == "Htm")		    return TREE_ICON_HTM;
	if (strIcon == "Java")		    return TREE_ICON_JAVA;
	if (strIcon == "FeaturePhone")	return TREE_ICON_FEATURE_PHONE;
	if (strIcon == "SmartPhone")	return TREE_ICON_SMART_PHONE;
	if (strIcon == "MessageBox")	return TREE_ICON_MESSAGEBOX;
	if (strIcon == "Build")	        return TREE_ICON_BUILD;
    if (strIcon == "LogRoot")	    return TREE_ICON_LOGROOT;
    if (strIcon == "LogFile")       return TREE_ICON_LOGFILE;
    if (strIcon == "Upgrade")       return TREE_ICON_UPGRADE;
    if (strIcon == "Watch")         return TREE_ICON_WATCH;
    if (strIcon == "Save")          return TREE_ICON_SAVE;
    if (strIcon == "Port")          return TREE_ICON_PORT;
    if (strIcon == "USB")           return TREE_ICON_USB;
    if (strIcon == "Mouse")         return TREE_ICON_MOUSE;
    if (strIcon == "Monitor")       return TREE_ICON_MONITOR;
    if (strIcon == "Woman")         return TREE_ICON_WOWMAN;
    if (strIcon == "Search")        return TREE_ICON_SEARCH;
    if (strIcon == "Man")           return TREE_ICON_MAN;
    if (strIcon == "Percent")       return TREE_ICON_PERCENT;
    if (strIcon == "Plus")          return TREE_ICON_PLUS;
    if (strIcon == "Minus")         return TREE_ICON_MINUS;

	return TREE_ICON_UNKNOWN;
}

HTREEITEM CXMLTreeView::Setting( IXMLDOMElement* pEle, const HTREEITEM &hParent )
{
	HTREEITEM	hItem		= NULL;
	CString		strTagName	= CUtilXML::GetTagName( pEle );
	CString		strID       = CUtilXML::GetID(      pEle );
	CString		strIcon     = CUtilXML::GetIcon(    pEle );
	CString		strValue    = CUtilXML::GetValue(   pEle );
	int         nIconIndex = IconString2IconIndex(strIcon);

    // value -> icon 
    if( nIconIndex == TREE_ICON_UNKNOWN ) {
        nIconIndex = Value2Icon( strValue );    
    }

    // setting item 
    if(		strTagName == "Root" ) {
		;
	}
	else if(strTagName == "Folder" ) {
		hItem = InsertItem(hParent, pEle, nIconIndex);
	}
	else if(strTagName == "Item" ) {
		hItem = InsertItem(hParent, pEle, nIconIndex);
	}
	else {
		ERROR_LOG( "++   [ERROR] Unknown Tag Name = " + strTagName );
	}

	return hItem;
}

int CXMLTreeView::Value2Icon( CString strValue )
{
    if( strValue.CompareNoCase( "On"  ) == 0 ) return TREE_ICON_GREEN_BALL;
    if( strValue.CompareNoCase( "Off" ) == 0 ) return TREE_ICON_GRAY_BALL;

    return TREE_ICON_UNKNOWN;
}

HTREEITEM CXMLTreeView::InsertItem( const HTREEITEM		&hParent, 
								    IXMLDOMElement		*pEle,
									int					nTreeIcon )
{
	// 0. value
	CString strName			= CUtilXML::GetName( pEle );
	CString strID 			= CUtilXML::GetID(   pEle );

	// 1. init
	TV_INSERTSTRUCT tvstruct;

	// 2. common setting
	tvstruct.hParent				= ( hParent == NULL ) ? TVI_ROOT : hParent;
	tvstruct.hInsertAfter			= TVI_LAST;;// TVI_SORT;
	tvstruct.item.iImage			= nTreeIcon;
	tvstruct.item.iSelectedImage	= nTreeIcon;
	tvstruct.item.pszText			= (char*)(LPCTSTR)strName;
	tvstruct.item.mask				= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_HANDLE;
	tvstruct.item.stateMask			= TVIS_STATEIMAGEMASK ;

    // 3. build time
    if( strID == "ZOOM_RATIO" ) {
        CString strZoomRatio = CUtilXML::GetValue(pEle);
        strName += "=" + strZoomRatio + "%";
        tvstruct.item.pszText = (char*)(LPCTSTR)strName;
        DEBUG_LOG( "++   BUILD_TIME=%s", tvstruct.item.pszText );
    } else if( strID == "BUILD_TIME" ) {
        strName.Format("Build Time : %s", BUILD_TIME() ); 
        tvstruct.item.pszText = (char*)(LPCTSTR)strName;
        DEBUG_LOG( "++   BUILD_TIME=%s", tvstruct.item.pszText );
    }else if ( strID.Find("LOG_") > -1){
        CFileFind finder;
		std::vector<CString> listFile;        
        CString strLogPath;
        int nFileCount = 0;

        if      (strID == "LOG_DEBUG")    strLogPath = CString( DEBUG_GET_DEBUG_PATH()   ) + "\\*.c";
        else if (strID == "LOG_ERROR")    strLogPath = CString( DEBUG_GET_ERROR_PATH()   ) + "\\*.c";
        else if (strID == "LOG_LIGHT")    strLogPath = CString( DEBUG_GET_LIGHT_PATH()   ) + "\\*.c";
        else if (strID == "LOG_WARNING")  strLogPath = CString( DEBUG_GET_WARNING_PATH() ) + "\\*.c";
        else {
            ERROR_LOG( "++   strID=" + strID );
        }

        BOOL bWorking = finder.FindFile(  strLogPath );
        while (bWorking) {
            bWorking = finder.FindNextFile();
            listFile.push_back(finder.GetFileName());
        }

        nFileCount = listFile.size();
        CString str;
        str.Format("%s (%d개)", strName, nFileCount);
        tvstruct.item.pszText = (char*)(LPCTSTR)(str);
        
        HTREEITEM hItem = GetTreeCtrl().InsertItem(&tvstruct);
        GetTreeCtrl().SetItemData(hItem, (DWORD_PTR)pEle);
        
        for (int nCnt = nFileCount-1; nCnt >= 0 ; nCnt--) {
            HTREEITEM hItem2 = GetTreeCtrl().InsertItem(listFile.at(nCnt).GetString(), TREE_ICON_LOGFILE, TREE_ICON_LOGFILE, hItem);
        }
        
        return hItem;
    }
	// 4. Insert Item
	HTREEITEM hItem = GetTreeCtrl().InsertItem(&tvstruct);

	// 5. save xml node
	GetTreeCtrl().SetItemData(hItem, (DWORD_PTR)pEle );

	return hItem;
}

////////////////////////////////////////////////////////////////////////////////
// Tree -> XML
////////////////////////////////////////////////////////////////////////////////

CString CXMLTreeView::Tree2XML()
{
//	DEBUG_EX( "CXMLTreeView::Tree2XML()" );

	CString			strXML		= "";
	CTreeCtrl&      treeCtrl    = this->GetTreeCtrl();
	HTREEITEM		hRoot		= treeCtrl.GetRootItem();
	IXMLDOMElement	*pEle		= (IXMLDOMElement*)treeCtrl.GetItemData( hRoot );	
	CString			strTagName	= CUtilXML::GetTagName( pEle);

	strXML       = "<?xml version=\"1.0\" encoding=\"euc-kr\" ?>\r\n"; 
	strXML      += "<Root>\r\n"; 
	strXML		+= "    <" + strTagName + CUtilXML::Attr2Str( pEle ) + ">\r\n";
	strXML      +=          TravelTree( hRoot, 2 );
	strXML		+= "    </" + strTagName + ">\r\n";
	strXML      += "</Root>\r\n";	

	return strXML;
}

CString CXMLTreeView::TravelTree( HTREEITEM hParent, int nDepth )
{
//	DEBUG_EX( "CXMLTreeView::TravelTree()" );
	CTreeCtrl&      treeCtrl    = this->GetTreeCtrl();
	int			    nCountChild = 0;
	CString		    strXML		= "";

    if( hParent == NULL) return "";
	// tab
    CString strTab = "";
	for( int i = 0; i < nDepth; i++ ) strTab += "    ";

    HTREEITEM	hChild = treeCtrl.GetNextItem(hParent, TVGN_CHILD);

    while(hChild) {
		// 0. ++
		nCountChild++;
		
		// 1. get 
		IXMLDOMElement*	pEle	= (IXMLDOMElement*)treeCtrl.GetItemData( hChild );
		CString strTagName = CUtilXML::GetTagName( pEle);

		// 3. xml
        strXML +=  strTab;
		strXML += "<" + strTagName;
		strXML +=	CUtilXML::Attr2Str( pEle );
		strXML += ">";
		
        if( strTagName == "Folder" ) {
            strXML += "\r\n";
        }

		// 4. recursive
		strXML += TravelTree( hChild, nDepth + 1);

        if( strTagName == "Folder" ) {
	        strXML += strTab;
        }

		// 5. end tag
		strXML += "</" + strTagName + ">\r\n";

        

		// 6. next
        hChild = treeCtrl.GetNextItem(hChild, TVGN_NEXT);
    }

	// 7. Program Menu에서 ActiveX 폴더 추가시, 펼쳐지지 않은 Item삭제되는 현상
	//	-> 이유 : Tree XML 로딩후, 모든 XML element를 Tree Item으로 만들지는 않는다( 모두 만들면 속도가 느리다 )
	//     따라서 Item로 만들어 지지 않은 element는 Tree가 저장될 시, XML로 재 탄생하지 않는다.
	//	-> 임시 대책 : XML을 만들기 위해서 Tree를 여행하다가 Leaf를 만나면, xml tree로 함께 검사한다.
	//	-> 근본적 대책 : 변경된 Item만 저장하도록 수정되어야 한다( 어럽겠지....음..--;;)
	if(	   nCountChild							== 0 ) {
		IXMLDOMElement*	pEleParent	= (IXMLDOMElement*)treeCtrl.GetItemData( hParent );
		if(	   pEleParent                           != NULL 
			&& CUtilXML::IsExistChild( pEleParent ) == TRUE ) {
			strXML = CUtilXML::Ele2XML( pEleParent, 0 );
		}
	}

    return strXML;
}


////////////////////////////////////////////////////////////////////////////////
// Utility
////////////////////////////////////////////////////////////////////////////////

BOOL CXMLTreeView::IsOn( CString strID )
{
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();
    HTREEITEM hItem = FindTreeItem( treeCtrl.GetRootItem(), strID );    
    if( hItem != NULL ) {
        IXMLDOMElement *pEle = (IXMLDOMElement*)GetTreeCtrl().GetItemData(hItem);
        CString strValue = CUtilXML::GetValue( pEle );
        if( strValue == "On" ) {
            return TRUE;
        }
    }

    return FALSE;
}

HTREEITEM CXMLTreeView::FindTreeItem( HTREEITEM hItem, CString strID )
{
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();

    IXMLDOMElement *pEle = (IXMLDOMElement*)treeCtrl.GetItemData(hItem);

    CString strTempTagName = CUtilXML::GetTagName( pEle );
    CString strTempName    = CUtilXML::GetName(    pEle );
    CString strTempID      = CUtilXML::GetID(      pEle );

    if( strTempID == strID ) {
        return hItem;           
    }

    // iterate
    HTREEITEM hItemChild = treeCtrl.GetChildItem( hItem );
	while( hItemChild ) {
		if( hItemChild != NULL) {
            HTREEITEM hItemResult = FindTreeItem( hItemChild, strID );
            if( hItemResult != NULL ) {
                return hItemResult;            
            }
        }
        hItemChild = treeCtrl.GetNextItem( hItemChild, TVGN_NEXT );
	}
    
    return NULL;
}

CString CXMLTreeView::GetString( CString strID )
{
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();
    HTREEITEM hItem = FindTreeItem( treeCtrl.GetRootItem(), strID );    
    if( hItem != NULL ) {
        IXMLDOMElement *pEle = (IXMLDOMElement*)GetTreeCtrl().GetItemData(hItem);
        CString strValue = CUtilXML::GetValue( pEle );
        return  strValue;
    }

    return "";
}
