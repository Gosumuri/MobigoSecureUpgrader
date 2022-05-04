
// PngButtonDlg.h : 헤더 파일
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
// 생성입니다.
public:
	CSplashDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLASH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
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

    int                 m_nZipFileCount; // Unzip할 zip 파일 개수( zip 파일내의 파일 개수 아님 )
    int                 m_nCurZipTh;     // 현재 unzip되고 있는 zip 파일 순번 
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
