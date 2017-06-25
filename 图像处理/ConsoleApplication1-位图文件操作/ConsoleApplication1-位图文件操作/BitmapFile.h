#pragma once
#define _AFXDLL
#include <afx.h>
#include <string>
using namespace std;
class BitmapFile
{
private:
	TCHAR path[30];
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfo;
	BYTE *bmpIhead;                   //��Ϊ������Ȳ�������Ҫ�ļ���bfoffBits����ϣ����Բ�ֱ�ӷ���ռ�
	RGBQUAD bitColor;
	BYTE *bmppelMatrix;               //���ؾ��󣬴�С��Ҫ��ȡͼ���ļ�ͷ֮���������֪��
public:
	BitmapFile(TCHAR *Location = _T("����ͼƬ.bmp")) {
		wcscpy_s(path, Location);
	}
	~BitmapFile() {}
	bool ReadBMPImage();              //��ȡBMP�ļ�����
	bool ShowBMPImage();              //��ʾBMP�ļ�����
	bool SaveBMPImage();              //����BMP�ļ�����
};