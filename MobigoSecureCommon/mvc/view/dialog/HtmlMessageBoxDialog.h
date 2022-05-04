#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32_WCE
#error "Windows CE에서는 CDHtmlDialog가 지원되지 않습니다."
#endif 
#include "resource.h"
#include <afxdhtml.h>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define HTML_MESSAGE_BOX_MODE_ALERT         0
#define HTML_MESSAGE_BOX_MODE_CONFIRM       1

#define HTML_MESSAGE_BOX_RESULT_NONE        0
#define HTML_MESSAGE_BOX_RESULT_OK          1
#define HTML_MESSAGE_BOX_RESULT_YES         2
#define HTML_MESSAGE_BOX_RESULT_NO          3

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

/**
 *  CDHtmlDialog
 *    + CHtmlMessageBoxDialog          <--
 */

class AFX_EXT_CLASS CHtmlMessageBoxDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CHtmlMessageBoxDialog)
    DECLARE_DISPATCH_MAP()
public:
	CHtmlMessageBoxDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHtmlMessageBoxDialog();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_HTML_MESSAGE_BOX };

protected:
	// virtual 
    virtual void    OnOK();
    virtual void    OnCancel();
    virtual BOOL    OnInitDialog();
    virtual void    OnBeforeNavigate( LPDISPATCH pDisp, LPCTSTR szUrl );
    virtual void    OnNavigateComplete( LPDISPATCH pDisp, LPCTSTR szUrl );
    virtual BOOL    CanAccessExternal() {return TRUE; }

    // evnet handler
    afx_msg void    OnTimer( UINT_PTR nIDEvent );
	DECLARE_MESSAGE_MAP()

// javascript
public :
    void            OnJs(LPCTSTR szID );
    void            OnUserAgent(LPCTSTR szUserAgent );
    void            OnResolution(LPCTSTR szResolution );
    void            ToJs();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

// debug
private : 
    CString     m_strDialogTitle;
    CString     m_strModuleName;
    CString     m_strUserAgent;
    CString     m_strResolution;
           
// property 
private:
    CString     m_strMessageBoxUrl;
    CString     m_strMessageBoxTitle;
    CString     m_strMessageBoxContent;
    int         m_nResult;
    int         m_nZoom;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public : 
    void    SetMessageBoxUrl(       CString strUrl        )  { m_strMessageBoxUrl       = strUrl;        }
    void    SetMessageBoxTitle(     CString strTitle      )  { m_strMessageBoxTitle     = strTitle;      }
    void    SetMessageBoxContent(   CString strContent    )  { m_strMessageBoxContent   = strContent;    }
    void    SetModuleName(          CString strModuleName )  { m_strModuleName          = strModuleName; } 
    int     GetResult()                                      { return m_nResult;                         }
// utility
private:
    void    LoadHtml( CString strUrl );
    void    Zoom();
    void    FillTitle();

};
