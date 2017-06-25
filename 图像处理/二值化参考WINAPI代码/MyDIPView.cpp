// MyDIPView.cpp : implementation of the CMyDIPView class
//

#include "stdafx.h"
#include "MyDIP.h"

#include "MyDIPDoc.h"
#include "MyDIPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView

IMPLEMENT_DYNCREATE(CMyDIPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyDIPView, CScrollView)
	//{{AFX_MSG_MAP(CMyDIPView)
	ON_COMMAND(ID_MENUITEM32778, OnMenuitem32778)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView construction/destruction

CMyDIPView::CMyDIPView()
{
	// TODO: add construction code here

}

CMyDIPView::~CMyDIPView()
{
}

BOOL CMyDIPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView drawing

void CMyDIPView::OnDraw(CDC* pDC)
{   
	CMyDIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_hDIB == NULL)
		return ;
	// TODO: add draw code for native data here
	int i,j;
    unsigned char *lpSrc;
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
	// 计算图像每行的字节数
	long lLineBytes = WIDTHBYTES(cxDIB * 8);
	// 每行
	for(i = 0; i < cyDIB; i++)
	{
		// 每列
		for(j = 0; j < cxDIB; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			// 计算新的灰度值
			//*(lpSrc) = BYTE(255-*lpSrc);
		}
	}
	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
	CRect rect(0,0,cxDIB,cyDIB), rcDIB(0,0,cxDIB,cyDIB);
	::PaintDIB(pDC->m_hDC, &rect, pDoc->m_hDIB, &rcDIB, pDoc->m_palDIB);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView printing

BOOL CMyDIPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyDIPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyDIPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView diagnostics

#ifdef _DEBUG
void CMyDIPView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyDIPView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyDIPDoc* CMyDIPView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDIPDoc)));
	return (CMyDIPDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView message handlers

void CMyDIPView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}

void CMyDIPView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, GetDocument()->m_sizeDoc);
}


//对比度拉伸
//DEL void CMyDIPView::OnMenuitem32777() 
//DEL {
//DEL 	
//DEL 	// 获取文档
//DEL 	CMyDIPDoc* pDoc = GetDocument();
//DEL 	int i,j;
//DEL 	int r1=60,r2=200;
//DEL 	double k=1.5;
//DEL     unsigned char *lpSrc;	
//DEL 	ASSERT_VALID(pDoc);
//DEL 	if(pDoc->m_hDIB == NULL)
//DEL 		return ;
//DEL 	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
//DEL 	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
//DEL 	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
//DEL 	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
//DEL 	long lLineBytes = WIDTHBYTES(cxDIB * 8);     // 计算图像每行的字节数
//DEL 	// 每行
//DEL 	for(i = 0; i < cyDIB; i++)
//DEL 	{
//DEL 		// 每列
//DEL 		for(j = 0; j < cxDIB; j++)
//DEL 		{
//DEL 			// 指向DIB第i行，第j个象素的指针
//DEL 			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
//DEL 			// 计算新的灰度值
//DEL 			if(*lpSrc<r1) *lpSrc=BYTE(*lpSrc/k);
//DEL 			else if(*lpSrc<r2) *lpSrc= BYTE((*lpSrc-r1)*k+r1/k);
//DEL 			else *lpSrc=BYTE((*lpSrc-r2)/k+255-(255-r2)/k);
//DEL 		}
//DEL 	}
//DEL 	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
//DEL     Invalidate(TRUE);
//DEL }



void CMyDIPView::OnMenuitem32778() 
{
	// TODO: Add your command handler code here
	int i,j;
    unsigned char *lpSrc;
	CMyDIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_hDIB == NULL)
		return ;
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
	long lLineBytes = WIDTHBYTES(cxDIB * 8);     // 计算图像每行的字节数
	const float c1=150,c2=2.5;
	// 每行
	for(i = 0; i < cyDIB; i++)
	{
		// 每列
		for(j = 0; j < cxDIB; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			
			// 计算新的灰度值
			if(*lpSrc<122) *lpSrc=BYTE(0);
			else *lpSrc = BYTE(255);
		}
	}
	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
    Invalidate(TRUE);
}