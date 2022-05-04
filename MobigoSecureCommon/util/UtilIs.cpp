#include "StdAfx.h"
#include "utilis.h"

CUtilIs::CUtilIs(void)
{
}

CUtilIs::~CUtilIs(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	<?xml version=\"1.0\" encoding=\"ks_c_5601-1987\"?>
//	<Root>          
//		<Result     
//			Code  = "SUCCESS" 
//			Name  = "name"
//		>
//			reason
//		</Result>
//	</Root>
//
// -------------------------------------------------------------------------------------------------
//	Code Value : 
//		- SUCCESS
//		- WARNING
//		- ERROR

////////////////////////////////////////////////////////////////////////////////////////////////////
// Ip Check

BOOL CUtilIs::IsValidIP( CString strIP )
{
	return CUtilIs::IsValidIP( (char*)(LPCTSTR)strIP );
}

BOOL CUtilIs::IsValidIP( char* szIP )
{
	if( CUtilIs::CountDot( szIP ) != 3 )
		return FALSE;

	int nCount = 0;
    char *szToken = strtok( szIP, "." );

    while( szToken != NULL )  {
		printf( " szToken : %s\r\n", szToken );
		
		// 1. check Filed  count between Dot(".") charator
		nCount++;
		if( 4 < nCount ) {
			// AfxMessageBox( "IP형식에 어긋납니다.!", MB_ICONWARNING );
			return FALSE;
		}

		// 2. check digit 
		if( !CUtilIs::IsDigit( szToken ) ) {
			// AfxMessageBox( "올바른 값을 입력하세요.!", MB_ICONWARNING );
			return FALSE;
		}

		// 3. check range( 0 ~ 255 )
		int n = atoi( szToken );
		
		if( 0 <= n && n <= 255 ) {
			szToken = strtok( NULL, "." );
			continue;
		}
		else {
			// AfxMessageBox( "IP Field 값은 0 ~ 255의 값이어야 합니다.!", MB_ICONWARNING );
			return FALSE;
		}
	}

	if( nCount != 4 )
		return FALSE;

	return TRUE;
}

int CUtilIs::CountDot( char* sz )
{
	int nCount = 0;

	for( int i = 0; i < (int)strlen(sz); i++ ) {
		if( sz[i] == '.' )
			nCount++;
	}

	return nCount;
}

BOOL CUtilIs::IsDigit( CString str )
{
	return CUtilIs::IsDigit( (char*)(LPCTSTR)str);
}

BOOL CUtilIs::IsDigit( char* sz )
{
	for( int i = 0; i < (int)strlen(sz); i++ ) {
		if( isdigit( (unsigned char)sz[i] ) == 0 )
			return FALSE;
	}

	return TRUE;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Port Check

BOOL CUtilIs::IsValidPort( CString strPort )
{
	char		*szPort = (char*)(LPCTSTR)strPort;

	// check range( 0 ~ 65536 )
	int n = atoi( szPort );

	if( 0 <= n && n <= 65536 ) 
	{
		return TRUE;
	}
	else 
	{
		MessageBox( NULL, "Port는 0 ~ 65536의 값이어야 합니다.!", "Warning", MB_ICONWARNING );
		return FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Etc
