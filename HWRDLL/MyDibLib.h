
#include "DIBAPI.h"

#include <deque>
using namespace std;

typedef deque<CRect> CRectLink;
typedef deque<HDIB>  HDIBLink;

// 1.��256ɫλͼתΪ�Ҷ�ͼ
void Convert256toGray(HDIB hDIB);

// 2.���Ҷ�ͼ��ֵ�� 
void ConvertGrayToWhiteBlack(HDIB hDIB);

// 4.ȥ����ɢ������
void RemoveScatterNoise(HDIB hDIB);

// 4.1.�ж��Ƿ�����ɢ������
bool DeleteScaterJudge(LPSTR lpDIBBits, WORD lLineBytes, LPBYTE lplab, int lWidth, int lHeight, int x, int y, CPoint lab[], int lianXuShu);

// 5.��б�ȵ���
void SlopeAdjust(HDIB hDIB);

// 6.��λͼ���зָ�.����һ���洢��ÿ��ָ����������
CRectLink CharSegment(HANDLE hDIB);

// 7.���ָ��������ַ����߱�׼�����Ա�����һ����BP������������
void StdDIBbyRect(HDIB hDIB, int tarWidth, int tarHeight);

// 8.���������ŵ���
HDIB AutoAlign(HDIB hDIB);

// 9.�ֱ𱣴���Щ�Ѿ������ָ��׼��ĵ����������ַ���bmp�ļ�
void ImgprcToDibAndSave(CString outputFilePath);

// 10.һ����Ԥ����
void ImgprcAll(HDIB hDIB,CString outputFilePath);
