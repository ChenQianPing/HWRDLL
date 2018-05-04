
struct new_pattern            // pattern结构体，保存某个数字类别(0～9)的所有样品特征
{
	int number;               // 该手写数字样品个数
	double feature[200][25];  // 各样品特征，每类手写数字最多有200个样品，每个样品有25个特征
};

struct new_number_no
{
	int number;
	int no;
};

class TestClass : public CObject
{

public:
	RGBQUAD* m_pRGB;
	BYTE* m_pData;
	UINT m_numberOfColors;
	BOOL m_valid;
	BITMAPFILEHEADER bitmapFileHeader;

	BITMAPINFOHEADER* m_pBitmapInfoHeader;
	BITMAPINFO* m_pBitmapInfo;
	BYTE* pDib;
	DWORD size;

	/*====================GetFeature==========================*/



	new_pattern pattern[10];     // 手写数字样品特征库

	double testsample[25];   // 待测的手写数字
	int width;               // 手写数字的宽
	int height;              // 手写数字的高
	int LineBytes;

	//void Save(int cls);       // 将手写的数字保存到cls(0～9)类别中
	//BOOL Saveable(int cls);   // 判断手写的数字能否保存到cls(0～9)类别中，因为各类别中样品特征不能重复
	//double Cal(int row, int col); // 计算分割好的5×5小区域中，黑像素所占的比例
	//void SetFeature();            // 计算手写数字的特征，赋值给testsample
	//void GetPosition();           // 获得手写数字的位置

	/*====================Classification==========================*/

	//// 计算两个样品的匹配程度 ,返回两各样品的的匹配程度。
	//double pipei(double s1[], double s2[]);
	//// 最小距离法 ,返回数字类别和编号 最邻近匹配模板法
	//new_number_no LeastDistance();
	//// 返回最邻近匹配模板法Result
	//int GetNumberByLeastDistance();



public:
	TestClass();
	~TestClass();

	char m_fileName[256];
	char* GetFileName();
	BOOL IsValid();
	DWORD GetSize();
	UINT GetWidth();
	UINT GetHeight();
	UINT GetNumberOfColors();
	RGBQUAD* GetRGB();
	BYTE* GetData();
	BITMAPINFO* GetInfo();

	WORD PaletteSize(LPBYTE lpDIB);
	WORD DIBNumColors(LPBYTE lpDIB);
	void SaveFile(const CString filename);

public:
	void LoadFile(const char* dibFileName);
	void SetHeight(UINT height);
	void SetWidth(UINT width);
	char* CStringToCharArray(CString str); // Note:

/*====================GetFeature==========================*/
protected:
	int bottom;   // 手写数字的底部
	int top;      // 手写数字的顶部
	int left;     // 手写数字的左边
	int right;    // 手写数字的右边

};


