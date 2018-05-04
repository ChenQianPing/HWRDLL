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


	// Ĭ���ļ�����ģʽΪ CFile::modeRead����ģʽΪֻ��ģʽ�� 
	// CFile dibFile(m_fileName, CFile::modeRead);
	// Note:ChenQP

	// �����ڶ��ֽڻ�UNICODEģʽ�¶����ԡ�
	// _bstr_t bstrTmp(m_fileName);
	//LPCTSTR strTmp = (LPTSTR)bstrTmp;

	//   CFile dibFile((LPCTSTR)strTmp, CFile::modeRead);

	// char* ת���� LPCTSTR
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
		// NOte:ǿ��ת�� (DWORD)
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
//*	�������ƣ�GetPosition()
//*   �������ͣ�void
//*   �������ܣ�������д���ֵ�λ�ã���ֵ��bottom,down,right,left
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
//*   �������ƣ�SetFeature()
//*   �������ͣ�void
//*   �������ܣ�����д�������������ڱ���testsample��
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
//*   �������ƣ�Cal(int row, int col)
//*   �������ͣ�double
//*   ����˵����int row, int col:��row�У���col������
//*   �������ܣ�����ĳһС�����ں�������ռ����,����ĳһС�����ں�������ռ����
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
//*   �������ƣ�Savealbe(int cls)
//*   �������ͣ�BOOL
//*   ����˵����int cls:���
//*   �������ܣ��ж���д���ֿɷ񱣴�Ϊcls����һ����Ʒ
//*             ͬһ������Ʒ�������ظ���
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
//*   �������ƣ�Save(int cls)
//*   �������ͣ�void
//*   ����˵����int cls:���
//*   �������ܣ�����д���ֱ���Ϊcls����һ����Ʒ����������Ʒ�ĵ�һ��
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
//*   �������ƣ�LeastDistance()
//*   �������ͣ�number_no���ṹ��
//*   �������ܣ���С���뷨 ,�����������ͱ��
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
//				// ƥ�����Сֵ
//				min = pipei(pattern[n].feature[i], testsample);
//				number_no.number = n;   // ��Ʒ���
//				number_no.no = i;       // ��Ʒ���
//			}
//		}
//	}
//	return number_no;// ������д���ֵ��������
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
//				//ƥ�����Сֵ
//				min = pipei(pattern[n].feature[i], testsample);
//				number_no.number = n;//��Ʒ���
//				number_no.no = i;//��Ʒ���
//			}
//		}
//	}
//
//	return number_no.number;//������д���ֵ��������
//}
//
//
///****************************************************************
//*   �������ƣ�pipei(double s1[], double s2[])
//*   �������ͣ�double
//*   ����˵����double s1[], double s2[]:������Ʒ������
//*   �������ܣ�����������Ʒ��ƥ��̶� ,����������Ʒ�ĵ�ƥ��̶ȡ�
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
