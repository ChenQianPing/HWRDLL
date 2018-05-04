// dibapi.h

#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// DIB���
DECLARE_HANDLE(HDIB);

// DIB����
#define PALVERSION   0x300

/* DIB�� */

// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// �����������Ŀ���
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// �����������ĸ߶�
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// Note:ChenQP 
// ����ԭ��
// ************************************************************************
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
// 

BOOL      WINAPI  PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
LPSTR     WINAPI  FindDIBBits (LPSTR lpbi);
DWORD     WINAPI  DIBWidth (LPSTR lpDIB);
DWORD     WINAPI  DIBHeight (LPSTR lpDIB);
WORD      WINAPI  DIBNumColors (LPSTR lpbi);
WORD	  WINAPI  DIBBitCount(LPSTR lpbi);
HGLOBAL   WINAPI  CopyHandle (HGLOBAL h);

HDIB	  WINAPI  NewDIB(long width, long height,unsigned short biBitCount);
BOOL      WINAPI  SaveDIB (HDIB hDib, CFile& file);
HDIB      WINAPI  ReadDIBFile(CFile& file);

WORD WINAPI PaletteSize(LPSTR lpbi);

#endif //!_INC_DIBAPI