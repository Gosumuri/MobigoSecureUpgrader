#include "StdAfx.h"
#include "util.h"
#include <msxml.h>
#include <comdef.h>

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// etc

CString operator+( char* szFirst, CString strSecond )
{
	CString strResult;

	strResult  = szFirst; 
	strResult += strSecond;

	return strResult;
}

CString CUtil::Random( int nLength )
{
	CString		strRandom = ""; 
	CString		strRandomSeed = "123456789HICDEXYZKL3RSTUVFGPQMNOB";
	srand( (unsigned)time(NULL));

	for (int i = 0; i < nLength; i++) {
		int n = abs( rand() )%strRandomSeed.GetLength();

		strRandom += strRandomSeed.Mid( n, 1);
    }

	return strRandom;
}

CString CUtil::Normal( CString str )
{
	CString strNormal	= "";
	int		nLength		= str.GetLength();

	for (int i = 0; i < nLength; i++) {
		char ch = str.GetAt(i);
		switch (ch) {
			case '<': {
				strNormal += "&lt;";
			}
			break;						  
			case '>': {
				strNormal += "&gt;";
			}
			break;
			case '&': {
				if(    str.Mid( i, 4 ).CompareNoCase( "&lt;" )   == 0 
                    || str.Mid( i, 4 ).CompareNoCase( "&gt;" )   == 0 
					|| str.Mid( i, 5 ).CompareNoCase( "&amp;" )  == 0 
					|| str.Mid( i, 6 ).CompareNoCase( "&quot;" ) == 0 
					|| str.Mid( i, 6 ).CompareNoCase( "&apos;" ) == 0 )
					strNormal += ch;
				else 
					strNormal += "&amp;";
			}
			break;
			case '"': {
				strNormal += "&quot;";
		    }
			break;
			case '\'': {
				strNormal += "&apos;";
			}
			break;
			default: {
				strNormal += ch;
			}
		}
	}

	return strNormal;
}

//////////////////////////////////////////////////////////////////////////////////////////
// etc

int CUtil::Round( float f)
{
	return int(f + 0.5);
}

// <XXX>                        <XXX> 
//		<!--...-->      => 
// </XXX>                       </XXX>

CString CUtil::RemoveComment( CString strXML )
{
	int nPrePos = 0; 
	while( TRUE ) {
		int n1 = strXML.Find( "<!--",  nPrePos );
		if( n1 < 0 )
			break;

		int n2 = strXML.Find( "-->",   n1 );
		strXML.Delete( n1, ( n2 + 3 ) - n1 );
		nPrePos = n1;
	}

	return strXML;
}

// example : ch = '0', strPost = "123", nPower = 6
//           strResult = "000123"
CString CUtil::PowerChar( char ch, CString strPost, int nPower )
{
	CString strResult = strPost;
		
	int nRest = nPower - strResult.GetLength();
		
	CString strPre = "";
	for( int i = 0; i < nRest; i++ ) {
		strPre += ch;
	}
		
	return strPre + strResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread

void CUtil::TerminateThread( HANDLE hThread )
{
	if( hThread != NULL ) {
		DWORD dwExitCode;
		::GetExitCodeThread( hThread, &dwExitCode);
		if(dwExitCode == STILL_ACTIVE) {
			::TerminateThread( hThread, 0);
		}
	}
}