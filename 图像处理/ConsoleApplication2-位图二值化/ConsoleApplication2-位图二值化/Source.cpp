#include "BitmapFile.h"
int main()
{
	BitmapFile BMP;
	BMP.ReadBMPImage();
	BMP.Thresholding();
	BMP.SaveBMPImage();
	return 0;
}