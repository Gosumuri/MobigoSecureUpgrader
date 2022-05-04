// ProgressEx.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressEx.h"


/* Constructor */
CProgressEx::CProgressEx()
{
	m_nMin = 0;
	m_nMax = 100;

	m_nPos = 0;
	m_nStep = 10;

	m_clrFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrBk			= ::GetSysColor(COLOR_WINDOW);
	m_clrTextFore	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrTextBk		= ::GetSysColor(COLOR_WINDOW);
	m_clrText = RGB(255, 0, 0);
	m_strText.Empty();
	m_nStyle = PROGRESS_NONE;	// ���� ProgressCtrl
} /* end of Constructor */


/* Destructor */
CProgressEx::~CProgressEx()
{
} /* end of Destructor */



/* MESSAGE_MAP */
BEGIN_MESSAGE_MAP(CProgressEx, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressCtrlH2)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* OnPaint */
void CProgressEx::OnPaint() 
{
	if(m_nStyle == PROGRESS_NONE) {	// �Ϲ� ProgressCtrl �϶�
		CProgressCtrl::OnPaint();
		return;
	}
			
	CPaintDC dc(this);
	CRect rcClient, rcLeft, rcRight;
	GetClientRect(rcClient);
	rcLeft = rcRight = rcClient;

	/* ����ڰ� ������ ��ġ ��(m_nPos)�� �������� */
	/* m_nMin�� m_nMax �������� �ش�Ǵ� �ۼ�Ʈ�� ���Ѵ� */
	double Fraction = (double)(m_nPos - m_nMin) / (double)(m_nMax - m_nMin);

	/* Progress ����ٸ� �׸��� ���� ���� ���� ���Ѵ� */
    rcLeft.right = rcLeft.left + (int)((rcLeft.right - rcLeft.left) * Fraction);

	/* rcLeft : ����� ����     */
	/* rcRight : ����ٸ� ������ ���� */
	rcLeft.right = (int)((rcClient.right - rcClient.left) * Fraction);
	rcRight.left = rcLeft.right;

	switch(m_nStyle) {
	case PROGRESS_TEXT : {	
			/* 1. ����ٸ� �� �׸��� */
		    dc.FillSolidRect(rcLeft, m_clrFore);	/* Ǫ���� */

			/* 2. ����ٸ� ������ ������ �κ��� �׸��� */
			dc.FillSolidRect(rcRight, m_clrBk);		/* ��� */

			if(m_strText.IsEmpty()) {
				CString str;
		        str.Format("%d%%", (int)(Fraction*100.0));

				m_strText = str;
			}

	        dc.SetBkMode(TRANSPARENT);

			/* CRgn class�� ����ϴ� ���� ? */
			/*  : ���ڿ��� Ư�� ���������� ���� ��ȭ�� �־�� �ϱ� ������ */
			/* 1. ����� ������ ������ ���ڿ� �׸��� */
			CRgn rgn;
	        rgn.CreateRectRgn(rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom);
	        dc.SelectClipRgn(&rgn);
	        dc.SetTextColor(m_clrTextBk);

	        dc.DrawText(m_strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			/* 2. ����� ������ ������ ���ڿ� �׸��� */
	        rgn.DeleteObject();
	        rgn.CreateRectRgn(rcRight.left, rcRight.top, rcRight.right, rcRight.bottom);
	        dc.SelectClipRgn(&rgn);
	        dc.SetTextColor(m_clrTextFore);

	        dc.DrawText(m_strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		} break;
	case PROGRESS_BITMAP : {
			int nHBmp, nVBmp;

			/* 1. SetBitmap �Լ��� ������ ��Ʈ�ʰ� ȣȯ�ȴ� DC�� ����� */
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&m_Bitmap);

			/* 2. ��Ʈ�Ѱ� ������ ������ �����ϴ� ��Ʈ�ʰ� DC�� ����� */
			CDC leftDC, rightDC;
			leftDC.CreateCompatibleDC(&dc);
			rightDC.CreateCompatibleDC(&dc);

			CBitmap leftBmp, rightBmp;
			leftBmp.CreateCompatibleBitmap(&memDC, rcClient.Width(), rcClient.Height());
			rightBmp.CreateCompatibleBitmap(&memDC, rcClient.Width(), rcClient.Height());
	
			CBitmap* pOldleftBmp = (CBitmap*)leftDC.SelectObject(&leftBmp);
			CBitmap* pOldrightBmp = (CBitmap*)rightDC.SelectObject(&rightBmp);

			/* ��Ʈ�� ũ��� ��Ʈ�� ũ�Ⱑ ��ġ���� �����Ƿ� Ÿ�� �������� leftDC�� ��Ʈ���� �׸��� */
			nHBmp = rcClient.Width() / m_nWidth;
			if(rcClient.Width() % m_nWidth) nHBmp++;

			nVBmp = rcClient.Height() / m_nHeight;
			if(rcClient.Height() % m_nHeight) nVBmp++;

			for(int x = 0; x < nHBmp; x++) {
				for(int y = 0; y < nVBmp; y++) {
					leftDC.BitBlt(m_nWidth * x, m_nHeight * y, m_nWidth, m_nHeight,
								&memDC, 0, 0, SRCCOPY);
				}
			}

			/* �⺻ ������ �����Ѵ� */
			rightDC.FillSolidRect(rcClient, m_clrBk);

			/* Font Object�� �����Ѵ� */
			CFont* pFont = GetFont();	/* ������ ��Ʈ�� ��´� */
			CFont* pOldFont = (CFont*)rightDC.SelectObject(pFont);

			CString str;
			str.Format("%d%%", m_nPos);
			CSize sz = dc.GetTextExtent(str);	/* ���ڿ��� ����� ���Ѵ� */

			if(rcRight.Width() > sz.cx) {
				rightDC.SetTextColor(m_clrText);
				rightDC.TextOut(2, (rcLeft.Height() / 2) - (sz.cy / 2) + 1, str);
			}

			/* Progress Ctrl�� �⺻���� 3D ������ �׸����� �ʱ� ���ؼ� ������ �����Ѵ� */
			rcLeft.DeflateRect(1, 1);
			rcRight.DeflateRect(1, 1);

			/* ����� �׸��� */
			dc.BitBlt(1, 1, rcLeft.Width(), rcLeft.Height(), &leftDC, 0, 0, SRCCOPY);

			/* ����ٸ� ������ ������ �κ� �׸��� */
			dc.BitBlt(rcRight.left, rcRight.top, rcRight.Width(), rcRight.Height(), &rightDC, 0, 0, SRCCOPY);

			leftDC.SelectObject(pOldleftBmp);
			rightDC.SelectObject(pOldrightBmp);
			memDC.SelectObject(pOldBitmap);

			rightDC.SelectObject(pOldFont);
	
			leftBmp.DeleteObject();
			rightBmp.DeleteObject();
		}
		break;
	}
} /* end of OnPaint */


/*OnEraseBkgnd*/
BOOL CProgressEx::OnEraseBkgnd(CDC* pDC) 
{
	if(m_nStyle != PROGRESS_NONE)
		return TRUE;

	return CProgressCtrl::OnEraseBkgnd(pDC);
} /*end of OnEraseBkgnd*/


/*SetBitmap*/
void CProgressEx::SetBitmap(int nId)
{
	/* ��Ʈ�� �ڵ��� ��´� */
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(nId),
		IMAGE_BITMAP,
		0,
		0,
		0);

	m_Bitmap.Detach();
	m_Bitmap.Attach(hBmp);

	BITMAP bmp;
	m_Bitmap.GetObject(sizeof(bmp), &bmp);

	m_nWidth = bmp.bmWidth;
	m_nHeight = bmp.bmHeight;
} /* End of SetBitmap */


/* Progress�� ����� �ؽ�Ʈ�� �����Ѵ� */
void CProgressEx::SetText(CString strText)
{
	m_strText = strText;
	Invalidate( TRUE );			
} /* end of SetText */


/* Setpos */
int CProgressEx::SetPos(int nPos)
{
	if(m_nMin <= nPos && nPos <= m_nMax) {
		m_nPos = nPos;
		Invalidate();
	}

	return CProgressCtrl::SetPos(m_nPos);
} /* end of SEtPos*/


/* SEtting Range */
void CProgressEx::SetRange(int nLower, int nUpper)
{
	if(nLower <= nUpper) {
		m_nMin = nLower;
		m_nMax = nUpper;
	}

	CProgressCtrl::SetRange(m_nMin, m_nMax);
} /* end of Setting Range */


/* SetStep */
int CProgressEx::SetStep(int nStep)
{
	m_nStep = nStep;

	return CProgressCtrl::SetStep(m_nStep);
} /* end of setStep */


/* StepIt */
int CProgressEx::StepIt()
{
	m_nPos += m_nStep;
	
	if(m_nPos > m_nMax)
		m_nPos = m_nMin + (m_nPos - m_nMax);

	Invalidate();

	return CProgressCtrl::StepIt();
} /* end of setIt */


/* ����� ���� */
void CProgressEx::SetForeColor(COLORREF clrFore)
{
	m_clrFore = clrFore;
} /* SetForeColor */


/* ����ٸ� ������ �κ� ���� */
void CProgressEx::SetBkColor(COLORREF clrBk)
{
	m_clrBk = clrBk;
} /* SetBkcolor */


/* ����ٿ� ��ġ�� ���ڿ� ���� */
void CProgressEx::SetTextForeColor(COLORREF clrTextFore)
{
	m_clrTextFore = clrTextFore;
} /* end fo SetTextForeColor */


/* ����ٸ� ������ �κп� ��ġ�� ���ڿ� ���� */
void CProgressEx::SetTextBkColor(COLORREF clrTextBk)
{
	m_clrTextBk = clrTextBk;
} /* end of setTextBkcolor */


/* ��Ʈ�� ����ٿ��� ������� ��Ÿ���� ���ڿ� ���� */
void CProgressEx::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
} /* SetTextColor */


/* ProgressCtrl ������ �����Ѵ� */
void CProgressEx::SetStyle(int nStyle)
{
	m_nStyle = nStyle;
} /* end of SetStyle */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable

void CProgressEx::SetEnable( BOOL bEnable )
{
	if( bEnable ) {
		SetBkColor( RGB(255, 255, 255) );
		Invalidate( TRUE );
	}
	else { 
		SetBkColor( RGB(192, 192, 192) );
		Invalidate( TRUE );
	}

	EnableWindow( bEnable );
}
