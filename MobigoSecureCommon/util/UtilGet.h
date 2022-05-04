#pragma once

class AFX_EXT_CLASS CUtilGet
{
public:
	CUtilGet(void);
	~CUtilGet(void);
	
	static CString			GetModulePath();

	static int				GetComboIndex( CString str, CString strList );
	static CString			GetComboString( int nIndex, CString strList );
	static BOOL 			SearchCmbItem(int value, CString strCmbItems, CString* strCmbItemResult );
	static int				SearchCmbItemIdx(int value, CString strCmbItems);
	static CString			SearchCmbItemString(int combo_idx, CString strCmbItems);
	static int				SearchCmbItemValue(int combo_idx, CString strCmbItems);
private :
	static CString			m_strModulePath;
};
