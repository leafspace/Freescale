// MyDIPDoc.cpp : implementation of the CMyDIPDoc class
//

#include "stdafx.h"
#include "MyDIP.h"

#include "MyDIPDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDIPDoc

IMPLEMENT_DYNCREATE(CMyDIPDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyDIPDoc, CDocument)
	//{{AFX_MSG_MAP(CMyDIPDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDIPDoc construction/destruction

CMyDIPDoc::CMyDIPDoc()
{
	// TODO: add one-time construction code here
	m_sizeDoc.cx = 800;                                                         //DIB区域的划定
	m_sizeDoc.cy = 400;                                                         //DIB区域的划定

	m_palDIB = NULL;
	m_hDIB = NULL;
}

CMyDIPDoc::~CMyDIPDoc()
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
	}	
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB=NULL;
	}
}

BOOL CMyDIPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_hDIB = NewDIB(m_sizeDoc.cx, m_sizeDoc.cy,8);
	InitDIBData();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyDIPDoc serialization

void CMyDIPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPDoc diagnostics

#ifdef _DEBUG
void CMyDIPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyDIPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDIPDoc commands

BOOL CMyDIPDoc::ReadImgFile(CString sName)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
	}	
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB=NULL;
	}

	// replace calls to Serialize with ReadDIBFile function
	CFile nFile;
	if(!nFile.Open(sName,CFile::modeRead))
		return false;
	m_hDIB = ReadDIBFile(nFile);
	nFile.Close();
	InitDIBData();

	if (m_hDIB == NULL)
		return FALSE;
	return TRUE;
}

BOOL CMyDIPDoc::InitDIBData()
{
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		return FALSE;
	}
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return FALSE;
	}

	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// DIB may not have a palette
		delete m_palDIB;
		m_palDIB = NULL;
	}

	return TRUE;
}

BOOL CMyDIPDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	BOOL b = ReadImgFile(lpszPathName);
	if(b)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		m_sizeDoc.cx = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		m_sizeDoc.cy = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) m_hDIB);
	}
	POSITION pos;
	pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		CScrollView * pView = (CScrollView *)GetNextView(pos);
		if(pView != NULL)
		{
			pView->SetScrollSizes(MM_TEXT, m_sizeDoc);

		}
	}
	return b;
}

BOOL CMyDIPDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	CFile nFile;
	if(m_hDIB == NULL)
		return  false;
	nFile.Open(lpszPathName,CFile::modeWrite | CFile::modeCreate);
	SaveDIB(m_hDIB, nFile);
	nFile.Close();
	return true;
	//return CDocument::OnSaveDocument(lpszPathName);
}