// MyDIP.h : main header file for the MYDIP application
//

#if !defined(AFX_MYDIP_H__CCB27B8B_4ABF_4C6F_8CC6_EF382EE0C903__INCLUDED_)
#define AFX_MYDIP_H__CCB27B8B_4ABF_4C6F_8CC6_EF382EE0C903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
class CMyDIPApp : public CWinApp
{
public:
	CMyDIPApp();
	public:
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
#endif
