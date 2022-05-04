////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilmessagebox.h"
#include "htmlMessageBoxDialog.h"
#include "UtilEnv.h"
#include "UtilFile.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

HHOOK   CUtilMessageBox::m_hhkAlert          =   NULL;
HHOOK   CUtilMessageBox::m_hhkConfirm        =   NULL;

CString CUtilMessageBox::m_strOkBtnText      =   "";
CString CUtilMessageBox::m_strYesBtnText     =   "";
CString CUtilMessageBox::m_strNoBtnText      =   "";

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilMessageBox::CUtilMessageBox(void)
{
    
}

CUtilMessageBox::~CUtilMessageBox(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// alert(system) 
////////////////////////////////////////////////////////////////////////////////

LRESULT CUtilMessageBox::AlertCallbackProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
   HWND hChildWnd;    // msgbox is "child"
   // notification that a window is about to be activated
   // window handle is wParam
   if (nCode == HCBT_ACTIVATE)
   {
      // set window handles
      hChildWnd = (HWND)wParam;
      //to get the text of the Yes button
      UINT result;
      if (GetDlgItem(hChildWnd,IDOK)!=NULL)
      {
         result= SetDlgItemText(hChildWnd,IDOK, m_strOkBtnText );
      }
      //Continue changing other button captions
 
      // exit CBT hook
      UnhookWindowsHookEx(m_hhkAlert);
   }
   // otherwise, continue with any possible chained hooks
   else CallNextHookEx(m_hhkAlert, nCode, wParam, lParam);
   return 0;
}

int CUtilMessageBox::Alert( CString strContent, CString strTitle, CString strBtnText )
{
    m_strOkBtnText = strBtnText;   

    m_hhkAlert = SetWindowsHookEx( WH_CBT, 
                                   CUtilMessageBox::AlertCallbackProc, 
                                   0,
                                   GetCurrentThreadId());

    return ::MessageBox( NULL, strContent, strTitle , MB_OK | MB_SETFOREGROUND | MB_TOPMOST );    
}

////////////////////////////////////////////////////////////////////////////////
// confirm(system)
////////////////////////////////////////////////////////////////////////////////

LRESULT CUtilMessageBox::ConfirmCallbackProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
   HWND hChildWnd;    // msgbox is "child"
   // notification that a window is about to be activated
   // window handle is wParam
   if (nCode == HCBT_ACTIVATE)
   {
      // set window handles
      hChildWnd = (HWND)wParam;
      //to get the text of the Yes button
      UINT result;
      if (GetDlgItem(hChildWnd,IDYES)!=NULL)
      {
         result= SetDlgItemText(hChildWnd,IDYES, m_strYesBtnText );
      }
      if (GetDlgItem(hChildWnd,IDNO)!=NULL)
      {
         result= SetDlgItemText(hChildWnd,IDNO, m_strNoBtnText );
      }
      //Continue changing other button captions
 
      // exit CBT hook
      UnhookWindowsHookEx(m_hhkConfirm);
   }
   // otherwise, continue with any possible chained hooks
   else CallNextHookEx(m_hhkConfirm, nCode, wParam, lParam);
   return 0;
}

int CUtilMessageBox::Confirm( CString strMessage, CString strTitle, CString strYesBtnText, CString strNoBtnText )
{
    m_strYesBtnText     = strYesBtnText;   
    m_strNoBtnText      = strNoBtnText;   

    m_hhkConfirm = SetWindowsHookEx( WH_CBT, 
                                     CUtilMessageBox::ConfirmCallbackProc, 
                                     0,
                                     GetCurrentThreadId());

    return ::MessageBox( NULL, strMessage, strTitle , MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST );    
}

////////////////////////////////////////////////////////////////////////////////
// alert(html)
////////////////////////////////////////////////////////////////////////////////

int CUtilMessageBox::AlertHtml( CString strModuleName, CString strContent, CString strTitle, CString strBtnText )
{
    CHtmlMessageBoxDialog dlg;

    dlg.SetModuleName( CUtilFile::ExtractFileNameWithoutExt( strModuleName ) );
    dlg.SetMessageBoxUrl( CUtilEnv::GetAlertHtmlPath() );
    dlg.SetMessageBoxContent(strContent);
    dlg.SetMessageBoxTitle(strTitle);
    dlg.DoModal();

    return IDOK;
}

////////////////////////////////////////////////////////////////////////////////
// confirm(html)
////////////////////////////////////////////////////////////////////////////////

int CUtilMessageBox::ConfirmHtml( CString strModuleName, CString strContent, CString strTitle, CString strYesBtnText, CString strNoBtnText )
{
    CHtmlMessageBoxDialog dlg;

    dlg.SetModuleName( CUtilFile::ExtractFileNameWithoutExt( strModuleName ) );
    dlg.SetMessageBoxUrl( CUtilEnv::GetConfirmHtmlPath() );
    dlg.SetMessageBoxContent(strContent);
    dlg.SetMessageBoxTitle(strTitle);
    dlg.DoModal();

    int nResult = dlg.GetResult();

    if( nResult == HTML_MESSAGE_BOX_RESULT_YES ) {
        return IDYES;
    }

    return IDNO;
}