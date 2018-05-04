//Download by http://www.NewXing.com
#include "stdafx.h"
#include "Cdib.h"
#include "windowsx.h"
#include "math.h"
#include "comutil.h"

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

CDib::CDib()
{
	size=0;
}

CDib::~CDib()
{
    GlobalFreePtr(m_pBitmapInfo);
}

void CDib::LoadFile(const char* dibFileName)
{
	// Note:strcpy -> strcpy_s 
	strcpy_s(m_fileName,dibFileName);

  
   // 默认文件访问模式为 CFile::modeRead，该模式为只读模式。 
   // CFile dibFile(m_fileName, CFile::modeRead);
   // Note:ChenQP

	// 这样在多字节或UNICODE模式下都可以。
	// _bstr_t bstrTmp(m_fileName);
	//LPCTSTR strTmp = (LPTSTR)bstrTmp;

 //   CFile dibFile((LPCTSTR)strTmp, CFile::modeRead);

	// char* 转换成 LPCTSTR
	int num = MultiByteToWideChar(0, 0, dibFileName, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, dibFileName, -1, wide, num);

	CFile dibFile(wide, CFile::modeRead);
	//CFile dibFile(_T("E:\\part2.bmp"), CFile::modeRead);

//	CFile dibFile;
//	dibFile.Open(_T("E:\\part1.bmp"), CFile::modeRead, NULL);
//   CFile dibFile(_T("E:\\part1.bmp"), CFile::modeRead);

	dibFile.Read((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
    //dibFile.Read((void*)&bitmapFileHeader,sizeof(BITMAPFILEHEADER));
    if (bitmapFileHeader.bfType == 0x4d42)
    {
		// NOte:强制转换 (DWORD)
        DWORD fileLength = (DWORD)dibFile.GetLength();
         size = fileLength -sizeof(BITMAPFILEHEADER);
         pDib =(BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, size);
        dibFile.Read((void*)pDib, size);
        dibFile.Close();
        m_pBitmapInfo = (BITMAPINFO*) pDib;
        m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pDib;
        m_pRGB = (RGBQUAD*)(pDib +
			m_pBitmapInfoHeader->biSize);
        int m_numberOfColors = GetNumberOfColors();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed =
			    m_numberOfColors;
        DWORD colorTableSize = m_numberOfColors *
            sizeof(RGBQUAD);
        m_pData = pDib + m_pBitmapInfoHeader->biSize
            + colorTableSize;
		if (m_pRGB == (RGBQUAD*)m_pData) // No color table
			m_pRGB = NULL;
        m_pBitmapInfoHeader->biSizeImage = GetSize();
		m_valid = TRUE;
    }    
    else
    {
        m_valid = FALSE;
		// Note:_T
        AfxMessageBox(_T("This isn't a bitmap file!"));
    }
}

BOOL CDib::IsValid()
{
    return m_valid;
}
        
char* CDib::GetFileName()
{
    return m_fileName;
}
        
UINT CDib::GetWidth()
{
    return (UINT) m_pBitmapInfoHeader->biWidth;
}
        
UINT CDib::GetHeight()
{
    return (UINT) m_pBitmapInfoHeader->biHeight;
}
        
DWORD CDib::GetSize()
{
    if (m_pBitmapInfoHeader->biSizeImage != 0)
        return m_pBitmapInfoHeader->biSizeImage;
	else
    {
        DWORD height = (DWORD) GetHeight();
        DWORD width = (DWORD) GetWidth();
        return height * width;
    }
}

void CDib::SetWidth(UINT width)
{
	m_pBitmapInfoHeader->biWidth=width;
}

// NOte:CStringToCharArray
char * CDib::CStringToCharArray(CString str)
{
	char *ptr;
#ifdef _UNICODE
	LONG len;
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	ptr = new char[len + 1];
	memset(ptr, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, str, -1, ptr, len + 1, NULL, NULL);
#else
	ptr = new char[str.GetAllocLength() + 1];
	sprintf(ptr, _T("%s"), str);
#endif
	return ptr;
}

void CDib::SetHeight(UINT height)
{
	m_pBitmapInfoHeader->biHeight=height;
}

UINT CDib::GetNumberOfColors()
{
	int numberOfColors;

    if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
          (m_pBitmapInfoHeader->biBitCount < 9))
	{
		switch (m_pBitmapInfoHeader->biBitCount)
		{
		    case 1: numberOfColors = 2; break;
		    case 4: numberOfColors = 16; break;
		    case 8: numberOfColors = 256;
		}
	}
    else
		numberOfColors = (int) m_pBitmapInfoHeader->biClrUsed;

    return numberOfColors;
}
    
BYTE* CDib::GetData()
{
    return m_pData;
}

RGBQUAD* CDib::GetRGB()
{
    return m_pRGB;
}

BITMAPINFO* CDib::GetInfo()
{
    return m_pBitmapInfo;
}

WORD CDib::PaletteSize(LPBYTE lpDIB) 
{ 
    return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 

WORD CDib::DIBNumColors(LPBYTE lpDIB) 
{ 
    WORD wBitCount;  // DIB bit count 
    wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
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

void CDib::SaveFile(const CString filename)
{ 
	// Note:strcpy -> strcpy_s,filename -> (char*)(LPCTSTR)filename
	strcpy_s(m_fileName, (char*)(LPCTSTR)filename);

    // CFile dibFile(m_fileName, CFile::modeCreate|CFile::modeWrite);
	// Note:ChenQP
	CFile dibFile((LPCTSTR)m_fileName, CFile::modeCreate | CFile::modeWrite);
    dibFile.Write((void*)&bitmapFileHeader,sizeof(BITMAPFILEHEADER));
    dibFile.Write((void*)pDib, size);
    dibFile.Close();

}

