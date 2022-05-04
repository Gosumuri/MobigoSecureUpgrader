#include "StdAfx.h"
#include "utilget.h"
#include "util2.h"

CString CUtilGet::m_strModulePath = "";

CUtilGet::CUtilGet(void)
{
}

CUtilGet::~CUtilGet(void)
{
}

CString CUtilGet::GetModulePath()
{
	if( CUtilGet::m_strModulePath.IsEmpty() == TRUE ) { 
		char szFullPath[MAX_PATH];
		::GetModuleFileName(NULL, szFullPath, MAX_PATH);

		CString strExePath( szFullPath );
		int nIndex = strExePath.ReverseFind( '\\' );

		strExePath = strExePath.Mid( 0, nIndex ) + "\\";
	
		CUtilGet::m_strModulePath = strExePath;
	}

	return CUtilGet::m_strModulePath;
}

int CUtilGet::GetComboIndex( CString str, CString strList )
{
   // [KTFT-SMPARK] 2005-05-08 - Edit ���� ���� ��ĭ�� ���, No Group �� No Key Num ����.
   // �� �κ��� ������ �� �ʿ��ϴ�. Group ������ �ٲ���� ���� ��� �����ϳ� ?
  if ( str=="" ) return (-1);
  // end of SMPARK
  
	for( int i = 0; TRUE ; i++ ) {
		CString strValue;
		BOOL b = AfxExtractSubString( strValue, strList, i , '|' );
		if( b == TRUE ) {
			if( str == strValue ) {

#if 1 //yjy_ya13 
				return (i/2);
#else
				return i;
#endif //yjy_ya13
			}
		}
		else 
			break;
	}

	return -1;
}

CString CUtilGet::GetComboString( int nIndex, CString strList )
{
	CString strValue = "";

#if 1 //yjy_ya13
	BOOL b = AfxExtractSubString( strValue, strList, nIndex*2, '|' );
#else
	BOOL b = AfxExtractSubString( strValue, strList, nIndex, '|' );
#endif //yjy_ya13

	if( b == FALSE )
		return "";

	return strValue;
}

// yjy_ya13 begin - for checking setting value with Combo item value
BOOL CUtilGet::SearchCmbItem(int value, CString strCmbItems, CString* strCmbItemResult )
{
	CString strItemValue; 
	BOOL b = TRUE; // yjy_ya13 - return value for AfxExtractSubString()
	int cmbItemValue;

	for(int i=1; b; i=i+2) 
	{

		b = AfxExtractSubString(strItemValue, strCmbItems, i, '|');
		if (b)
		{
			cmbItemValue = CUtil2::Str2Int(strItemValue);
			if (cmbItemValue == value)
			{
				AfxExtractSubString(*strCmbItemResult, strCmbItems, i-1, '|' );
				break;
			}
		}
	}

	return b;
}

int CUtilGet::SearchCmbItemIdx(int value, CString strCmbItems)
{
	CString strItemValue; 
	BOOL b = TRUE; // yjy_ya13 - return value for AfxExtractSubString()
	int cmbItemValue;
	int iCmbIdx = 0;

	for(int i=1; b; i=i+2) 
	{

		b = AfxExtractSubString(strItemValue, strCmbItems, i, '|');
		if (b)
		{
			cmbItemValue = CUtil2::Str2Int(strItemValue);
			if (cmbItemValue == value)
			{
				iCmbIdx = (i-1)/2;
				break;
			}
		}
		else
		{
			AfxMessageBox("No Item Index");
		}
	}

	return iCmbIdx;
}

CString CUtilGet::SearchCmbItemString(int combo_idx, CString strCmbItems)
{
	CString strItemValue; 
	BOOL b = TRUE; // yjy_ya13 - return value for AfxExtractSubString()
	int iCmbValueIdx = combo_idx*2+1;

	b = AfxExtractSubString(strItemValue, strCmbItems, iCmbValueIdx, '|');

	if (b==FALSE)
	{
		AfxMessageBox("Invalid Combo index");
		AfxExtractSubString(strItemValue, strCmbItems, 1, '|'); //force to set first combo item value
	}

	return strItemValue;	
}

int CUtilGet::SearchCmbItemValue(int combo_idx, CString strCmbItems)
{
	CString strValue = "";
	int ret = 0;

	BOOL b = AfxExtractSubString( strValue, strCmbItems, combo_idx*2+1, '|' );

	if( b == FALSE )
		return -1;

	ret = atoi(strValue);
	return ret;
}
//yjy_ya13 end

