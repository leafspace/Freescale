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
	bool Thresholding8Bit();          //8λλͼ����
	bool Thresholding24Bit();         //24λ���ɫλͼ����
	int AverageMatrix8Bit();          //��8λͼ������ƽ��ֵ
	int AverageMatrix24Bit();         //��24λͼ������ƽ��ֵ
public:
	BitmapFile(TCHAR *Location = _T("����ͼƬ.bmp")) {
		wcscpy_s(path, Location);
	}
	~BitmapFile() {}
	bool ReadBMPImage();              //��ȡBMP�ļ�����
	bool ShowBMPImage();              //��ʾBMP�ļ�����
	bool SaveBMPImage();              //����BMP�ļ�����
	bool Thresholding();              //��ֵ��BMP�ļ�����
};