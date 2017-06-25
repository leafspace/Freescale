#include "BitmapFile.h"
#include <windows.h>                   //支持WINGDI
#include <afx.h>                       //支持CFILE
//#define NormBit 128
int NormBit = 128;

bool BitmapFile::Thresholding8Bit()
{
	//1.方法一，扫描每个像素值，小于127的像素数值设置为0其余为1
	BYTE *BMPTemp = bmppelMatrix;
	for (DWORD i = 0; i < bitInfo.biSizeImage; ++i) {
		if (*BMPTemp < NormBit) {
			*BMPTemp = 0;
		}
		else {
			*BMPTemp = 255;
		}
		++BMPTemp;
	}
	//=====================================================
	return true;
}

bool BitmapFile::Thresholding24Bit()
{
	//1.方法一，扫描每个像素值，小于127的像素数值设置为0其余为1
	BYTE *BMPTemp = bmppelMatrix;
	for (DWORD i = 0; i < bitInfo.biSizeImage; i += 3) {
		int Temp = *BMPTemp + *(BMPTemp + 1) + *(BMPTemp + 2);
		if (Temp / 3 < NormBit) {
			*BMPTemp++ = 0;
			*BMPTemp++ = 0;
			*BMPTemp++ = 0;
		}
		else {
			*BMPTemp++ = 255;
			*BMPTemp++ = 255;
			*BMPTemp++ = 255;
		}
	}
	//=====================================================
	return true;
}

int BitmapFile::AverageMatrix8Bit()
{
	BYTE *BMPTemp = bmppelMatrix;
	DWORD i = 0;
	int Average = 0;
	for (; i < bitInfo.biSizeImage; ++i) {
		Average += *BMPTemp;
		BMPTemp++;
	}
	return Average / i;
}

int BitmapFile::AverageMatrix24Bit()
{
	BYTE *BMPTemp = bmppelMatrix;
	DWORD i = 0;
	int Average = 0;
	for (; i < bitInfo.biSizeImage; i += 3) {
		int Temp = *BMPTemp + *(BMPTemp + 1) + *(BMPTemp + 2);
		Average += Temp;
		BMPTemp += 3;
	}
	return Average / i / 3;
	return 0;
}

bool BitmapFile::ReadBMPImage()
{
	
	CFile BMPFile(path, CFile::modeRead);                              //新建一个CFile类的对象，以只读的模式打开  //(CFile对象在构造的时候会返回bool的判断值)
	BMPFile.Read(&bitHead, sizeof(BITMAPFILEHEADER));                  //读入位图文件头
	bmpIhead = new BYTE[bitHead.bfOffBits - 14];                       //为位图头信息分配空间      14为头文件大小
	BMPFile.Read(bmpIhead, bitHead.bfOffBits - 14);                    //读入位图头信息
	memcpy(&bitInfo, bmpIhead, sizeof(BITMAPINFOHEADER));              //拷贝一个信息头大小的字节到该变量中
	bmppelMatrix = new BYTE[bitInfo.biSizeImage];                      //为像素矩阵分配空间
	BMPFile.Read(bmppelMatrix, bitInfo.biSizeImage);                   //读入位图的像素矩阵
	BMPFile.Close();                                                   //关闭文件
	return false;
}

bool BitmapFile::ShowBMPImage()
{
	BITMAPINFOHEADER bmpIheadTemp;
	memcpy(&bmpIheadTemp, bmpIhead, sizeof(BITMAPINFOHEADER));
	LONG width = bmpIheadTemp.biWidth;
	LONG height = bmpIheadTemp.biHeight;
	//StretchDIBits(GetDC()->m_hDC, 0, 0, width, height, 0, 0, width, height, m_pPixel, (BITMAPINFO*)m_pBmInfo, DIB_RGB_COLORS, SRCCOPY);                                  //关于show的目标目前不知道
	return false;
}

bool BitmapFile::SaveBMPImage()
{
	CFile BMPFile(TEXT("结果图片.bmp"), CFile::modeCreate | 
		CFile::modeWrite | CFile::typeBinary);                         //以写二进制数据的方式，打开文件
	BMPFile.Write(&bitHead, sizeof(BITMAPFILEHEADER));                 //写入已经读入的文件头
	BMPFile.Write(bmpIhead, bitHead.bfOffBits - 14);                   //写入已经读入的位图信息
	BMPFile.Write(bmppelMatrix, bitInfo.biSizeImage);                  //写入已经读入的像素矩阵
	BMPFile.Close();                                                   //关闭文件
	return false;
}

bool BitmapFile::Thresholding()
{
	//1.方法一，扫描每个像素值，小于127的像素数值设置为0其余为1
	if (bitInfo.biBitCount == 8) {
		Thresholding8Bit();
	}
	else {
		Thresholding24Bit();
	}
	//=====================================================
	//2.方法二，采用求平均值的方式，求出NormBit
	/*NormBit = AverageMatrix8Bit();
	if (NormBit < 0) {
		NormBit = 128;
	}
	if (bitInfo.biBitCount == 8) {
		Thresholding8Bit();
	}
	else {
		Thresholding24Bit();
	}*/
	printf("Dispose ok!\n");
	return true;
}


