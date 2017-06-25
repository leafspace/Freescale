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
	BYTE *bmpIhead;                   //因为这个长度不定，需要文件的bfoffBits的配合，所以不直接分配空间
	RGBQUAD bitColor;
	BYTE *bmppelMatrix;               //像素矩阵，大小需要读取图像文件头之后分析才能知道
public:
	BitmapFile(TCHAR *Location = _T("测试图片.bmp")) {
		wcscpy_s(path, Location);
	}
	~BitmapFile() {}
	bool ReadBMPImage();              //读取BMP文件操作
	bool ShowBMPImage();              //显示BMP文件操作
	bool SaveBMPImage();              //保存BMP文件操作
};