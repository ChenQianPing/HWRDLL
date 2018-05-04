#include "stdafx.h"
#include "MyDibLib.h"



// ������ͷ�ļ�.h���涨��ȫ�ֱ��������������ʱ������cpp�ļ��������ͷ�ļ����涨��ı�������ͻ
// ����һЩ��Ҫ��ȫ�ֱ���
HDIB m_hDIB;
int m_lianXuShu;
int digicount;
// int w_sample = 28;    // ͼƬ���
// int h_sample = 28;    // ͼƬ�߶�
CRectLink m_charRect;
CRectLink m_charRectCopy;
HDIBLink  m_dibRect;
HDIBLink  m_dibRectCopy;


/****************************************************************
* �������ƣ�
*      Convert256toGray()
*
* ������
*     HDIB hDIB     ��ͼ��ľ��
*
*  ����ֵ��
*        ��
*
*  ���ܣ�
*     ��256ɫλͼת��Ϊ�Ҷ�ͼ
*
***************************************************************/
void Convert256toGray(HDIB hDIB)
{
	LPSTR	lpDIB;

	// ��DIB����õ�DIBָ�벢����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

	// ָ��DIB������������ָ��
	LPSTR   lpDIBBits;

	// ָ��DIB���ص�ָ��
	BYTE *	lpSrc;

	// ͼ����
	LONG	lWidth;
	// ͼ��߶�
	LONG  	lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;

	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;

	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;

	// �Ҷ�ӳ���
	BYTE bMap[256];

	// ����Ҷ�ӳ������������ɫ�ĻҶ�ֵ����������DIB��ɫ��
	int	i, j;
	for (i = 0; i < 256; i++)
	{
		// �������ɫ��Ӧ�ĻҶ�ֵ
		bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +

			0.587 * lpbmi->bmiColors[i].rgbGreen +

			0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
		// ����DIB��ɫ���ɫ����
		lpbmi->bmiColors[i].rgbRed = i;

		// ����DIB��ɫ����ɫ����
		lpbmi->bmiColors[i].rgbGreen = i;

		// ����DIB��ɫ����ɫ����
		lpbmi->bmiColors[i].rgbBlue = i;

		// ����DIB��ɫ�屣��λ
		lpbmi->bmiColors[i].rgbReserved = 0;

	}
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	// ��ȡͼ����
	lWidth = ::DIBWidth(lpDIB);

	// ��ȡͼ��߶�
	lHeight = ::DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ����ÿ�����ص���ɫ�����������ջҶ�ӳ����ɻҶ�ֵ��

	//����ɨ��
	for (i = 0; i < lHeight; i++)
	{

		//����ɨ��
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// �任
			*lpSrc = bMap[*lpSrc];
		}
	}

	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
}

/******************************************************************
*
* ��������ConvertGrayToWhiteBlack()
*
* ���� ��HDIB hDIB     ��ԭͼ�ľ��
*
* ����ֵ����
*
* ����: ConvertGrayToWhiteBlack��������Ӳ��ֵ�ķ�����ʵ�ֽ�ͼ���ֵ���Ĺ��ܡ�
*
* ˵����
Ҫ��������ͼƬΪ256ɫ
************************************************************************/
void ConvertGrayToWhiteBlack(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ��DIB����õ�DIBָ�벢����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

	// ָ��DIB������������ָ��
	LPSTR   lpDIBBits;

	// ָ��DIB���ص�ָ��
	BYTE *	lpSrc;

	// ͼ����
	LONG	lWidth;

	// ͼ��߶�
	LONG	lHeight;
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	// ��ȡͼ����
	lWidth = ::DIBWidth(lpDIB);

	// ��ȡͼ��߶�
	lHeight = ::DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ����ÿ�����ص���ɫ�����������ջҶ�ӳ����ɻҶ�ֵ��
	int i, j;

	//����ɨ��
	for (i = 0; i < lHeight; i++)
	{

		//����ɨ��
		for (j = 0; j < lWidth; j++)
		{

			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			// ��ֵ������

			//����220������Ϊ255�����׵�
			if (*lpSrc>220) *lpSrc = 255;

			//��������Ϊ0�����ڵ�
			else *lpSrc = 0;

		}

	}

	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
}

/*****************************************************************
*
*  ��������
*       DeleteScaterJudge()
*
*  ������
*     LPSTR   lpDIBBits      ��ָ��������ʼλ�õ�ָ��
*	  WORD    lLineBytes     ��ͼ��ÿ�е��ֽ���
*     LPBYTE  lplab          ����־λ����
*     int     lWidth         ��ͼ��Ŀ��
*	  int     lHeight        ��ͼ��ĸ߶�
*     int     x              ����ǰ��ĺ�����
*	  int     y              ����ǰ���������
*     CPoint  lab[]          ������鿼���������������
*     int     lianXuShu      ����ɢ����ж�����
*
*  ����ֵ��
*     Bool                   ������ɢ�㷵��false ������ɢ�㷵��true
*
*  ���ܣ�
*     ���õݹ��㷨ͳ��������ĸ�����ͨ����ֵ���ж��Ƿ�Ϊ��ɢ��
*
*  ˵����
*     ֻ�ܶ�2ֵͼ����д���
******************************************************************/
bool DeleteScaterJudge(LPSTR lpDIBBits, WORD lLineBytes, LPBYTE lplab, int lWidth, int lHeight, int x, int y, CPoint lab[], int lianXuShu)
{
	//���������������Ҫ��˵��������ɢ�㣬����
	if (m_lianXuShu >= lianXuShu)
		return TRUE;

	//���ȼ�һ
	m_lianXuShu++;

	//�趨���ʱ�־
	lplab[lWidth * y + x] = true;

	//������ʵ�����
	lab[m_lianXuShu - 1].x = x;
	lab[m_lianXuShu - 1].y = y;

	//���صĻҶ�ֵ
	int gray;

	//ָ�����ص�ָ��
	LPSTR lpSrc;

	//�����ж�
	//���������������Ҫ��˵��������ɢ�㣬����
	if (m_lianXuShu >= lianXuShu)
		return TRUE;

	//�������ݹ�
	else
	{
		//�������������Լ����ϡ����ϡ����¡����°˸�����
		//����Ǻ�ɫ�㣬����ú���������еݹ�

		//���������

		lpSrc = (char*)lpDIBBits + lLineBytes * (y - 1) + x;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y - 1 >= 0 && gray == 0 && lplab[(y - 1)*lWidth + x] == false)

			//���еݹ鴦��		
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x, y - 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//���µ�

		lpSrc = (char*)lpDIBBits + lLineBytes * (y - 1) + x - 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y - 1 >= 0 && x - 1 >= 0 && gray == 0 && lplab[(y - 1)*lWidth + x - 1] == false)

			//���еݹ鴦��		
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x - 1, y - 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//���

		lpSrc = (char*)lpDIBBits + lLineBytes * y + x - 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (x - 1 >= 0 && gray == 0 && lplab[y*lWidth + x - 1] == false)

			//���еݹ鴦��		
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x - 1, y, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//����

		lpSrc = (char*)lpDIBBits + lLineBytes * (y + 1) + x - 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y + 1 <lHeight && x - 1 >= 0 && gray == 0 && lplab[(y + 1)*lWidth + x - 1] == false)

			//���еݹ鴦��

			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x - 1, y + 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//����

		lpSrc = (char*)lpDIBBits + lLineBytes * (y + 1) + x;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y + 1 < lHeight && gray == 0 && lplab[(y + 1)*lWidth + x] == false)

			//���еݹ鴦��

			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x, y + 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//����

		lpSrc = (char*)lpDIBBits + lLineBytes * (y + 1) + x + 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y + 1 <lHeight && x + 1 <lWidth &&  gray == 0 && lplab[(y + 1)*lWidth + x + 1] == false)

			//���еݹ鴦��
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x + 1, y + 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//�ұ�

		lpSrc = (char*)lpDIBBits + lLineBytes * y + x + 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (x + 1 <lWidth && gray == 0 && lplab[y*lWidth + x + 1] == false)

			//���еݹ鴦��		
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x + 1, y, lab, lianXuShu);

		//�жϳ���

		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;

		//����

		lpSrc = (char*)lpDIBBits + lLineBytes * (y - 1) + x + 1;

		//���ݻҶ�ֵ
		gray = *lpSrc;

		//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
		if (y - 1 >= 0 && x + 1 <lWidth && gray == 0 && lplab[(y - 1)*lWidth + x + 1] == false)

			//���еݹ鴦��		
			DeleteScaterJudge(lpDIBBits, lLineBytes, lplab, lWidth, lHeight, x + 1, y - 1, lab, lianXuShu);

		//�жϳ���
		//���������������Ҫ��˵��������ɢ�㣬����
		if (m_lianXuShu >= lianXuShu)
			return TRUE;
	}


	//����ݹ����������false��˵������ɢ��
	return FALSE;

}

/************************************************************
*
*  �������ƣ�
*       RemoveScatterNoise()
*
*  ������
*     HDIB    hDIB     ��ԭͼ��ľ��
*
*  ����ֵ:
*       ��
*
*  ���ܣ�
*     ͨ���������㳤�ȵ�ͳ����ȥ����ɢ�ӵ�
*
*  ˵����
*      ֻ�ܶ�2ֵͼ����д���
****************************************************************/
void RemoveScatterNoise(HDIB hDIB)
{

	// ָ��DIB��ָ��
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;

	// �ҵ�DIBͼ����������������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	//���ͼ��ĳ���
	LONG lWidth = ::DIBWidth((char*)lpDIB);

	//���ͼ��ĸ߶�
	LONG lHeight = ::DIBHeight((char*)lpDIB);

	//�����ж������ĳ�����ֵΪ15
	//������뿼��������ӵĺڵ����ĿС��15����Ϊ�������������
	int length = 15;

	// ѭ������
	m_lianXuShu = 0;
	LONG	i;
	LONG	j;
	LONG    k;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);


	LPSTR lpSrc;

	//����һ��������ű�־���ڴ�����
	LPBYTE lplab = new BYTE[lHeight * lWidth];

	//����һ������������ɢ�ж�������ڴ�����
	bool *lpTemp = new bool[lHeight * lWidth];

	//��ʼ����־����
	for (i = 0; i<lHeight*lWidth; i++)
	{

		//�����еı�־λ����Ϊ��
		lplab[i] = false;

	}

	//���������ɢ������������
	CPoint lab[21];

	//Ϊѭ����������ʼֵ
	k = 0;

	//ɨ������ͼ��

	//����ɨ��
	for (i = 0; i<lHeight; i++)
	{

		//����ɨ��
		for (j = 0; j<lWidth; j++)
		{
			//�Ȱѱ�־λ��false
			for (k = 0; k<m_lianXuShu; k++)
				lplab[lab[k].y * lWidth + lab[k].x] = false;

			//��������0
			m_lianXuShu = 0;

			//������ɢ���ж�
			lpTemp[i*lWidth + j] = DeleteScaterJudge(lpDIBBits, (WORD)lLineBytes, lplab, lWidth, lHeight, j, i, lab, length);

		}
	}

	//ɨ������ͼ�񣬰���ɢ�����ɰ�ɫ

	//����ɨ��
	for (i = 0; i<lHeight; i++)
	{

		//����ɨ��
		for (j = 0; j<lWidth; j++)
		{
			//�鿴��־λ,���Ϊ���򽫴˵���Ϊ�׵�
			if (lpTemp[i*lWidth + j] == false)
			{
				//ָ���i�е�j�����ص�ָ��
				lpSrc = (char*)lpDIBBits + lLineBytes * i + j;

				//����������Ϊ�׵�
				*lpSrc = BYTE(255);
			}
		}
	}

	//�������
	::GlobalUnlock((HGLOBAL)hDIB);

}

/*********************************************************

* �������ƣ�
*         SlopeAdjust()
*
* ������
*     HDIB   hDIB       ��ԭͼ��ľ��
*
* ����ֵ��
*         ��
*
* ���ܣ�
*     ͨ����ͼ�����Ұ��ƽ���߶ȵ�ͳ����������б�ĵ���
*
* ˵����
*      ֻ�ܶ�2ֵͼ����д���
*
****************************************************************/
void SlopeAdjust(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;

	// ѭ������
	LONG	i;
	LONG	j;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	//ͼ��ĳ���
	LONG    lWidth;

	//ͼ��Ŀ��
	LONG	lHeight;

	//��ȡͼ��ĳ���
	lWidth = ::DIBWidth((char*)lpDIB);

	//��ȡͼ��Ŀ��
	lHeight = ::DIBHeight((char*)lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//ͼ�����ߵ�ƽ���߶�
	double leftaver = 0.0;

	//ͼ���Ұ�ߵ�ƽ���߶�
	double rightaver = 0.0;

	//ͼ�����б��
	double slope;

	//ͳ��ѭ������
	LONG counts = 0;

	//ɨ�����ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i = 0; i<lHeight; i++)
	{

		//��
		for (j = 0; j<lWidth / 2; j++)
		{

			//ָ���i�е�j�����ص�ָ��	
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes *  i + j;

			//���Ϊ�ڵ�
			if (*lpSrc == 0)
			{

				//����߶Ƚ���ͳ�Ƶ���
				counts += lWidth / 2 - j;
				leftaver += i*(lWidth / 2 - j);

			}

		}
	}

	//����ƽ���߶�
	leftaver /= counts;

	//��ͳ��ѭ���������¸�ֵ
	counts = 0;

	//ɨ���Ұ�ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i = 0; i<lHeight; i++)
	{

		//��
		for (j = lWidth / 2; j<lWidth; j++)
		{
			//ָ���i�е�j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes *  i + j;

			//���Ϊ�ڵ�
			if (*lpSrc == 0)
			{

				//����ͳ�Ƶ���
				counts += lWidth - j;
				rightaver += i*(lWidth - j);
			}
		}
	}

	//�����Ұ�ߵ�ƽ���߶�
	rightaver /= counts;

	//����б��
	slope = (leftaver - rightaver) / (lWidth / 2);

	//ָ���µ�ͼ��������ʼλ�õ�ָ��
	LPSTR lpNewDIBBits;

	//ָ����ͼ���ָ��
	LPSTR lpDst;

	//��ͼ��ľ��
	HLOCAL nNewDIBBits = LocalAlloc(LHND, lLineBytes*lHeight);

	//�����ڴ�
	lpNewDIBBits = (char*)LocalLock(nNewDIBBits);

	//ָ����ͼ�����ص�ָ��
	lpDst = (char*)lpNewDIBBits;

	//Ϊ��ͼ�񸳳�ʼֵ
	memset(lpDst, (BYTE)255, lLineBytes*lHeight);

	//���ص�ĻҶ�ֵ
	int gray;

	//λ��ӳ��ֵ
	int i_src;

	//����б�ʣ��ѵ�ǰ��ͼ��ĵ�ӳ�䵽Դͼ��ĵ�

	//��
	for (i = 0; i<lHeight; i++)
	{
		//��
		for (j = 0; j<lWidth; j++)
		{
			//����ӳ��λ��	
			i_src = int(i - (j - lWidth / 2)*slope);

			//�������ͼ���⣬�����ð�ɫ
			if (i_src <0 || i_src >= lHeight)
				gray = 255;

			else
			{
				//����Դͼ�����ҵ㣬ȡ������ֵ

				//ָ���i_src�е�j�����ص�ָ��
				lpSrc = (unsigned char *)lpDIBBits + lLineBytes *  i_src + j;
				gray = *lpSrc;
			}

			//����ͼ��ĵ��õõ�������ֵ���
			//ָ���i�е�j�����ص�ָ��
			lpDst = (char *)lpNewDIBBits + lLineBytes * i + j;
			*lpDst = gray;
		}
	}

	// ���µ�ͼ������ݿ������ɵ�ͼ����
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes*lHeight);

	// �������
	::GlobalUnlock((HGLOBAL)hDIB);
}

/*************************************************
*
* �������ƣ�
*       CharSegment()
*
*  ������
*      HDIB  hDIB      ��ԭͼ��ľ��
*
*  ����ֵ��
*     CRectLink        ����ű��ָ�ĸ����ַ�λ����Ϣ������
*
* ���ܣ�
*    ��ͼ���д�ʶ����ַ����������������ش�Ÿ����ַ���λ����Ϣ������
*
*  ˵����
*    �˺���ֻ�ܶ�2ֵ�����ͼ����д���
*
*********************************************************/
CRectLink CharSegment(HANDLE hDIB)
{

	//�����������ÿ���ַ����������
	CRectLink charRect1, charRect2;
	charRect1.clear();
	charRect2.clear();

	// ָ��DIB��ָ��
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	//ָ�����صĵ�ָ��
	BYTE* lpSrc;

	//ͼ��ĳ��ȺͿ��
	int height, width;

	//��ȡͼ��Ŀ��
	width = (int)::DIBWidth(lpDIB);

	//��ȡͼ��ĳ���
	height = (int)::DIBHeight(lpDIB);

	//����ͼ��ÿ�е��ֽ���
	LONG	lLineBytes = WIDTHBYTES(width * 8);

	//�������±߽���������
	int top, bottom;

	//���صĻҶ�ֵ
	int gray;

	//����ѭ������
	int i, j;

	//����ͳ��ͼ�����ַ������ļ�����
	digicount = 0;


	//��������ɨ�裬�ҵ��ϱ߽�

	//��
	for (i = 0; i<height; i++)
	{
		//��
		for (j = 0; j<width; j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//��øõ�ĻҶ�ֵ
			gray = *(lpSrc);

			//���Ƿ�Ϊ�ڵ�
			if (gray == 0)
			{
				//��Ϊ�ڵ㣬�Ѵ˵���Ϊ�ַ����µ���ߵ�
				top = i;

				//��iǿ�и�ֵ���ж�ѭ��
				i = height;

				//����ѭ��
				break;
			}

			//����õ㲻�Ǻڵ㣬����ѭ��
		}
	}


	//��������ɨ�裬���±߽�

	//��
	for (i = height - 1; i >= 0; i--)
	{

		//��
		for (j = 0; j<width; j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//��ȡ�õ�ĻҶ�ֵ
			gray = *(lpSrc);

			//�ж��Ƿ�Ϊ�ڵ�
			if (gray == 0)
			{
				//��Ϊ�ڵ㣬�Ѵ˵���Ϊ�ַ����µ���͵�
				bottom = i;

				//��iǿ�и�ֵ���ж�ѭ��
				i = -1;

				//����ѭ��
				break;
			}

			//����õ㲻�Ǻڵ㣬����ѭ��
		}

	}

	//lab �����Ƿ����һ���ַ��ָ�ı�־
	bool lab = false;

	//����ɨ��һ�����Ƿ��ֺ�ɫ��
	bool black = false;

	//���λ����Ϣ�Ľṹ��
	CRect rect;

	//����������
	digicount = 0;

	//��
	for (i = 0; i<width; i++)
	{
		//��ʼɨ��һ��
		black = false;

		for (j = 0; j<height; j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;

			//��ȡ�õ�ĻҶ�ֵ
			gray = *(lpSrc);

			//�ж��Ƿ�Ϊ�ڵ�
			if (gray == 0)
			{
				//������ֺڵ㣬���ñ�־λ
				black = true;

				//�����û�н���һ���ַ��ķָ�
				if (lab == false)
				{
					//�������߽�
					rect.left = i;

					//�ַ��ָʼ
					lab = true;
				}

				//����ַ��ָ��Ѿ���ʼ��
				else

					//����ѭ��
					break;
			}
		}

		//����Ѿ�ɨ�������ұ����У�˵������ͼ��ɨ����ϡ��˳�
		if (i == (width - 1))

			//�˳�����ѭ��	   
			break;

		//�������black��Ϊfalse��˵��ɨ����һ�У���û�з��ֺڵ㡣������ǰ�ַ��ָ����
		if (lab == true && black == false)
		{
			//��λ����Ϣ����ṹ����

			//�����ұ߽�
			rect.right = i;

			//�����ϱ߽�
			rect.top = top;

			//�����±߽�
			rect.bottom = bottom;

			//��������һ�����أ�����ѹ���ַ�
			rect.InflateRect(1, 1);

			//������ṹ�������λ����Ϣ������1�ĺ���
			charRect1.push_back(rect);

			//���ñ�־λ����ʼ��һ�ε��ַ��ָ�
			lab = false;

			//�ַ�����ͳ�Ƽ�������1
			digicount++;

		}

		//������һ�е�ɨ��

	}

	//�ٽ������������ε�top��bottom��ȷ��

	//������1��ֵ������2
	charRect2 = charRect1;

	//������2���������
	charRect2.clear();

	//����һ���µĴ��λ����Ϣ�Ľṹ��
	CRect rectnew;

	//��������1��ͷ��β�������ɨ��
	while (!charRect1.empty())
	{
		//������1ͷ�ϵõ�һ������
		rect = charRect1.front();

		//������1ͷ����ɾ��һ��
		charRect1.pop_front();

		//������Ӿ�ȷ�ľ�������

		//��þ�ȷ����߽�
		rectnew.left = rect.left - 1;

		//��þ�ȷ���ұ߽�
		rectnew.right = rect.right + 1;

		//ͨ����õľ�ȷ���ұ߽�����±߾����½��о�ȷ��λ

		// ���¶���ɨ������ϱ߽�

		//��
		for (i = rect.top; i<rect.bottom; i++)
		{
			//��
			for (j = rect.left; j<rect.right; j++)
			{
				// ָ��ͼ���i�У���j�����ص�ָ��
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//�����������Ǻڵ�
				if (*lpSrc == 0)
				{
					//�����ϱ߽�
					rectnew.top = i - 1;

					//��i����ǿ�ƶ���������ѭ��
					i = rect.bottom;

					//����ѭ��
					break;
				}
			}
		}

		//���¶���ɨ������±߽�

		//��
		for (i = rect.bottom - 1; i >= rect.top; i--)
		{
			//��
			for (j = rect.left; j<rect.right; j++)
			{
				// ָ��ͼ���i�У���j�����ص�ָ��
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//�õ����Ϊ�ڵ�
				if (*lpSrc == 0)
				{
					//�����±߽�
					rectnew.bottom = i + 1;

					//��i����ǿ�ƶ���������ѭ��
					i = -1;
					//����ѭ��
					break;
				}
			}
		}

		//���õ����µ�׼ȷ��λ����Ϣ�Ӻ���嵽����2��β��
		charRect2.push_back(rectnew);
	}

	//������2 ���ݸ�����1
	charRect1 = charRect2;

	//�������
	::GlobalUnlock(hDIB);

	//������1����
	return charRect1;
}

/******************************************************************
*
*  �������ƣ�
*      StdDIBbyRect()
*
*  ������
*     HDIB  hDIB          ��ͼ��ľ��
*     int   tarWidth      ����׼���Ŀ��
*     int   tarHeight     ����׼���ĸ߶�
*
*  ����ֵ��
*         ��
*
*  ���ܣ�
*     �������ָ���ַ����������Ŵ���ʹ���ǵĿ�͸�һֱ���Է�����������ȡ
*
*  ˵����
*     �������õ��ˣ�ÿ���ַ���λ����Ϣ�����Ա�����ִ����ָ����֮�����ִ�б�׼������
*
******************************************************************/
void StdDIBbyRect(HDIB hDIB, int tarWidth, int tarHeight)
{

	//ָ��ͼ���ָ��
	BYTE* lpDIB = (BYTE*)::GlobalLock((HGLOBAL)hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE* lpDIBBits = (BYTE*)::FindDIBBits((char*)lpDIB);

	//ָ�����ص�ָ��
	BYTE* lpSrc;

	//��ȡͼ��ĵĿ��
	LONG lWidth = ::DIBWidth((char*)lpDIB);

	//��ȡͼ��ĸ߶�
	LONG lHeight = ::DIBHeight((char*)lpDIB);

	// ѭ������
	int	i;
	int	j;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes = WIDTHBYTES(lWidth * 8);

	//��ȡ��߶ȷ����ϵ���������
	double wscale, hscale;

	//����һ����ʱ������,����ű仯���ͼ����Ϣ
	LPSTR lpNewDIBBits;
	LPSTR lpDst;

	//�������Ĵ�С��ԭͼ�����������Сһ��
	HLOCAL nNewDIBBits = LocalAlloc(LHND, lLineBytes*lHeight);

	//ָ�򻺴�����ʼλ�õ�ָ��
	lpNewDIBBits = (char*)LocalLock(nNewDIBBits);

	//ָ�򻺴�����Ϣ��ָ��
	lpDst = (char*)lpNewDIBBits;

	//�������������ݸ���ʼֵ
	memset(lpDst, (BYTE)255, lLineBytes*lHeight);

	//����ӳ��������������
	int i_src, j_src;

	//����ַ�λ����Ϣ�Ľṹ��
	CRect rect;
	CRect rectnew;

	//�����һ���µľ������������Ա�洢��׼����ľ�����������
	m_charRectCopy.clear();

	//��ͷ��β���ɨ��������
	while (!m_charRect.empty())
	{
		//�ӱ�ͷ�ϵõ�һ������
		rect = m_charRect.front();

		//������ͷ����ɾ��һ��
		m_charRect.pop_front();

		//������������

		//�����귽�����������
		wscale = (double)tarWidth / rect.Width();

		//�����귽�����������
		hscale = (double)tarHeight / rect.Height();

		//�����׼������

		//�ϱ߽�
		rectnew.top = rect.top;

		//�±߽�
		rectnew.bottom = rect.top + tarHeight;

		//��߽�
		rectnew.left = rect.left;

		//�ұ߽�
		rectnew.right = rectnew.left + tarWidth;

		//��ԭ���ο��ڵ�����ӳ�䵽�µľ��ο���
		for (i = rectnew.top; i<rectnew.bottom; i++)
		{
			for (j = rectnew.left; j<rectnew.right; j++)
			{

				//����ӳ������
				i_src = rectnew.top + int((i - rectnew.top) / hscale);
				j_src = rectnew.left + int((j - rectnew.left) / wscale);

				//�����Ӧ�����ص����ӳ�����
				lpSrc = (unsigned char *)lpDIBBits + lLineBytes *  i_src + j_src;
				lpDst = (char *)lpNewDIBBits + lLineBytes * i + j;
				*lpDst = *lpSrc;
			}
		}
		//����׼����ľ�����������µ�����
		m_charRectCopy.push_back(rectnew);


	}

	//�洢��׼�����µ�rect����
	m_charRect = m_charRectCopy;

	//�������������ݿ�����ͼ�����������
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes*lHeight);

	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
}

/*******************************************
*
*  �������ƣ�
*  AutoAlign()
*
*  ������
*    HDIB   hDIB        ��ԭͼ��ľ��
*
*  ����ֵ
*    HDIB               ���������к����ͼ��ľ��
*
*  ���ܣ�
*     �������˱�׼��������ַ����й��������У��Է�����һ���Ĵ���
*
*  ˵����
*     �������еĲ��������ڱ�׼������֮�����
*
********************************************************/


HDIB AutoAlign(HDIB hDIB)
{

	//ָ��ͼ���ָ��
	BYTE* lpDIB = (BYTE*)::GlobalLock((HGLOBAL)hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE* lpDIBBits = (BYTE*)::FindDIBBits((char*)lpDIB);

	//ָ�����ص�ָ��
	BYTE* lpSrc;

	//��ȡͼ��Ŀ��
	LONG lWidth = ::DIBWidth((char*)lpDIB);

	//��ȡͼ��ĸ߶�
	LONG lHeight = ::DIBHeight((char*)lpDIB);

	//��ȡ��׼���Ŀ��
	int w = m_charRect.front().Width();

	//��ȡ��׼���ĸ߶�
	int h = m_charRect.front().Height();

	//����һ���µ�ͼ�������ܹ�����׼�����ַ����ŷ���
	HDIB hNewDIB = ::NewDIB(digicount*w, h, 8);

	//ָ���µ�ͼ���ָ��
	BYTE* lpNewDIB = (BYTE*) ::GlobalLock((HGLOBAL)hNewDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE* lpNewDIBBits = (BYTE*)::FindDIBBits((char*)lpNewDIB);

	//ָ�����ص�ָ��
	BYTE* lpDst = lpNewDIBBits;

	//����ԭͼ��ÿ�е��ֽ���
	LONG lLineBytes = (lWidth + 3) / 4 * 4;

	//������ͼ��ÿ�е��ֽ���
	LONG lLineBytesnew = (digicount*w + 3) / 4 * 4;

	//���µ�ͼ���ʼ��Ϊ��ɫ
	memset(lpDst, (BYTE)255, lLineBytesnew * h);

	//ӳ��������������
	int i_src, j_src;

	//ѭ������
	int i, j;

	//ͳ���ַ������ı���
	int counts = 0;

	//���λ����Ϣ�Ľṹ��
	CRect rect, rectnew;

	//���һ���µ�����������µ��ַ�λ����Ϣ
	m_charRectCopy.clear();

	//��ͷ��β���ɨ��ԭ����ĸ������
	while (!m_charRect.empty())
	{
		//�ӱ�ͷ�ϵõ�һ�����ο�
		rect = m_charRect.front();

		//��������ο��������ɾ��
		m_charRect.pop_front();

		//�����µľ��ο��λ����Ϣ

		//��߽�
		rectnew.left = counts*w;

		//�ұ߽�
		rectnew.right = (counts + 1)*w;

		//�ϱ߽�
		rectnew.top = 0;

		//�±߽�
		rectnew.bottom = h;

		//����õ��µľ��ο���뵽�µ�������
		m_charRectCopy.push_back(rectnew);

		//��ԭ���ο��ڵ�����ӳ�䵽�µľ��ο���
		for (i = 0; i<h; i++)
		{
			for (j = counts*w; j<(counts + 1)*w; j++)
			{

				//����ӳ������
				i_src = rect.top + i;
				j_src = rect.left + j - counts*w;

				//�������ص�ӳ��
				lpSrc = (BYTE *)lpDIBBits + lLineBytes *  i_src + j_src;
				lpDst = (BYTE *)lpNewDIBBits + lLineBytesnew * i + j;
				*lpDst = *lpSrc;
			}
		}

		//�ַ�������1
		counts++;

	}

	//����õ��µ������Ƶ�ԭ�����У��Է�����һ�εĵ���
	m_charRect = m_charRectCopy;

	//�������
	::GlobalUnlock(hDIB);


	::GlobalUnlock(hNewDIB);
	return hNewDIB;
}

// �ֱ𱣴���Щ�Ѿ������ָ��׼��ĵ����������ַ���bmp�ļ����Ա��������ʹ��
void ImgprcToDibAndSave(CString strPath)
{
	unsigned char* lpSrc;
	int w, h;
	w = m_charRect.front().Width();
	h = m_charRect.front().Height();
	m_dibRect.clear();
	m_dibRectCopy.clear();
	int i_src, j_src;
	int i, j;
	int counts = 0;
	CRect rect, rectnew;
	BYTE* lpDIB = (BYTE*)::GlobalLock((HGLOBAL)m_hDIB);
	BYTE* lpDIBBits = (BYTE*)::FindDIBBits((char*)lpDIB);
	BYTE* lpNewDIBBits;
	BYTE* lpDst;
	LONG lLineBytes = (digicount*w + 3) / 4 * 4;
	LONG lLineBytesnew = (w + 3) / 4 * 4;
	HDIB hDIB = NULL;
	while (!m_charRect.empty())
	{
		hDIB = ::NewDIB(w, h, 8);
		lpDIB = (BYTE*) ::GlobalLock((HGLOBAL)hDIB);
		lpNewDIBBits = (BYTE*)::FindDIBBits((char*)lpDIB);
		lpDst = (BYTE*)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lLineBytesnew * h);
		rect = m_charRect.front();
		m_charRect.pop_front();
		for (i = 0; i<h; i++)
			for (j = 0; j<w; j++)
			{
				i_src = rect.top + i;
				j_src = j + counts*w;
				lpSrc = (BYTE *)lpDIBBits + lLineBytes *  i_src + j_src;
				lpDst = (BYTE *)lpNewDIBBits + lLineBytesnew * i + j;
				*lpDst = *lpSrc;
			}
		::GlobalUnlock(hDIB);
		m_dibRect.push_back(hDIB);
		counts++;
	}
	m_charRect = m_charRectCopy;
	m_dibRectCopy = m_dibRect;
	//���Ϊ.bmp�ļ�
	CString str;
	counts = 1;
	while (!m_dibRect.empty())
	{
		//str.Format("part%d.bmp", counts);
		str.Format(_T("part%d.bmp"), counts);

		// Note:strPath ����ļ����·�� 
		//str=strPath+"\\"+str;
		str = strPath + _T("\\") + str;

		CFile file(str, CFile::modeReadWrite | CFile::modeCreate);
		hDIB = m_dibRect.front();
		::SaveDIB(hDIB, file);
		m_dibRect.pop_front();
		file.Close();
		counts++;
	}
	m_dibRect = m_dibRectCopy;
}

// һ����Ԥ����
void ImgprcAll(HDIB hDIB,CString strPath)
{
	m_hDIB = hDIB;
	// 1.��256ɫͼת��Ϊ�Ҷ�ͼ
	Convert256toGray(m_hDIB);
	// 2.���Ҷ�ͼ��ֵ��
	ConvertGrayToWhiteBlack(m_hDIB);
	// 3.�ݶ���
	// 4.ȥ����ɢ�ӵ�����
	RemoveScatterNoise(m_hDIB);
	// 5.���������ַ���������б
	//SlopeAdjust(m_hDIB);  ��һ���㷨��Щ����
	// 6.�ָ�����ʶ
	m_charRect = CharSegment(m_hDIB);
	// 7.���ָ��������ַ����߱�׼�����Ա�����һ����BP������������
	//StdDIBbyRect(m_hDIB, 64, 64);
	//StdDIBbyRect(m_hDIB, 8, 16);
	StdDIBbyRect(m_hDIB, 28, 28);  // ����ŦԼ��ѧѵ����
	ConvertGrayToWhiteBlack(m_hDIB);
	// 8.�������������ַ�,Note:Chenqp ��һ�������٣�����ͼƬ�ϵ����ֳ�����;
	m_hDIB = AutoAlign(m_hDIB);
	// 9.�ֱ𱣴���Щ�Ѿ������ָ��׼��ĵ����������ַ���bmp�ļ����Ա��������ʹ��
	ImgprcToDibAndSave(strPath);

}




