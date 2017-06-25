// MyDIPView.h : interface of the CMyDIPView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDIPVIEW_H__9BEE6960_11E8_4B4E_A88D_7314312B6D29__INCLUDED_)
#define AFX_MYDIPVIEW_H__9BEE6960_11E8_4B4E_A88D_7314312B6D29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyDIPView : public CScrollView
{
protected: // create from serialization only
	CMyDIPView();
	DECLARE_DYNCREATE(CMyDIPView)

// Attributes
public:
	CMyDIPDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDIPView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyDIPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyDIPView)
	afx_msg void OnMenuitem32778();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnInitialUpdate();
	void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in MyDIPView.cpp
inline CMyDIPDoc* CMyDIPView::GetDocument()
   { return (CMyDIPDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIPVIEW_H__9BEE6960_11E8_4B4E_A88D_7314312B6D29__INCLUDED_)
