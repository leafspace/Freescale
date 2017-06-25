// MyDIPDoc.h : interface of the CMyDIPDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDIPDOC_H__EAAB6882_287D_41DA_81A4_1E2D93804886__INCLUDED_)
#define AFX_MYDIPDOC_H__EAAB6882_287D_41DA_81A4_1E2D93804886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif


class CMyDIPDoc : public CDocument
{
protected:
	CMyDIPDoc();
	DECLARE_DYNCREATE(CMyDIPDoc)
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }
	CSize m_sizeDoc;
	HDIB m_hDIB;
	CPalette* m_palDIB;
public:
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
public:
	virtual ~CMyDIPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
private:
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL InitDIBData();
	BOOL ReadImgFile(CString sName);
};
#endif
