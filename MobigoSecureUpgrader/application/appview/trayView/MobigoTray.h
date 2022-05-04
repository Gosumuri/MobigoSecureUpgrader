#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include "CoolMenu.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

#define MOBIGO_TRAY_INIT( __mobigo_tray__, __parent__, __msg__, __menu__ ) {                    \
        CUtilSys::RefreshTrayIcon();                                                            \
        if( __mobigo_tray__.m_hWnd == NULL ) {                                                  \
            HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));   \
            BOOL b = __mobigo_tray__.Create(  __parent__,                                       \
                                              __msg__,                                          \
                                              _T("Mobigo Tray"),                                \
                                              hIcon,                                            \
                                             __menu__);                                         \
            if ( b == TRUE ) {                                                                  \
                __mobigo_tray__.SetMenuDefaultItem(-1, TRUE);                                   \
	            __mobigo_tray__.SetIconList(IDI_TRAY_1, IDI_TRAY_4);                            \
            }                                                                                   \
        }                                                                                       \
    }

#define MENU_ICON_INSTALL( __menu_mgr__, __wnd__, __toolbar__, __menu__ )                       \
        if( __menu_mgr__.IsInstall() == FALSE ) {                                               \
	        __menu_mgr__.Install(__wnd__);                                                      \
	        UINT ayUInt[] = { __toolbar__ };                                                    \
	        __menu_mgr__.LoadToolbars(ayUInt, 1);                                               \
	        __menu_mgr__.m_bAutoAccel = FALSE;                                                  \
            ::SendMessage( __wnd__->m_hWnd, WM_INITMENUPOPUP, (WPARAM)__menu__.m_hMenu, NULL ); \
        }


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CMobigoTray : public CWnd
{
// Construction/destruction
public:
    CMobigoTray();
    CMobigoTray(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
    virtual ~CMobigoTray();

    DECLARE_DYNAMIC(CMobigoTray)

    // Modified by Jin Kim ( Korean )
    CWnd *m_pParent;
// Operations
public:
    BOOL Enabled() { return m_bEnabled; }
    BOOL Visible() { return !m_bHidden; }

    // Create the tray icon
    BOOL Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

    // Change or retrieve the Tooltip text
    BOOL    SetTooltipText(LPCTSTR pszTooltipText);
    BOOL    SetTooltipText(UINT nID);
    CString GetTooltipText() const;

    // Change or retrieve the icon displayed
    BOOL  SetIcon(HICON hIcon);
    BOOL  SetIcon(LPCTSTR lpszIconName);
    BOOL  SetIcon(UINT nIDResource);
    BOOL  SetStandardIcon(LPCTSTR lpIconName);
    BOOL  SetStandardIcon(UINT nIDResource);
    HICON GetIcon() const;
    void  HideIcon();
    void  ShowIcon();
    void  RemoveIcon();
    void  MoveToRight();

    // For icon animation
    BOOL  SetIconList(UINT uFirstIconID, UINT uLastIconID); 
    BOOL  SetIconList(HICON* pHIconList, UINT nNumIcons); 
    BOOL  Animate(UINT nDelayMilliSeconds, int nNumSeconds = -1);
    BOOL  StepAnimation();
    BOOL  StopAnimation();

    // Change menu default item
    void GetMenuDefaultItem(UINT& uItem, BOOL& bByPos);
    BOOL SetMenuDefaultItem(UINT uItem, BOOL bByPos);

    // Change or retrieve the window to send notification messages to
    BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
    CWnd* GetNotificationWnd() const;

    // Default handler for tray notification message
    virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMobigoTray)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
    void Initialise();

    BOOL            m_bEnabled;   // does O/S support tray icon?
    BOOL            m_bHidden;    // Has the icon been hidden?
    NOTIFYICONDATA  m_tnd;

    CArray<HICON, HICON> m_IconList;
    static UINT  m_nIDEvent;
    UINT         m_uIDTimer;
    int          m_nCurrentIcon;
    COleDateTime m_StartTime;
    int          m_nAnimationPeriod;
    HICON        m_hSavedIcon;
    UINT         m_DefaultMenuItemID;
    BOOL         m_DefaultMenuItemByPos;

	CCoolMenuManager	m_menuMgr;
// Generated message map functions
protected:
	//{{AFX_MSG(CMobigoTray)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};


