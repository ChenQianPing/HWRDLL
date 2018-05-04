// ************************************************************************
//  �ļ�����dibapi.cpp
//
//  ���� DIB(Independent Bitmap) API�����⣺
//
//  PaintDIB()          - ����DIB����
//  FindDIBBits()       - ����DIBͼ��������ʼλ��
//  DIBWidth()          - ����DIB����
//  DIBHeight()         - ����DIB�߶�
//  DIBNumColors()      - ����DIB��ɫ����ɫ��Ŀ
//  CopyHandle()        - �����ڴ��
//
//  SaveDIB()           - ��DIB���浽ָ���ļ���
//  ReadDIBFile()       - ��ָ���ļ��ж�ȡDIB����
//  NewDIB()            - �����ṩ�Ŀ����ߡ���ɫλ��������һ���µ�DIB
//
//  PaletteSize()       - ����DIB��ɫ���С
//
// ************************************************************************

#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

/*
 * Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/*************************************************************************
 *
 * �������ƣ�
 *   PaintDIB()
 *
 * ����:
 *   HDC hDC            - ����豸DC
 *   LPRECT lpDCRect    - ���ƾ�������
 *   HDIB hDIB          - ָ��DIB�����ָ��
 *   LPRECT lpDIBRect   - Ҫ�����DIB����
 *   CPalette* pPal     - ָ��DIB�����ɫ���ָ��
 *
 * ����ֵ:
 *   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����Ҫ��������DIB�������е�����StretchDIBits()����
 * SetDIBitsToDevice()������DIB����������豸���ɲ���hDCָ
 * �������Ƶľ��������ɲ���lpDCRectָ�������DIB�������ɲ���
 * lpDIBRectָ����
 *
 ************************************************************************/

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // BITMAPINFOHEADERָ��
	LPSTR    lpDIBBits;           // DIB����ָ��
	BOOL     bSuccess=FALSE;      // �ɹ���־
	HPALETTE hPal=NULL;           // DIB��ɫ��
	HPALETTE hOldPal=NULL;        // ��ǰ�ĵ�ɫ��

	// �ж�DIB�����Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		// ����
		return FALSE;
	}

	// ����DIB
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// ��ȡDIB��ɫ�壬��ѡ����
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// ѡ�е�ɫ��
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	// ������ʾģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
	{
		// ԭʼ��С���������졣
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
	}
    else
	{
		// ��ԭʼ��С�����졣
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
	}
	
    // �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// �ָ���ǰ�ĵ�ɫ��
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	// ����
	return bSuccess;
}


/*************************************************************************
 *
 * �������ƣ�
 *   FindDIBBits()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   LPSTR              - ָ��DIBͼ��������ʼλ��
 *
 * ˵��:
 *   �ú�������DIB��ͼ�����ص���ʼλ�ã�������ָ������ָ�롣
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBWidth()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIB��ͼ��Ŀ���
 *
 * ˵��:
 *   �ú�������DIB��ͼ��Ŀ��ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biWidthֵ��������������BITMAPCOREHEADER�е�bcWidthֵ��
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ���
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBHeight()
 *
 * ����:
 *   LPSTR lpDIB        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIB��ͼ��ĸ߶�
 *
 * ˵��:
 *   �ú�������DIB��ͼ��ĸ߶ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biHeightֵ��������������BITMAPCOREHEADER�е�bcHeightֵ��
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ���
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}




/*************************************************************************
 *
 * �������ƣ�
 *   DIBNumColors()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   �ú�������DIB�е�ɫ�����ɫ�����������ڵ�ɫλͼ������2��
 * ����16ɫλͼ������16������256ɫλͼ������256���������ɫ
 * λͼ��24λ����û�е�ɫ�壬����0��
 *
 ************************************************************************/
WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��
	
	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;
		
		// ��ȡdwClrUsedֵ
		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		
		if (dwClrUsed != 0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// �������ص�λ��������ɫ��Ŀ
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


/*************************************************************************
 *
 * �������ƣ�
 *   DIBBitCount()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   �ú�������DIBBitCount��
 *
 ************************************************************************/
WORD WINAPI DIBBitCount(LPSTR lpbi)
{
	WORD wBitCount;

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// ����wBitCount
	return wBitCount;
}

/*************************************************************************
 *
 * �������ƣ�
 *   CopyHandle()
 *
 * ����:
 *   HGLOBAL h          - Ҫ���Ƶ��ڴ�����
 *
 * ����ֵ:
 *   HGLOBAL            - ���ƺ�����ڴ�����
 *
 * ˵��:
 *   �ú�������ָ�����ڴ����򡣷��ظ��ƺ�����ڴ����򣬳���ʱ����0��
 *
 ************************************************************************/

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	// ��ȡָ���ڴ������С
	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	
	// �������ڴ�ռ�
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);
	
	// �жϷ����Ƿ�ɹ�
	if (hCopy != NULL)
	{
		// ����
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		
		// ����
		memcpy(lpCopy, lp, dwLen);
		
		// �������
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}



/*************************************************************************
 *
 * �������ƣ�
 *   SaveDIB()
 *
 * ����:
 *   HDIB hDib          - Ҫ�����DIB
 *   CFile& file        - �����ļ�CFile
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE����CFileException
 *
 * ˵��:
 *   �ú�����ָ����DIB���󱣴浽ָ����CFile�С���CFile�ɵ��ó���򿪺͹رա�
 *
 *************************************************************************/


BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// DIB��С
	DWORD dwDIBSize;

	if (hDib == NULL)
	{
		// ���DIBΪ�գ�����FALSE
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	
	if (lpBI == NULL)
	{
		// Ϊ�գ�����FALSE
		return FALSE;
	}
	
	// �ж��Ƿ���WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// ��֧���������͵�DIB����
		
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// ����FALSE
		return FALSE;
	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType = DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
	
	// �ļ�ͷ��С����ɫ����С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);
	
	// ����ͼ���С
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ���صĴ�С
		DWORD dwBmBitsSize;

		// ��СΪWidth * Height
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		
		// �����DIB�����Ĵ�С
		dwDIBSize += dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage = dwBmBitsSize;
	}


	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	
	// ����������
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ����С
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
	// ����д�ļ�
	TRY
	{
		// д�ļ�ͷ
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// дDIBͷ������
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// �׳��쳣
		THROW_LAST();
	}
	END_CATCH
	
	// �������
	::GlobalUnlock((HGLOBAL) hDib);
	
	// ����TRUE
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   ReadDIBFile()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ��ļ�CFile
 *
 * ����ֵ:
 *   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú�����ָ�����ļ��е�DIB�������ָ�����ڴ������С���BITMAPFILEHEADER
 * ������ݶ����������ڴ档
 *
 *************************************************************************/

HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	// ��ȡDIB���ļ������ȣ��ֽڣ�

	// Note:ChenQP  2016.04.20
	// ULONGLONG��64λ�ģ�DWORD��32λ�ģ�ֻҪ�ļ�����4G��ת���ͻ�������ݴ������С�ڣ���û��ϵ��
	// dwBitsSize = file.GetLength();

	// ǿ��ת��
	dwBitsSize = (DWORD)file.GetLength();

	// ���Զ�ȡDIB�ļ�ͷ
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}

	// �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
	{
		// ��DIB���󣬷���NULL��
		return NULL;
	}

	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// ������

	// Note:ChenQP
	// ������������Ľ�������Ѵ����е�ReadHuge����Read��WriteHuge����Write���ɡ�
    // ԭ����VC++2005������VC�汾�е�CFileû��ReadHuge��WriteHuge��Ա������
	// ֻ��VC6�е�CFile���У�����ReadHuge���������ѱ�Read�����������棬
	// WriteHuge���������ѱ�Write�����������档

	if (file.Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// ����DIB���
	return hDIB;
}

/*************************************************************************
 *
 * �������ƣ�
 *   NewDIB()
 *
 * ����:
 *   width			- ��Ҫ����DIB�Ŀ�
 *   height			- ��Ҫ����DIB�ĸ�
 *   biBitCount		- ��Ҫ����DIB��λ�������磬���Ҫ����256ɫDIB�����ֵΪ8
 * ����ֵ:
 *   HDIB           - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú���ָ�������ߡ���ɫλ��������һ���µ�DIB������������
 *************************************************************************/
/*******************************************************************
*�������ƣ�
* NewDIB()
*
* ����:
* width			- ��Ҫ����DIB�Ŀ�
* height			- ��Ҫ����DIB�ĸ�
* biBitCount		- ��Ҫ����DIB��λ�������磬���Ҫ����256ɫDIB�����ֵΪ8
*
*����ֵ:
*  HDIB           - �ɹ�����DIB�ľ�������򷵻�NULL��
*
*˵��:
*  �ú���ָ�������ߡ���ɫλ��������һ���µ�DIB������������
*
***************************************************************/
HDIB WINAPI NewDIB(long width, long height,unsigned short biBitCount) 
{
	
//�����½���DIBÿ����ռ���ֽ���
long dwindth = (width*biBitCount/8+3)/4*4;

//�½���DIB��ɫ���б������Ŀ 
	 WORD color_num;
   
     //ͨ�������biBitCountֵ��ȷ����ɫ��ı�����Ŀ 
	switch(biBitCount)
	{   

       //�����1 bit����ʾһ��������ô��ɫ�������������� 
		case 1:
         color_num=2;
		break;

       //�����4 bit����ʾһ��������ô��ɫ������16������
		case 4:
		color_num=16;
		break;

       //�����8bit����ʾһ�����أ���ô��ɫ���еñ�����256�У������������������ʽ��
		case 8:
		color_num=256;
		break;

       //�����������ɫ����û�б�������λͼ
		default:
		color_num=0;
		break;
	}

//����λͼ������ռ�Ŀռ�
//dwindth *heightΪ����������ռ�Ŀռ�
//40Ϊλͼ��Ϣͷռ�Ŀռ�
//color_num*4Ϊ��ɫ��ı�����ռ�Ŀռ䣨��ɫ��ÿ������ռ4�����ֽڣ�

 long   dwBitsSize = dwindth *height + 40 + color_num*4;

//����ָ��λͼ�ļ���ָ�� 
LPSTR pDIB;

//����洢�ռ䣬������ָ��λͼ�ľ��
HDIB hDIB=(HDIB) ::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, dwBitsSize);

//�������ռ䲻�ɹ����ش�����Ϣ
	if (hDIB == 0)
	{
		return NULL;
	}

   //�������ռ�ɹ������ڴ棬�����ڴ��ָ�봫��pDIB
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
   //����ָ��λͼ��Ϣͷ�ṹ��ָ��
	LPBITMAPINFO lpmf = (LPBITMAPINFO)pDIB;

   //��λͼ��Ϣͷ�ڵĸ���������ֵ

   //ָ��λͼ��Ϣͷ�ṹ�Ĵ�СΪ40�ֽ�
	lpmf->bmiHeader.biSize = 40;

   //ָ���½�λͼ�Ŀ���
	lpmf->bmiHeader.biWidth = width;

   //ָ���½�λͼ�ĸ߶�
lpmf->bmiHeader.biHeight = height;

   //λƽ��������Ϊ1
lpmf->bmiHeader.biPlanes = 1;

//ȷ���½�λͼ��ʾ��ɫ��Ҫ�õ���bit��
lpmf->bmiHeader.biBitCount = biBitCount;

    //�Ƿ����ѹ��
	lpmf->bmiHeader.biCompression = 0;

    //�½���λͼ��ʵ�ʵ�λͼ������ռ���ֽ���
lpmf->bmiHeader.biSizeImage = dwindth *height;

//ָ��Ŀ���豸��ˮƽ�ֱ���
	lpmf->bmiHeader.biXPelsPerMeter = 2925;

    //ָ��Ŀ���豸�Ĵ�ֱ�ֱ���
lpmf->bmiHeader.biYPelsPerMeter = 2925;

//�½�ͼ��ʵ���õ�����ɫ�� ���Ϊ0���õ�����ɫ��Ϊ2��biBitCount��
lpmf->bmiHeader.biClrUsed = 0;
 
//ָ���½�ͼ������Ҫ����ɫ�������Ϊ0�����е���ɫ����Ҫ
	lpmf->bmiHeader.biClrImportant= 0; 	
    
    //����½���ͼ���к��е�ɫ�壬��������Ե�ɫ��ĸ�����ɫ��������ʼֵ
	if(color_num!=0)
	{
		for(int i=0;i<color_num;i++)
		{
			lpmf->bmiColors[i].rgbRed =(BYTE)i;
			lpmf->bmiColors[i].rgbGreen =(BYTE)i;
			lpmf->bmiColors[i].rgbBlue =(BYTE)i;
		}
	}

//�������
::GlobalUnlock((HGLOBAL) hDIB);

//�����½�λͼ�ľ��
	 return hDIB;

}


/*************************************************************************
 *
 * �������ƣ�
 *   PaletteSize()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - DIB�е�ɫ��Ĵ�С
 *
 * ˵��:
 *   �ú�������DIB�е�ɫ��Ĵ�С������Windows 3.0 DIB��������ɫ��Ŀ��
 * RGBQUAD�Ĵ�С����������������ɫ��Ŀ��RGBTRIPLE�Ĵ�С��
 *
 ************************************************************************/
WORD WINAPI PaletteSize(LPSTR lpbi)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB (lpbi))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}