#include "StdAfx.h"
#include "utilprog.h"

#ifndef MAX_BUF
	#define MAX_BUF 256
#endif

// window
HWND		CUtilProg::m_hWndMainFrm		= (HWND)NULL; 
char*		CUtilProg::m_pSzMsg				= (char*)NULL;

CUtilProg::CUtilProg(void)
{
}

CUtilProg::~CUtilProg(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Init / Exit

void CUtilProg::Init( HWND hWnd )
{
	m_hWndMainFrm = hWnd;

	// 0. message
	m_pSzMsg			= (char*)malloc(MAX_BUF + 1);
	memset( m_pSzMsg, 0, sizeof(char)*( MAX_BUF + 1) );
}

void CUtilProg::Exit()
{
	// 1. message
	if( m_pSzMsg != NULL )
		free( m_pSzMsg );
    m_pSzMsg = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Progress

void CUtilProg::ProgressStart( CString strLabel, int nTotal )
{
	if( m_hWndMainFrm != NULL ) {
		if( !strLabel.IsEmpty() )
			_tcscpy( (LPTSTR)m_pSzMsg, strLabel );
		else 
			_tcscpy( (LPTSTR)m_pSzMsg, "" );

		::SendMessage( m_hWndMainFrm, MY_MSG_PROGRESS_START, (WPARAM)m_pSzMsg, (LPARAM)nTotal ); 
	}
}

void CUtilProg::ProgressStep( int nPos )
{
	if( m_hWndMainFrm != NULL )
		::SendMessage( m_hWndMainFrm, MY_MSG_PROGRESS_STEP, NULL, (LPARAM)nPos ); 
}


void CUtilProg::ProgressEnd()
{
	if( m_hWndMainFrm != NULL ) 
		::SendMessage( m_hWndMainFrm, MY_MSG_PROGRESS_END, NULL, NULL ); 
}