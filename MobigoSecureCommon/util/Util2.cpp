#include "StdAfx.h"
#include "util2.h"

CUtil2::CUtil2(void)
{
}

CUtil2::~CUtil2(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// primitive

CString CUtil2::Int2Str( int n )
{
	CString str;
	str.Format( "%d", n );
	return str;
}

CString CUtil2::Float2Str( float f )
{
	CString str;
	str.Format( "%f", f );
	return str;
}

CString CUtil2::Float2DecimalStr( float f )
{
	CString strTemp, str;
	strTemp.Format( "%f", f );
	str = strTemp.Mid( 0, strTemp.Find(".")+2 );
	return str;
}

CString CUtil2::Long2Str( long l )
{
	CString str;
	str.Format( "%ld", l );
	return str;
}

float CUtil2::Str2Float( CString str )
{
	float f = atof( (char*)(LPCTSTR)str );
	return f;
}

CString CUtil2::Bool2Str( BOOL b )
{
	CString str;
	
	if( b )
		str.Format( "%s", "true" );
	else 
		str.Format( "%s", "false" );
	
	return str;
}

CString CUtil2::Bool2Option( BOOL b )
{
	CString str;
	
	if( b )
		str.Format( "%s", "M" ); // mandatory 
	else 
		str.Format( "%s", "O" ); // Option
	
	return str;
}

BOOL CUtil2::Option2Bool( CString strOption )
{
	if( strOption.CompareNoCase( "M" ) == 0 )
		return TRUE;
	
	return FALSE;
}

int CUtil2::Str2Int( CString str )
{
	if(str.IsEmpty() )
		return (int)-1;

	return atoi( (char*)(LPCTSTR)str );
}

long CUtil2::Str2Long( CString str )
{
	if(str.IsEmpty() )
		return (long)-1;

	return atol( (char*)(LPCTSTR)str );
}

BOOL CUtil2::Str2Bool( CString str )
{
	if( !str.CompareNoCase("T") )
		return TRUE;
	return FALSE;
}

CRgn* CUtil2::Rect2Rgn( CRect &rect)
{
	CRgn* rgn = new CRgn;
	POINT pt[5];

	pt[0].x = rect.left; 
	pt[0].y = rect.top;

	pt[1].x = rect.right; 
	pt[1].y = rect.top;

	pt[2].x = rect.right; 
	pt[2].y = rect.bottom;

	pt[3].x = rect.left; 
	pt[3].y = rect.bottom;

	pt[4].x = rect.left; 
	pt[4].y = rect.top;

	rgn->CreatePolygonRgn(pt, 5, WINDING);

	return rgn;
}

// CRect -> [left, top, right, bottom ]
CString CUtil2::Rect2Str( CRect &rc )
{
	CString str;

	str.Format( "[%d, %d, %d, %d", rc.left, rc.top, rc.right, rc.bottom );

	return str;
}


// int -> Kilo
CString CUtil2::Int2K( DWORD dw )
{
	CString strK;  
	strK.Format("%sKB", CUtil2::Int2Money( ceil( (LONGLONG) dw / 1024.0 ) ) );
	return strK;
}

// int -> MEGA
CString CUtil2::Int2M( DWORD dw )
{
	CString strM;  
	strM.Format("%3.0fMB", (LONGLONG) dw / (1024*1024.0));
	return strM;
}

// int -> GIGA
CString CUtil2::Int2G( DWORD dw )
{
	CString strG;  
	strG.Format("%3.0fGB", (LONGLONG) dw / (1024.0*1024*1024));
	return strG;
}

// 10000000 -> 10,000,000
CString CUtil2::Int2Money( int n )
{
	CString str;
	str.Format( "%d", n );
	return CUtil2::Int2Money( str );
}

// 10000000 -> 10,000,000
CString CUtil2::Int2Money( CString strInt )
{
	static const int		SEP_INT		= 3;	// seperator interval 
	static const CString	SEP_CHAR	= ",";	// seperator charactor
    
	CString					strMoney;			// result

    for( int i = strInt.GetLength() - 1; 0 <= i ; i-- ) {
		strMoney.Insert( 0, strInt.Mid( i, 1 ) );

		if(    ( strInt.GetLength() - ( i + 1) )%SEP_INT == SEP_INT - 1 
			&& i != 0 )
			strMoney.Insert( 0, SEP_CHAR );
    }

	return strMoney;
}

long CUtil2::Time2Second( COleDateTimeSpan span )
{
	return   span.GetDays()  * 24 * 60 * 60
		   + span.GetHours()      * 60 * 60
		   + span.GetMinutes()         * 60
		   + span.GetSeconds();         
}


CString	CUtil2::Str2Star( CString str )
{
	CString strStar = "";
	for( int i = 0; i < str.GetLength(); i++ )
		strStar += "*";

	return strStar;
}

CStringArray* CUtil2::Str2StrArray( CString str )
{
	if( str.IsEmpty() == TRUE )
		return NULL;

	CString		   strTemp = str;
	CStringArray*   pAyStrList = new CStringArray();
	while ( strTemp.Find(",") != -1 ) {
		pAyStrList->Add( strTemp.Mid( 0, strTemp.Find(",") ) );
		strTemp.Delete(0, strTemp.Find(",") + 1 );
	}
	pAyStrList->Add( strTemp );

	return pAyStrList;
}

CString CUtil2::Point2Str( CPoint point )
{
	CString str;
	str.Format( "x = %d, y = %d", point.x, point.y );
	return str;
}

int	CUtil2::COMStr2COMNum( CString strCOM )
{
	if( strCOM.IsEmpty() == FALSE ) {
		CString strCOMNum = strCOM.Mid( 3 ); // "11" <- "COM 11"
		strCOMNum.Trim();
		int nCOMNum = CUtil2::Str2Int( strCOMNum );

		return nCOMNum;
	}
	return -1;
}

void CUtil2::StrIP2ByteIP(       CString strIP, BYTE btAyIP[4] )
{
	CString strValue;
	BOOL	b;

	// 0. XXX.xxx.xxx.xxx
	b = AfxExtractSubString( strValue, strIP, 0, '.' ); 
	if( b == TRUE ) btAyIP[0] = (BYTE)CUtil2::Str2Int( strValue );
	else			btAyIP[0] = 0;

	// 1. xxx.XXX.xxx.xxx
	b = AfxExtractSubString( strValue, strIP, 1, '.' ); 
	if( b == TRUE ) btAyIP[1] = (BYTE)CUtil2::Str2Int( strValue );
	else			btAyIP[1] = 0;

	// 2. xxx.xxx.XXX.xxx
	b = AfxExtractSubString( strValue, strIP, 2, '.' ); 
	if( b == TRUE ) btAyIP[2] = (BYTE)CUtil2::Str2Int( strValue );
	else			btAyIP[2] = 0;

	// 2. xxx.xxx.xxx.XXX
	b = AfxExtractSubString( strValue, strIP, 3, '.' ); 
	if( b == TRUE ) btAyIP[3] = (BYTE)CUtil2::Str2Int( strValue );
	else			btAyIP[3] = 0;
}
// [KTFT-SMPARK] 2005-05-09 - Call Number Check! must digit, * or #
BOOL CUtil2::is_CallDigit_STR( char* str )
{
  //CString Call_String = "0123456789*#";
  byte len = 0;

  if (str=="" || str==NULL )  return FALSE;
  len = strlen(str);

  for ( byte i=0; i<len; i++)
  {
    if ( str[i]<'0' )           // ch<48
        if ( str[i]!='*')     // ch=42
          if ( str[i]!='#')   // ch=35
            return FALSE;
    if ( str[i]>'9')            // ch>57
      return FALSE;
  }
  return TRUE;
}
