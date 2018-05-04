
#include "DIBAPI.h"

#include <deque>
using namespace std;

typedef deque<CRect> CRectLink;
typedef deque<HDIB>  HDIBLink;

// 1.将256色位图转为灰度图
void Convert256toGray(HDIB hDIB);

// 2.将灰度图二值化 
void ConvertGrayToWhiteBlack(HDIB hDIB);

// 4.去除离散噪声点
void RemoveScatterNoise(HDIB hDIB);

// 4.1.判断是否是离散噪声点
bool DeleteScaterJudge(LPSTR lpDIBBits, WORD lLineBytes, LPBYTE lplab, int lWidth, int lHeight, int x, int y, CPoint lab[], int lianXuShu);

// 5.倾斜度调整
void SlopeAdjust(HDIB hDIB);

// 6.对位图进行分割.返回一个存储着每块分割区域的链表
CRectLink CharSegment(HANDLE hDIB);

// 7.将分割后的数字字符宽、高标准化，以便于下一步与BP网络的输入兼容
void StdDIBbyRect(HDIB hDIB, int tarWidth, int tarHeight);

// 8.紧缩、重排调整
HDIB AutoAlign(HDIB hDIB);

// 9.分别保存这些已经经过分割、标准后的单个的数字字符到bmp文件
void ImgprcToDibAndSave(CString outputFilePath);

// 10.一次性预处理
void ImgprcAll(HDIB hDIB,CString outputFilePath);
