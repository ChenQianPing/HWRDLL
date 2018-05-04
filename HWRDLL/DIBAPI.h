// dibapi.h

#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// DIB句柄
DECLARE_HANDLE(HDIB);

// DIB常量
#define PALVERSION   0x300

/* DIB宏 */

// 判断是否是Win 3.0的DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// 计算矩形区域的宽度
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// 计算矩形区域的高度
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// 在计算图像大小时，采用公式：biSizeImage = biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// Note:ChenQP 
// 函数原型
// ************************************************************************
//  公用 DIB(Independent Bitmap) API函数库：
//
//  PaintDIB()          - 绘制DIB对象
//  FindDIBBits()       - 返回DIB图像象素起始位置
//  DIBWidth()          - 返回DIB宽度
//  DIBHeight()         - 返回DIB高度
//  DIBNumColors()      - 计算DIB调色板颜色数目
//  CopyHandle()        - 拷贝内存块
//
//  SaveDIB()           - 将DIB保存到指定文件中
//  ReadDIBFile()       - 重指定文件中读取DIB对象
//  NewDIB()            - 根据提供的宽、高、颜色位数来创建一个新的DIB
//
//  PaletteSize()       - 返回DIB调色板大小
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
