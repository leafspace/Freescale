#include "stdafx.h"
#include "mydib.h"

/*************************************************************************
					      方块编码子程序
*************************************************************************/
void rectcoding(HDIB hDIB,int wRect,int hRect)
{
	// 指向源图像象素的指针
	unsigned char *	lpSrc;
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	//锁定并获得位图句柄的地址
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		::AfxMessageBox ("目前只支持256色位图的运算！");
		
		// 返回
		return;
	}

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	//图像处理操作

	//方块编码

	//获得图像的宽度和高度
	LONG lWidth=::DIBWidth ((char*)lpDIB);
	LONG lHeight=::DIBHeight ((char*)lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);

	if(lWidth!=lHeight)
	{
		// 提示用户
		::AfxMessageBox("只能对宽度和高度相等的图像进行方块编码！");

		// 返回
		return;
	}

	//编码过程

	//循环变量
	LONG i,j,k;
	int ii,jj;
	double* Xt=new double [lWidth*lHeight/(hRect*wRect)];
	int Xi;
	int* a0=new int [lWidth*lHeight/(hRect*wRect)];
	int* a1=new int [lWidth*lHeight/(hRect*wRect)];
	int* N0=new int [lWidth*lHeight/(hRect*wRect)];
	int* N1=new int [lWidth*lHeight/(hRect*wRect)];

	//计算阈值
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			Xt[k]=0;
			//计算Xt
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // 指向图像指针
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					Xt[k]+=Xi;
				}	
			Xt[k]/=hRect*wRect;
			k++;	
		}		
	}

	//计算N0、N1
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			N0[k]=0;
			N1[k]=0;
			//计算N0、N1
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // 指向图像指针
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) N0[k]++;
					else N1[k]++;
				}	
			k++;	
		}	
	}

	//计算a0、a1
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			a0[k]=0;
			a1[k]=0;
			//计算N0、N1
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // 指向图像指针
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) a0[k]+=Xi;
					else a1[k]+=Xi;
				}	
			if(N0[k]!=0&&N1[k]!=0)
			{	a0[k]/=N0[k];
				a1[k]/=N1[k];
			}	
			k++;	
		}		
	}

	//二值化编码过程
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // 指向图像指针
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) Xi=a0[k];
					else Xi=a1[k];
					//赋新的像素值给像素
					*lpSrc=Xi;
				}		
			k++;	
		}		
	}
	::GlobalUnlock((HGLOBAL) hDIB);
}