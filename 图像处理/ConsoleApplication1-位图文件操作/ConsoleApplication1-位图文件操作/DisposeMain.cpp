#include <stdio.h>
#include "BitmapFile.h"
int main()
{
	BitmapFile BMP;
	BMP.ReadBMPImage();
	BMP.SaveBMPImage();
	return 0;
}