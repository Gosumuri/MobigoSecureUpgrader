// LogEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LogEdit.h"


// CLogEdit

IMPLEMENT_DYNAMIC(CLogEdit, CEdit)
CLogEdit::CLogEdit()
{
}

CLogEdit::~CLogEdit()
{
}

BEGIN_MESSAGE_MAP(CLogEdit, CEdit)
	ON_MESSAGE( MY_MSG_DEBUG,   OnMsgDebugHandler   )  // debug
	ON_MESSAGE( MY_MSG_WARNING, OnMsgWarningHandler )  // warning
	ON_MESSAGE( MY_MSG_ERROR,   OnMsgErrorHandler   )  // error
	ON_MESSAGE( MY_MSG_WATCH,   OnMsgWatchHandler   )  // watch
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// message handlers

void CLogEdit::AddString( CString lParamString )
{
	CString     strMsg( lParamString );

	strMsg += "\r\n";

	// ¹æ¹ý 4
	this->SetSel( INT_MAX, -1 );
	this->ReplaceSel( strMsg );
}

// debug
LRESULT CLogEdit::OnMsgDebugHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( (char*)lParam );

	return TRUE;
} 

// warning
LRESULT CLogEdit::OnMsgWarningHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( (char*)lParam );

	return TRUE;
} 

// error
LRESULT CLogEdit::OnMsgErrorHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( (char*)lParam );

	return TRUE;
} 

// watch
LRESULT CLogEdit::OnMsgWatchHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( (char*)lParam );

	return TRUE;
} 