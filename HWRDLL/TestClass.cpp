#include "stdafx.h"
#include "TestClass.h"

#include "windowsx.h"
#include "math.h"
#include "comutil.h"

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 


TestClass::TestClass()
{
	size = 0;
}


TestClass::~TestClass()
{
	GlobalFreePtr(m_pBitmapInfo);
}

void TestClass::LoadFile(const char* dibFileName)
{
	// Note:strcpy -> strcpy_s 
	strcpy_s(m_fileName, dibFileName);


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
		size = fileLength - sizeof(BITMAPFILEHEADER);
		pDib = (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, size);
		dibFile.Read((void*)pDib, size);
		dibFile.Close();
		m_pBitmapInfo = (BITMAPINFO*)pDib;
		m_pBitmapInfoHeader = (BITMAPINFOHEADER*)pDib;
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

BOOL TestClass::IsValid()
{
	return m_valid;
}

char* TestClass::GetFileName()
{
	return m_fileName;
}

UINT TestClass::GetWidth()
{
	return (UINT)m_pBitmapInfoHeader->biWidth;
}

UINT TestClass::GetHeight()
{
	return (UINT)m_pBitmapInfoHeader->biHeight;
}

DWORD TestClass::GetSize()
{
	if (m_pBitmapInfoHeader->biSizeImage != 0)
		return m_pBitmapInfoHeader->biSizeImage;
	else
	{
		DWORD height = (DWORD)GetHeight();
		DWORD width = (DWORD)GetWidth();
		return height * width;
	}
}

void TestClass::SetWidth(UINT width)
{
	m_pBitmapInfoHeader->biWidth = width;
}

// NOte:CStringToCharArray
char * TestClass::CStringToCharArray(CString str)
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

void TestClass::SetHeight(UINT height)
{
	m_pBitmapInfoHeader->biHeight = height;
}

UINT TestClass::GetNumberOfColors()
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
		numberOfColors = (int)m_pBitmapInfoHeader->biClrUsed;

	return numberOfColors;
}

BYTE* TestClass::GetData()
{
	return m_pData;
}

RGBQUAD* TestClass::GetRGB()
{
	return m_pRGB;
}

BITMAPINFO* TestClass::GetInfo()
{
	return m_pBitmapInfo;
}

WORD TestClass::PaletteSize(LPBYTE lpDIB)
{
	return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}

WORD TestClass::DIBNumColors(LPBYTE lpDIB)
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

void TestClass::SaveFile(const CString filename)
{
	// Note:strcpy -> strcpy_s,filename -> (char*)(LPCTSTR)filename
	strcpy_s(m_fileName, (char*)(LPCTSTR)filename);

	// CFile dibFile(m_fileName, CFile::modeCreate|CFile::modeWrite);
	// Note:ChenQP
	CFile dibFile((LPCTSTR)m_fileName, CFile::modeCreate | CFile::modeWrite);
	dibFile.Write((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
	dibFile.Write((void*)pDib, size);
	dibFile.Close();

}


/*====================GetFeature==========================*/
//
///***************************************************************
//*	函数名称：GetPosition()
//*   函数类型：void
//*   函数功能：搜索手写数字的位置，赋值给bottom,down,right,left
//****************************************************************/
//void TestClass::GetPosition()
//{
//	width = GetWidth();
//	height = GetHeight();
//	LineBytes = (width * 8 + 31) / 32 * 4;
//
//	int i, j;
//	BOOL flag;
//	for (j = 0; j<height; j++)
//	{
//		flag = FALSE;
//		for (i = 0; i<width; i++)
//			if (m_pData[j*LineBytes + i] == 0)
//			{
//				flag = TRUE;
//				break;
//			}
//		if (flag)
//			break;
//	}
//	bottom = j;
//	for (j = height - 1; j>0; j--)
//	{
//		flag = FALSE;
//		for (i = 0; i<width; i++)
//			if (m_pData[j*LineBytes + i] == 0)
//			{
//				flag = TRUE;
//				break;
//			}
//		if (flag)
//			break;
//	}
//	top = j;
//	for (i = 0; i<width; i++)
//	{
//		flag = FALSE;
//		for (j = 0; j<height; j++)
//			if (m_pData[j*LineBytes + i] == 0)
//			{
//				flag = TRUE;
//				break;
//			}
//		if (flag)
//			break;
//	}
//	left = i;
//	for (i = width - 1; i>0; i--)
//	{
//		flag = FALSE;
//		for (j = 0; j<height; j++)
//			if (m_pData[j*LineBytes + i] == 0)
//			{
//				flag = TRUE;
//				break;
//			}
//		if (flag)
//			break;
//	}
//	right = i;
//}
//
///***************************************************************
//*   函数名称：SetFeature()
//*   函数类型：void
//*   函数功能：将手写数字特征保存在变量testsample中
//****************************************************************/
//void TestClass::SetFeature()
//{
//	int i, j;
//	for (j = 0; j<5; j++)
//	{
//		for (i = 0; i<5; i++)
//		{
//			testsample[5 * (4 - j) + i] = Cal(j, i);//(Cal(j,i)>0.10)?1:0;//
//		}
//	}
//}
//
///****************************************************************
//*   函数名称：Cal(int row, int col)
//*   函数类型：double
//*   参数说明：int row, int col:第row行，第col个区域
//*   函数功能：计算某一小区域内黑像素所占比例,返回某一小区域内黑像素所占比例
//****************************************************************/
//double TestClass::Cal(int row, int col)
//{
//	double w, h, count;
//	w = (right - left) / 5;
//	h = (top - bottom) / 5;
//	count = 0;
//
//	for (int j = bottom + row*h; j<bottom + (row + 1)*h; j++)
//		for (int i = left + col*w; i<left + (col + 1)*w; i++)
//		{
//			if (m_pData[j*LineBytes + i] == 0)
//				count++;
//		}
//
//	return (double)count / (w*h);
//}
//
///***************************************************************
//*   函数名称：Savealbe(int cls)
//*   函数类型：BOOL
//*   参数说明：int cls:类别
//*   函数功能：判断手写数字可否保存为cls类别的一个样品
//*             同一类别的样品不能有重复的
//****************************************************************/
//BOOL TestClass::Saveable(int cls)
//{
//	for (int i = 0; i<pattern[cls].number; i++)
//	{
//		BOOL flag = TRUE;
//		for (int j = 0; j<25; j++)
//			flag &= (pattern[cls].feature[i][j] == testsample[j]) ? TRUE : FALSE;
//		if (flag) return FALSE;
//	}
//	return TRUE;
//}
//
///***************************************************************
//*   函数名称：Save(int cls)
//*   函数类型：void
//*   参数说明：int cls:类别
//*   函数功能：将手写数字保存为cls类别的一个样品，保存在样品的第一个
//****************************************************************/
//void TestClass::Save(int cls)
//{
//	for (int i = pattern[cls].number; i>0; i--)
//		for (int j = 0; j<25; j++)
//			pattern[cls].feature[i][j] = pattern[cls].feature[i - 1][j];
//	for (int i = 0; i<25; i++)
//		pattern[cls].feature[0][i] = testsample[i];
//	pattern[cls].number++;
//}


///*====================Classification==========================*/
//
///******************************************************************
//*   函数名称：LeastDistance()
//*   函数类型：number_no，结构体
//*   函数功能：最小距离法 ,返回数字类别和编号
//******************************************************************/
//new_number_no TestClass::LeastDistance()
//{
//
//	double min = 10000000000;
//	new_number_no number_no;
//	for (int n = 0; n<10; n++)
//	{
//		for (int i = 0; i<pattern[n].number; i++)
//		{
//			if (pipei(pattern[n].feature[i], testsample)<min)
//			{
//				// 匹配的最小值
//				min = pipei(pattern[n].feature[i], testsample);
//				number_no.number = n;   // 样品类别
//				number_no.no = i;       // 样品序号
//			}
//		}
//	}
//	return number_no;// 返回手写数字的类别和序号
//}
//
//int TestClass::GetNumberByLeastDistance()
//{
//	double min = 10000000000;
//	new_number_no number_no;
//	for (int n = 0; n<10; n++)
//	{
//		for (int i = 0; i<pattern[n].number; i++)
//		{
//			if (pipei(pattern[n].feature[i], testsample)<min)
//			{
//				//匹配的最小值
//				min = pipei(pattern[n].feature[i], testsample);
//				number_no.number = n;//样品类别
//				number_no.no = i;//样品序号
//			}
//		}
//	}
//
//	return number_no.number;//返回手写数字的类别和序号
//}
//
//
///****************************************************************
//*   函数名称：pipei(double s1[], double s2[])
//*   函数类型：double
//*   参数说明：double s1[], double s2[]:两个样品的特征
//*   函数功能：计算两个样品的匹配程度 ,返回两各样品的的匹配程度。
//****************************************************************/
//double TestClass::pipei(double s1[], double s2[])
//{
//	double count = 0.0;
//	for (int i = 0; i<25; i++)
//	{
//		count += (s1[i] - s2[i])*(s1[i] - s2[i]);
//	}
//	return count;
//}
