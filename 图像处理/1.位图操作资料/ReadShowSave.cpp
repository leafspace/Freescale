#include <windows.h>                   //支持WINGDI
#include <afx.h>                       //支持CFILE

/*
#if !defined(AFX_MAINFRM_H__985B7281_641A_418D_BAEC_55EB8382DA1A__INCLUDED_)
#define AFX_MAINFRM_H__985B7281_641A_418D_BAEC_55EB8382DA1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif
*/

class CReadImageDlg : public CDialog
{
public:
	BYTE* m_pPixel;
	BYTE* m_pBmInfo;
	BITMAPFILEHEADER* m_pBmfh;              //这样存储的时候会有点问题     就要先给它一些空间
	CReadImageDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_READIMAGE_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};


CReadImageDlg::CReadImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//为位图文件头指针分配一个空间
	m_pBmfh = new BITMAPFILEHEADER;

}
//然后为Read按钮添加单击的响应函数，在下面写入如下程序：
void CReadImageDlg::OnBtnRead()
{
	//用打开对话框获得要打开位图的路径
	CFileDialog dlg(true, "*.bmp", NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, "BMP files(*.bmp)|*.bmp||");
	if (dlg.DoModal() != IDOK) {
		return;
	}
	//新建一个CFile类的对象dib，并以读的模式打开
	CFile dib(dlg.GetPathName(), CFile::modeRead);
	//读入位图文件头
	dib.Read(m_pBmfh, sizeof(BITMAPFILEHEADER));
	//为位图信息（信息头+调色板（如果有的话））分配空间
	m_pBmInfo = new BYTE[m_pBmfh->bfOffBits - 14];
	//读入位图信息，其大小为像素数据的偏移地址-位图文件头大小
	dib.Read(m_pBmInfo, m_pBmfh->bfOffBits - 14);
	//下面的程序的目的是计算像素矩阵的大小，就是为了得到height和LineBytes
	//新建一个位图信息头变量
	BITMAPINFOHEADER bmih;
	//拷贝一个信息头大小的字节到该变量中
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	//从信息头中获取位图的信息
	long width = bmih.biWidth;//获取宽度
	int bitCount = bmih.biBitCount;//获取位数
	long height = bmih.biHeight;//获取高度
	long LineBytes = (width*bitCount + 31) / 32 * 4;//计算每行像素所占的字节数
	//为像素矩阵分配空间
	m_pPixel = new BYTE[height*LineBytes];
	//读入位图的像素矩阵
	dib.Read(m_pPixel, height*LineBytes);
	//关闭文件
	dib.Close();
}

//为Show按钮添加一个响应函数，其代码如下：
void CReadImageDlg::OnBtnShow()
{
	BITMAPINFOHEADER bmih;
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	long width = bmih.biWidth;
	long height = bmih.biHeight;
	StretchDIBits(GetDC()->m_hDC, 0, 0, width, height, 0, 0, width, height, m_pPixel, (BITMAPINFO*)m_pBmInfo, DIB_RGB_COLORS, SRCCOPY);
}

//为Save按钮添加一个响应函数，其代码如下：
void CReadImageDlg::OnBtnSave()
{
	//用保存对话框获取保存路径
	CFileDialog dlg(false, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP files(*.bmp)|*.bmp||");
	if (dlg.DoModal() != IDOK) {
		return;
	}
	//以写二进制数据的方式，打开文件
	CFile dib(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	//写入已经读入的文件头
	dib.Write(m_pBmfh, sizeof(BITMAPFILEHEADER));
	//写入已经读入的位图信息（信息头+调色板（如果有的话））
	dib.Write(m_pBmInfo, m_pBmfh->bfOffBits - 14);
	//下面的程序的目的是计算像素矩阵的大小，就是为了得到height和LineBytes
	BITMAPINFOHEADER bmih;
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	long width = bmih.biWidth;
	int bitCount = bmih.biBitCount;
	long height = bmih.biHeight;
	long LineBytes = (width*bitCount + 31) / 32 * 4;
	//写入已经读入的像素矩阵
	dib.Write(m_pPixel, height*LineBytes);
	//关闭文件
	dib.Close();
}