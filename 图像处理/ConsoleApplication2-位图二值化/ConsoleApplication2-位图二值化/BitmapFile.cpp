#include "BitmapFile.h"
#include <windows.h>                   //֧��WINGDI
#include <afx.h>                       //֧��CFILE
//#define NormBit 128
int NormBit = 128;

bool BitmapFile::Thresholding8Bit()
{
	//1.����һ��ɨ��ÿ������ֵ��С��127��������ֵ����Ϊ0����Ϊ1
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
	//1.����һ��ɨ��ÿ������ֵ��С��127��������ֵ����Ϊ0����Ϊ1
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
	
	CFile BMPFile(path, CFile::modeRead);                              //�½�һ��CFile��Ķ�����ֻ����ģʽ��  //(CFile�����ڹ����ʱ��᷵��bool���ж�ֵ)
	BMPFile.Read(&bitHead, sizeof(BITMAPFILEHEADER));                  //����λͼ�ļ�ͷ
	bmpIhead = new BYTE[bitHead.bfOffBits - 14];                       //Ϊλͼͷ��Ϣ����ռ�      14Ϊͷ�ļ���С
	BMPFile.Read(bmpIhead, bitHead.bfOffBits - 14);                    //����λͼͷ��Ϣ
	memcpy(&bitInfo, bmpIhead, sizeof(BITMAPINFOHEADER));              //����һ����Ϣͷ��С���ֽڵ��ñ�����
	bmppelMatrix = new BYTE[bitInfo.biSizeImage];                      //Ϊ���ؾ������ռ�
	BMPFile.Read(bmppelMatrix, bitInfo.biSizeImage);                   //����λͼ�����ؾ���
	BMPFile.Close();                                                   //�ر��ļ�
	return false;
}

bool BitmapFile::ShowBMPImage()
{
	BITMAPINFOHEADER bmpIheadTemp;
	memcpy(&bmpIheadTemp, bmpIhead, sizeof(BITMAPINFOHEADER));
	LONG width = bmpIheadTemp.biWidth;
	LONG height = bmpIheadTemp.biHeight;
	//StretchDIBits(GetDC()->m_hDC, 0, 0, width, height, 0, 0, width, height, m_pPixel, (BITMAPINFO*)m_pBmInfo, DIB_RGB_COLORS, SRCCOPY);                                  //����show��Ŀ��Ŀǰ��֪��
	return false;
}

bool BitmapFile::SaveBMPImage()
{
	CFile BMPFile(TEXT("���ͼƬ.bmp"), CFile::modeCreate | 
		CFile::modeWrite | CFile::typeBinary);                         //��д���������ݵķ�ʽ�����ļ�
	BMPFile.Write(&bitHead, sizeof(BITMAPFILEHEADER));                 //д���Ѿ�������ļ�ͷ
	BMPFile.Write(bmpIhead, bitHead.bfOffBits - 14);                   //д���Ѿ������λͼ��Ϣ
	BMPFile.Write(bmppelMatrix, bitInfo.biSizeImage);                  //д���Ѿ���������ؾ���
	BMPFile.Close();                                                   //�ر��ļ�
	return false;
}

bool BitmapFile::Thresholding()
{
	//1.����һ��ɨ��ÿ������ֵ��С��127��������ֵ����Ϊ0����Ϊ1
	if (bitInfo.biBitCount == 8) {
		Thresholding8Bit();
	}
	else {
		Thresholding24Bit();
	}
	//=====================================================
	//2.��������������ƽ��ֵ�ķ�ʽ�����NormBit
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


