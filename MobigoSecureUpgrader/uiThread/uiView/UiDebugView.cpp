
// MobigoSecureUpgraderView.cpp : CUiDebugView 클래스의 구현
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// TODO: 여기에 생성 코드를 추가합니다.

}

CUiDebugView::~CUiDebugView()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
/////////////////////////////////////////////////////////////////////////////

BOOL CUiDebugView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CUiDebugView 그리기

void CUiDebugView::OnDraw(CDC* /*pDC*/)
{
	CUiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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

CUiDoc* CUiDebugView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
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

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifndef FEATURE_RELEASE_VIEW
    this->SetTimer( MY_TIMER_SPLASH_SHOW, 100, NULL ); 
#endif
}

void CUiDebugView::OnTimer( UINT_PTR nIDEvent )
{
	DEBUG_LOG(">> CUiDebugView::OnTimer()");

    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch( nIDEvent ) {
        case MY_TIMER_SPLASH_SHOW : {
	        DEBUG_LOG("++   MY_TIMER_SPLASH_SHOW");
            KillTimer(MY_TIMER_SPLASH_SHOW);    
            // this를 넘기는 이유는 CUiSplashView에서 window handle을 사용하기 위함.
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