
// PngButtonDlg.h : ��� ����
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#include "resource.h"
#include "GdipButton.h"
#include "TextProgressCtrl.h"
#include "ColorStatic.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CSplashDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSplashDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SPLASH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

// virtual
public :
	virtual BOOL OnInitDialog();

// message map 
public :
	afx_msg void        OnPaint();
    afx_msg void        OnNcPaint();
    afx_msg void        OnWindowPosChanging( WINDOWPOS* lpwndpos );
	afx_msg HCURSOR     OnQueryDragIcon();
    afx_msg BOOL        OnEraseBkgnd( CDC* pDC );
	DECLARE_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private:
    CGdipButton         m_btMobigoIcon;
    CGdipButton         m_btMobigoLogo;
    CColorStatic        m_stUpdate;
    CColorStatic        m_stVersion;
    CColorStatic        m_stMainText;
    CColorStatic        m_stMainPercent;
    CColorStatic        m_stSubText;
	CTextProgressCtrl   m_pgDownload;
    CColorStatic        m_stBuildTime;

    int                 m_nCurDownloadSize;
    int                 m_nTotalDownloadSize;

    int                 m_nZipFileCount; // Unzip�� zip ���� ����( zip ���ϳ��� ���� ���� �ƴ� )
    int                 m_nCurZipTh;     // ���� unzip�ǰ� �ִ� zip ���� ���� 
////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// init
public:
    void                InitDialog();

// get/set
public :
    void                SetMainText( CString strMainText )                  { m_stMainText.SetWindowText( strMainText );       }
    void                SetMainPercent( CString strMainPercent )            { m_stMainPercent.SetWindowText( strMainPercent ); }
    void                SetSubText( CString strSubText )                    { m_stSubText.SetWindowText( strSubText );         }

    void                SetProgressRange( int nLower, int nUpper );
    void                SetProgressPos( int nPos )                          { m_pgDownload.SetPos( nPos );               }
    int                 GetProgressPos()                                    { return m_pgDownload.GetPos();              }
	void			    SetTotalDownloadSize( int nTotalDownloadSize )		{ m_nTotalDownloadSize = nTotalDownloadSize; }
	int           	    GetTotalDownloadSize()                         		{ return m_nTotalDownloadSize;               }
    void                SetZipFileCount( int nZipFileCount )                { m_nZipFileCount = nZipFileCount;           }
    void                SetCurZipTh( int nCurZipTh )                        { m_nCurZipTh = nCurZipTh;                   }

// public
public :
    void                StepIt();
    void                StepDownload( int nDownloadDeltaSize );
    void                StepUnzip( int nLower, int nUpper, int nSubPos );
};
