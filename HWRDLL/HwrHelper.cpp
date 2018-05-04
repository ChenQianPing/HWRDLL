#include "stdafx.h"
#include "HwrHelper.h"
#include "DIBAPI.h"
#include "MyDibLib.h"

#include "Classification.h"
#include "TestClass.h"


extern "C" __declspec(dllexport) int Add(int a, int b)
{
	TestClass classify;

	//const char* templateFilePath;
	//templateFilePath = "G:\\testDLL\\template.dat";

	//// 1.读取训练库 存入classify.pattern
	//// char* 转换成 LPCTSTR
	//int num_first = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	//wchar_t *wide_frist = new wchar_t[num_first];
	//MultiByteToWideChar(0, 0, templateFilePath, -1, wide_frist, num_first);

	//CFile TheFile_first(wide_frist, CFile::modeRead);

	////CFile TheFile(LPCWSTR("E:\\template.dat"), CFile::modeRead);
	//CArchive ar_first(&TheFile_first, CArchive::load, 40960);
	//TheFile_first.SeekToBegin();


	//for (int i = 0; i<10; i++)
	//{
	//	// 输入数据到变量中
	//	ar_first >> classify.pattern[i].number;
	//	for (int n = 0; n<classify.pattern[i].number; n++)
	//		for (int j = 0; j<25; j++)
	//		{
	//			ar_first >> classify.pattern[i].feature[n][j];
	//		}
	//}

	//ar_first.Close();
	//TheFile_first.Close();

	return a + b;
}


/****************************************************************
* 函数名称：
*      GetPictureWord()  

* 作者: ChenQP 2016-04-15
*
* 参数：
*     char * imageFilePath     -- 待图片文字的文件路径
*     char * outputFilePath    -- 输出文件路径
*     
*  返回值：
*        根据输出文件路径，生成分割好的图片文件
*
*  功能：
*     图片文字分割处理算法

*  其他说明:
*   const char*  -> C#  string  注意：C#类型对照
*
***************************************************************/
extern "C" __declspec(dllexport) int GetPictureWord(const char* imageFilePath, const char* outputFilePath)
{
	CString strPathName;  // 完整的文件路径
	HDIB m_hDIB;

	//strPathName = "D:\\00\\t-3.bmp";
	strPathName = imageFilePath;
	// 创建一个文件对象
	CFile file;
	// 以只读模式打开文件
	file.Open(strPathName, CFile::modeRead);

	// 读取文件到HDIB句柄中. 注意:此时只是读取位图文件中文件头之后的部分,不含文件头
	m_hDIB = ::ReadDIBFile(file);

	// HDIB句柄: 就是一块存储位图数据的内存区域的地址
	// HDIB句柄包含:位图信息头、调色板(如果有的话)、DIB图像数据
	// 关闭文件
	file.Close();

	// 指向DIB的指针(指向位图信息头)
	BYTE* lpDIB = (BYTE*)::GlobalLock((HGLOBAL)m_hDIB);
	// 获取DIB中颜色表中的颜色数目
	WORD wNumColors;
	wNumColors = ::DIBNumColors((char*)lpDIB);

	// 判断是否是256色位图
	if (wNumColors != 256)
	{
		// 提示用户
		MessageBox(NULL, (LPCWSTR)L"非256色位图！", (LPCWSTR)L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)m_hDIB);
		// 返回
		return 0;
	}

	// char * -->CString
	// outputFilePath = "G:\\testDLL"
	CString str1(outputFilePath);

	ImgprcAll(m_hDIB, str1);

	//MessageBox(NULL, (LPCWSTR)L"处理完成，请到目录下查看图片！", (LPCWSTR)L"系统提示", MB_ICONINFORMATION | MB_OK);

	return 0;
}

/****************************************************************
* 函数名称：
*      DigitRecognition()

* 作者: ChenQP 2016-04-15
*
* 参数：
*     char * imageFilePath     -- 单个识别文件路径
*     char * templateFilePath  -- 训练库文件路径

*
*  返回值：
*        无
*
*  功能：
*     最邻近模板匹配法算法
* 其他说明：
*  const char*  -> C#  string  注意：C#类型对照
*
***************************************************************/
extern "C" __declspec(dllexport) int DigitRecognition(const char* imageFilePath,const char* templateFilePath)
{

	Classification classify;
	//CString filename;

	//return 1;

	//templateFilePath = "G:\\testDLL\\template.dat";
	//CFile TheFile(_T("E:\\template.dat"), CFile::modeRead);

	// char* 转换成 LPCTSTR
	int num = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, templateFilePath, -1, wide, num);

	CFile TheFile(wide, CFile::modeRead);

	//CFile TheFile(LPCWSTR("E:\\template.dat"), CFile::modeRead);
	CArchive ar(&TheFile, CArchive::load, 40960);
	TheFile.SeekToBegin();

	//return 2;

	for (int i = 0; i<10; i++)
	{
		// 输入数据到变量中
		ar >> classify.pattern[i].number;
		for (int n = 0; n<classify.pattern[i].number; n++)
			for (int j = 0; j<25; j++)
			{
				ar >> classify.pattern[i].feature[n][j];
			}
	}

	

	ar.Close();
	TheFile.Close();
	//return 3;

	int result;
	CString str;


	//imageFilePath = "G:\\testDLL\\part1.bmp";
	classify.LoadFile(imageFilePath);
	//classify.LoadFile("G:\\testDLL\\part1.bmp");

	//return 7;

	
	classify.GetPosition();
	classify.SetFeature();

	// 最邻近模板匹配法
	result = classify.GetNumberByLeastDistance();

	//str.Format(_T("应用最小距离法，\n自动分类识别结果为：%d"), result);
	//AfxMessageBox(str, MB_OK, NULL);

	return result;
}

// 创建空的训练库，用于保存样本
extern "C" __declspec(dllexport) int CreateTrainingbase(const char* templateFilePath)
{
	int num = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, templateFilePath, -1, wide, num);

	CFile TheFile(wide, CFile::modeCreate | CFile::modeReadWrite);//
	CArchive ar(&TheFile, CArchive::store, 40960);
	TheFile.SeekToBegin();

	ar.Close();
	TheFile.Close();

	return 0;
}

// 保存样品到训练库 
extern "C" __declspec(dllexport) int AddTrainingbase(const char* imageFilePath, int m_num, const char* templateFilePath)
{
	Classification classify;

	// 1.读取训练库 存入classify.pattern
	// char* 转换成 LPCTSTR
	int num_first = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	wchar_t *wide_frist = new wchar_t[num_first];
	MultiByteToWideChar(0, 0, templateFilePath, -1, wide_frist, num_first);

	CFile TheFile_first(wide_frist, CFile::modeRead);

	//CFile TheFile(LPCWSTR("E:\\template.dat"), CFile::modeRead);
	CArchive ar_first(&TheFile_first, CArchive::load, 40960);
	TheFile_first.SeekToBegin();


	for (int i = 0; i<10; i++)
	{
		// 输入数据到变量中
		ar_first >> classify.pattern[i].number;
		for (int n = 0; n<classify.pattern[i].number; n++)
			for (int j = 0; j<25; j++)
			{
				ar_first >> classify.pattern[i].feature[n][j];
			}
	}

	ar_first.Close();
	TheFile_first.Close();

	//// 2.加载文件
	classify.LoadFile(imageFilePath);

	// 3.这两个函数必须要执行
	classify.GetPosition();
	classify.SetFeature();

	// 4.数字存入训练库
	if (classify.pattern[m_num].number>=200)
	{
		//MessageBox(NULL,_T("模板个数不能超过200个！"), NULL, MB_OK);
	}
		
	else
	{
		if (classify.Saveable(m_num))
		{
			classify.Save(m_num);

			// templateFilePath
			// templateFilePath = "G:\\testDLL\\template.dat";
			// char* 转换成 LPCTSTR
			int num = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
			wchar_t *wide = new wchar_t[num];
			MultiByteToWideChar(0, 0, templateFilePath, -1, wide, num);

			CFile TheFile(wide, CFile::modeCreate | CFile::modeReadWrite);//
			CArchive ar(&TheFile, CArchive::store, 40960);
			TheFile.SeekToBegin();
			for (int n = 0; n<10; n++)
			{
				ar << classify.pattern[n].number;
				for (int i = 0; i<classify.pattern[n].number; i++)
					for (int j = 0; j<25; j++)
					{
						ar << classify.pattern[n].feature[i][j];
					}
			}
			ar.Close();
			TheFile.Close();
		}
		else
		{
			//MessageBox(NULL,_T("此样品已存在于样品库中，请改变样品的特征。"), NULL, MB_OK);
		}
			
	}



	return 0; //0 表示成功执行

}

