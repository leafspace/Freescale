#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>
#include <math.h>

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

#ifdef _MAC
#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))
void ByteSwapHeader(BITMAPFILEHEADER* bmiHeader);
void ByteSwapInfo(LPSTR lpHeader, BOOL fWin30Header);
#endif

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP

   ::GlobalUnlock((HGLOBAL) hDIB);

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}




BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return FALSE;

   lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = pPal->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bResult;
}



LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}





DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}





WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}


HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}
void WINAPI ConvertShort(unsigned short *array, long length)
{
   unsigned long b1, b2;
   unsigned char *ptr;

   ptr = (unsigned char *)array;
   while (length--) {
      b1 = *ptr++;
      b2 = *ptr++;
      *array++ = (unsigned short)((b1 << 8) | (b2));
   }
}

void WINAPI ConvertLong(unsigned long  *array, long length)
{
   unsigned long b1, b2, b3, b4;
   unsigned char *ptr;

   ptr = (unsigned char *)array;
   while (length--) {
      b1 = *ptr++;
      b2 = *ptr++;
      b3 = *ptr++;
      b4 = *ptr++;
      *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
   }
}




BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDib);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"


	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
#ifdef _MAC
	ByteSwapHeader(&bmfHdr);

	// First swap the size field
	*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));

	// Now swap the rest of the structure (we don't save < Win30 files)
	ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif

	TRY
	{
		// Write the file header
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		//file.WriteHuge(lpBI, dwDIBSize);                                            ERROR!!
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
#ifdef _MAC
		// Swap everything back
		*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));
		ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif
		::GlobalUnlock((HGLOBAL) hDib);
		THROW_LAST();
	}
	END_CATCH

#ifdef _MAC
	// Swap everything back
	*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));
	ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif

	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}


HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	/*
	 * get length of DIB in bytes for use when reading
	 */

	dwBitsSize = file.GetLength();

	/*
	 * Go read the DIB file header and check if it's valid.
	 */
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

#ifdef _MAC
	ByteSwapHeader(&bmfHeader);
#endif
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return NULL;

	/*
	 * Allocate memory for DIB
	 */
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/*
	 * Go read the bits.
	 */
	if(file.Read(pDIB,dwBitsSize-sizeof(BITMAPFILEHEADER))!=dwBitsSize-sizeof(BITMAPFILEHEADER))
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
#ifdef _MAC
	// First swap the size field
	*((LPDWORD)pDIB) = SWAPLONG(*((LPDWORD)pDIB));

	// Now swap the rest of the structure
	ByteSwapInfo(pDIB, IS_WIN30_DIB(pDIB));
#endif
	::GlobalUnlock((HGLOBAL) hDIB);
	return hDIB;
}
//  add Nov.27,1999
HDIB WINAPI NewDIB(long width, long height,unsigned short biBitCount) // add "biBitCount" by hchen Jan 25,00
{
	long dwindth = (width*biBitCount/8+3)/4*4; // align 4 bytes
	WORD color_num;
	switch(biBitCount)
	{
		case 1:
			color_num=2;
			break;

		case 4:
			color_num=16;
			break;
		case 8:
			color_num=256;
			break;
		default:
			color_num=0;
			break;
	}

	long dwBitsSize = dwindth *height + 40 + color_num*4;// 40 LPBIMHEAD

	LPSTR pDIB;
	HDIB hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	LPBITMAPINFO lpmf = (LPBITMAPINFO)pDIB;
	lpmf->bmiHeader.biSize = 40;//40
	lpmf->bmiHeader.biWidth = width; //i_info.width;
    lpmf->bmiHeader.biHeight = height;
    lpmf->bmiHeader.biPlanes = 1;
	lpmf->bmiHeader.biBitCount = biBitCount;// 24
	lpmf->bmiHeader.biCompression = 0;
    lpmf->bmiHeader.biSizeImage = dwindth *height;
	lpmf->bmiHeader.biXPelsPerMeter = 2925;
    lpmf->bmiHeader.biYPelsPerMeter = 2925;
    lpmf->bmiHeader.biClrUsed = 0; 
	lpmf->bmiHeader.biClrImportant= 0; 	

	if(color_num!=0)
	{
		for(int i=0;i<color_num;i++)
		{
			lpmf->bmiColors[i].rgbRed =(BYTE)i;
			lpmf->bmiColors[i].rgbGreen =(BYTE)i;
			lpmf->bmiColors[i].rgbBlue =(BYTE)i;
		}
	}

	::GlobalUnlock((HGLOBAL) hDIB);
	
	return hDIB;
}

#ifdef _MAC
void ByteSwapHeader(BITMAPFILEHEADER* bmfHeader)
{
	bmfHeader->bfType = SWAPWORD(bmfHeader->bfType);
	bmfHeader->bfSize = SWAPLONG(bmfHeader->bfSize);
	bmfHeader->bfOffBits = SWAPLONG(bmfHeader->bfOffBits);
}


void ByteSwapInfo(LPSTR lpHeader, BOOL fWin30Header)
{
	// Note this doesn't swap the bcSize/biSize field.  It assumes that the
	// size field was swapped during read or while setting the fWin30Header
	// flag.

	if (fWin30Header)
	{
		LPBITMAPINFOHEADER lpBMIH = &(LPBITMAPINFO(lpHeader)->bmiHeader);

		//lpBMIH->biSize = SWAPLONG(lpBMIH->biSize);
		lpBMIH->biWidth = SWAPLONG(lpBMIH->biWidth);
		lpBMIH->biHeight = SWAPLONG(lpBMIH->biHeight);
		lpBMIH->biPlanes = SWAPWORD(lpBMIH->biPlanes);
		lpBMIH->biBitCount = SWAPWORD(lpBMIH->biBitCount);
		lpBMIH->biCompression = SWAPLONG(lpBMIH->biCompression);
		lpBMIH->biSizeImage = SWAPLONG(lpBMIH->biSizeImage);
		lpBMIH->biXPelsPerMeter = SWAPLONG(lpBMIH->biXPelsPerMeter);
		lpBMIH->biYPelsPerMeter = SWAPLONG(lpBMIH->biYPelsPerMeter);
		lpBMIH->biClrUsed = SWAPLONG(lpBMIH->biClrUsed);
		lpBMIH->biClrImportant = SWAPLONG(lpBMIH->biClrImportant);
	}
	else
	{
		LPBITMAPCOREHEADER lpBMCH = &(LPBITMAPCOREINFO(lpHeader)->bmciHeader);

		lpBMCH->bcWidth = SWAPWORD(lpBMCH->bcWidth);
		lpBMCH->bcHeight = SWAPWORD(lpBMCH->bcHeight);
		lpBMCH->bcPlanes = SWAPWORD(lpBMCH->bcPlanes);
		lpBMCH->bcBitCount = SWAPWORD(lpBMCH->bcBitCount);
	}
}

#endif


