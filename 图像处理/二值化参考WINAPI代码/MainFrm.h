//Cmainframe类的接口
#if !defined(AFX_MAINFRM_H__985B7281_641A_418D_BAEC_55EB8382DA1A__INCLUDED_)
#define AFX_MAINFRM_H__985B7281_641A_418D_BAEC_55EB8382DA1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
#endif
