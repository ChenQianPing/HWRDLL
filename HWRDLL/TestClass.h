
struct new_pattern            // pattern�ṹ�壬����ĳ���������(0��9)��������Ʒ����
{
	int number;               // ����д������Ʒ����
	double feature[200][25];  // ����Ʒ������ÿ����д���������200����Ʒ��ÿ����Ʒ��25������
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



	new_pattern pattern[10];     // ��д������Ʒ������

	double testsample[25];   // �������д����
	int width;               // ��д���ֵĿ�
	int height;              // ��д���ֵĸ�
	int LineBytes;

	//void Save(int cls);       // ����д�����ֱ��浽cls(0��9)�����
	//BOOL Saveable(int cls);   // �ж���д�������ܷ񱣴浽cls(0��9)����У���Ϊ���������Ʒ���������ظ�
	//double Cal(int row, int col); // ����ָ�õ�5��5С�����У���������ռ�ı���
	//void SetFeature();            // ������д���ֵ���������ֵ��testsample
	//void GetPosition();           // �����д���ֵ�λ��

	/*====================Classification==========================*/

	//// ����������Ʒ��ƥ��̶� ,����������Ʒ�ĵ�ƥ��̶ȡ�
	//double pipei(double s1[], double s2[]);
	//// ��С���뷨 ,�����������ͱ�� ���ڽ�ƥ��ģ�巨
	//new_number_no LeastDistance();
	//// �������ڽ�ƥ��ģ�巨Result
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
	int bottom;   // ��д���ֵĵײ�
	int top;      // ��д���ֵĶ���
	int left;     // ��д���ֵ����
	int right;    // ��д���ֵ��ұ�

};


