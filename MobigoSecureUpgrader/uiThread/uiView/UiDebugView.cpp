
// MobigoSecureUpgraderView.cpp : CUiDebugView Ŭ������ ����
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MobigoSecureUpgrader.h"
#endif

#include "UiDoc.h"
#include "UiDebugView.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CUiDebugView, CView)

BEGIN_MESSAGE_MAP(CUiDebugView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_TIMER()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUiDebugView::CUiDebugView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CUiDebugView::~CUiDebugView()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
/////////////////////////////////////////////////////////////////////////////

BOOL CUiDebugView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CUiDebugView �׸���

void CUiDebugView::OnDraw(CDC* /*pDC*/)
{
	CUiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}

////////////////////////////////////////////////////////////////////////////////
// debug
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CUiDebugView::AssertValid() const
{
	CView::AssertValid();
}

void CUiDebugView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUiDoc* CUiDebugView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUiDoc)));
	return (CUiDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CUiDebugView::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	DEBUG_LOG(">> CUiDebugView::OnWindowPosChanging()");

    CView::OnWindowPosChanging( lpwndpos );

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
#ifndef FEATURE_RELEASE_VIEW
    this->SetTimer( MY_TIMER_SPLASH_SHOW, 100, NULL ); 
#endif
}

void CUiDebugView::OnTimer( UINT_PTR nIDEvent )
{
	DEBUG_LOG(">> CUiDebugView::OnTimer()");

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
    switch( nIDEvent ) {
        case MY_TIMER_SPLASH_SHOW : {
	        DEBUG_LOG("++   MY_TIMER_SPLASH_SHOW");
            KillTimer(MY_TIMER_SPLASH_SHOW);    
            // this�� �ѱ�� ������ CUiSplashView���� window handle�� ����ϱ� ����.
            if( m_pWndParent == NULL ) {
                ShowSplash( this, 0, SPLASH_PROGRESS_POS_UNZIP );
                FirstStepSplash();
            }
        }
        break;
        case MY_TIMER_SPLASH_CLOSE : {
	        DEBUG_LOG("++   MY_TIMER_SPLASH_CLOSE");
            KillTimer(MY_TIMER_SPLASH_CLOSE);    
            
            int nYesNo =  AfxMessageBox( "Run App ?" , MB_YESNO);
            if( nYesNo == IDYES ) {
                OnCloseSplash( TRUE /* bRunApp */, TRUE /* bExit */  );
            } else {
                OnCloseSplash( FALSE /* bRunApp */, FALSE /* bExit */  );
            }
        }
        break;
    }
    
    CView::OnTimer( nIDEvent );
}