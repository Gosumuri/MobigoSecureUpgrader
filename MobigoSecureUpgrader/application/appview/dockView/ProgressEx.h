#pragma once


// CProgressEx

#define PROGRESS_NONE	1
#define PROGRESS_TEXT	2
#define PROGRESS_BITMAP	3

class CProgressEx : public CProgressCtrl
{
/* Construction */
public:
	CProgressEx();
	~CProgressEx();

/* Attributes */
public:

/* Operations */
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressEx)
	//}}AFX_VIRTUAL

/* Implementation */
public:
	int SetPos(int nPos);
	void SetRange(int nLower, int nUpper);
	int SetStep(int nStep);
	int StepIt();
	void SetTextBkColor(COLORREF clrTextBk);
	void SetTextForeColor(COLORREF clrTextFore);
	void SetBkColor(COLORREF clrBk);
	void SetForeColor(COLORREF clrFore);
	void SetTextColor(COLORREF clrText);

	void SetStyle(int nStyle);
	void SetText(CString strText);
	void SetBitmap(int nId);

	// Generated message map functions
protected:
	//{{AFX_MSG(CProgressEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CBitmap m_Bitmap;
	int m_nHeight;
	int m_nWidth;

	int m_nMin;
	int m_nMax;

	int m_nPos;
	int m_nStep;

	int m_nStyle;

	COLORREF m_clrBk;
	COLORREF m_clrFore;
	COLORREF m_clrTextBk;
	COLORREF m_clrTextFore;
	COLORREF m_clrText;

	CString m_strText;
// Enable
public:
	void	SetEnable( BOOL bEnable );
};
