#include "BitmapFile.h"
#include <windows.h>                   //֧��WINGDI
#include <afx.h>                       //֧��CFILE

bool BitmapFile::ReadBMPImage()
{
	
	CFile BMPFile(path, CFile::modeRead);            //�½�һ��CFile��Ķ�����ֻ����ģʽ��  //(CFile�����ڹ����ʱ��᷵��bool���ж�ֵ)
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
