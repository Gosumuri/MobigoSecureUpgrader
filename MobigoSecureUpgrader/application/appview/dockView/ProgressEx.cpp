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
	m_nStyle = PROGRESS_NONE;	// 공통 ProgressCtrl
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
	if(m_nStyle == PROGRESS_NONE) {	// 일반 ProgressCtrl 일때
		CProgressCtrl::OnPaint();
		return;
	}
			
	CPaintDC dc(this);
	CRect rcClient, rcLeft, rcRight;
	GetClientRect(rcClient);
	rcLeft = rcRight = rcClient;

	/* 사용자가 지정한 위치 값(m_nPos)을 기준으로 */
	/* m_nMin과 m_nMax 범위에서 해당되는 퍼센트를 구한다 */
	double Fraction = (double)(m_nPos - m_nMin) / (double)(m_nMax - m_nMin);

	/* Progress 진행바를 그리기 위한 범위 값을 구한다 */
    rcLeft.right = rcLeft.left + (int)((rcLeft.right - rcLeft.left) * Fraction);

	/* rcLeft : 진행바 영역     */
	/* rcRight : 진행바를 제외한 영역 */
	rcLeft.right = (int)((rcClient.right - rcClient.left) * Fraction);
	rcRight.left = rcLeft.right;

	switch(m_nStyle) {
	case PROGRESS_TEXT : {	
			/* 1. 진행바를 를 그린다 */
		    dc.FillSolidRect(rcLeft, m_clrFore);	/* 푸른색 */

			/* 2. 진행바를 제외한 나머지 부분을 그린다 */
			dc.FillSolidRect(rcRight, m_clrBk);		/* 흰색 */

			if(m_strText.IsEmpty()) {
				CString str;
		        str.Format("%d%%", (int)(Fraction*100.0));

				m_strText = str;
			}

	        dc.SetBkMode(TRANSPARENT);

			/* CRgn class를 사용하는 이유 ? */
			/*  : 문자열의 특정 범위까지만 색상에 변화를 주어야 하기 때문에 */
			/* 1. 진행바 영역에 없을때 문자열 그리기 */
			CRgn rgn;
	        rgn.CreateRectRgn(rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom);
	        dc.SelectClipRgn(&rgn);
	        dc.SetTextColor(m_clrTextBk);

	        dc.DrawText(m_strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			/* 2. 진행바 영역에 있을때 문자열 그리기 */
	        rgn.DeleteObject();
	        rgn.CreateRectRgn(rcRight.left, rcRight.top, rcRight.right, rcRight.bottom);
	        dc.SelectClipRgn(&rgn);
	        dc.SetTextColor(m_clrTextFore);

	        dc.DrawText(m_strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		} break;
	case PROGRESS_BITMAP : {
			int nHBmp, nVBmp;

			/* 1. SetBitmap 함수로 설정한 비트맵과 호환된는 DC를 만든다 */
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&m_Bitmap);

			/* 2. 컨트롤과 동일한 영역을 차지하는 비트맵과 DC를 만든다 */
			CDC leftDC, rightDC;
			leftDC.CreateCompatibleDC(&dc);
			rightDC.CreateCompatibleDC(&dc);

			CBitmap leftBmp, rightBmp;
			leftBmp.CreateCompatibleBitmap(&memDC, rcClient.Width(), rcClient.Height());
			rightBmp.CreateCompatibleBitmap(&memDC, rcClient.Width(), rcClient.Height());
	
			CBitmap* pOldleftBmp = (CBitmap*)leftDC.SelectObject(&leftBmp);
			CBitmap* pOldrightBmp = (CBitmap*)rightDC.SelectObject(&rightBmp);

			/* 비트맵 크기와 컨트롤 크기가 일치하지 않으므로 타이 형식으로 leftDC에 비트맵을 그린다 */
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

			/* 기본 색상을 설정한다 */
			rightDC.FillSolidRect(rcClient, m_clrBk);

			/* Font Object를 설정한다 */
			CFont* pFont = GetFont();	/* 설정된 폰트를 얻는다 */
			CFont* pOldFont = (CFont*)rightDC.SelectObject(pFont);

			CString str;
			str.Format("%d%%", m_nPos);
			CSize sz = dc.GetTextExtent(str);	/* 문자열의 사이즈를 구한다 */

			if(rcRight.Width() > sz.cx) {
				rightDC.SetTextColor(m_clrText);
				rightDC.TextOut(2, (rcLeft.Height() / 2) - (sz.cy / 2) + 1, str);
			}

			/* Progress Ctrl의 기본적인 3D 영역은 그리ㄱ지 않기 위해서 영역을 설정한다 */
			rcLeft.DeflateRect(1, 1);
			rcRight.DeflateRect(1, 1);

			/* 진행바 그리기 */
			dc.BitBlt(1, 1, rcLeft.Width(), rcLeft.Height(), &leftDC, 0, 0, SRCCOPY);

			/* 진행바를 제외한 나머지 부분 그리기 */
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
	/* 비트맵 핸들을 얻는다 */
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


/* Progress에 출력할 텍스트를 설정한다 */
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


/* 진행바 색상 */
void CProgressEx::SetForeColor(COLORREF clrFore)
{
	m_clrFore = clrFore;
} /* SetForeColor */


/* 진행바를 제외한 부분 색상 */
void CProgressEx::SetBkColor(COLORREF clrBk)
{
	m_clrBk = clrBk;
} /* SetBkcolor */


/* 진행바에 위치한 문자열 색상 */
void CProgressEx::SetTextForeColor(COLORREF clrTextFore)
{
	m_clrTextFore = clrTextFore;
} /* end fo SetTextForeColor */


/* 진행바를 제외한 부분에 위치한 문자열 색상 */
void CProgressEx::SetTextBkColor(COLORREF clrTextBk)
{
	m_clrTextBk = clrTextBk;
} /* end of setTextBkcolor */


/* 비트맵 진행바에서 진행률을 나타내는 문자열 색상 */
void CProgressEx::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
} /* SetTextColor */


/* ProgressCtrl 성격을 결정한다 */
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
