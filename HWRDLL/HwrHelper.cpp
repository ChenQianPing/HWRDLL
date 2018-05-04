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

	//// 1.��ȡѵ���� ����classify.pattern
	//// char* ת���� LPCTSTR
	//int num_first = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	//wchar_t *wide_frist = new wchar_t[num_first];
	//MultiByteToWideChar(0, 0, templateFilePath, -1, wide_frist, num_first);

	//CFile TheFile_first(wide_frist, CFile::modeRead);

	////CFile TheFile(LPCWSTR("E:\\template.dat"), CFile::modeRead);
	//CArchive ar_first(&TheFile_first, CArchive::load, 40960);
	//TheFile_first.SeekToBegin();


	//for (int i = 0; i<10; i++)
	//{
	//	// �������ݵ�������
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
* �������ƣ�
*      GetPictureWord()  

* ����: ChenQP 2016-04-15
*
* ������
*     char * imageFilePath     -- ��ͼƬ���ֵ��ļ�·��
*     char * outputFilePath    -- ����ļ�·��
*     
*  ����ֵ��
*        ��������ļ�·�������ɷָ�õ�ͼƬ�ļ�
*
*  ���ܣ�
*     ͼƬ���ַָ���㷨

*  ����˵��:
*   const char*  -> C#  string  ע�⣺C#���Ͷ���
*
***************************************************************/
extern "C" __declspec(dllexport) int GetPictureWord(const char* imageFilePath, const char* outputFilePath)
{
	CString strPathName;  // �������ļ�·��
	HDIB m_hDIB;

	//strPathName = "D:\\00\\t-3.bmp";
	strPathName = imageFilePath;
	// ����һ���ļ�����
	CFile file;
	// ��ֻ��ģʽ���ļ�
	file.Open(strPathName, CFile::modeRead);

	// ��ȡ�ļ���HDIB�����. ע��:��ʱֻ�Ƕ�ȡλͼ�ļ����ļ�ͷ֮��Ĳ���,�����ļ�ͷ
	m_hDIB = ::ReadDIBFile(file);

	// HDIB���: ����һ��洢λͼ���ݵ��ڴ�����ĵ�ַ
	// HDIB�������:λͼ��Ϣͷ����ɫ��(����еĻ�)��DIBͼ������
	// �ر��ļ�
	file.Close();

	// ָ��DIB��ָ��(ָ��λͼ��Ϣͷ)
	BYTE* lpDIB = (BYTE*)::GlobalLock((HGLOBAL)m_hDIB);
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	WORD wNumColors;
	wNumColors = ::DIBNumColors((char*)lpDIB);

	// �ж��Ƿ���256ɫλͼ
	if (wNumColors != 256)
	{
		// ��ʾ�û�
		MessageBox(NULL, (LPCWSTR)L"��256ɫλͼ��", (LPCWSTR)L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL)m_hDIB);
		// ����
		return 0;
	}

	// char * -->CString
	// outputFilePath = "G:\\testDLL"
	CString str1(outputFilePath);

	ImgprcAll(m_hDIB, str1);

	//MessageBox(NULL, (LPCWSTR)L"������ɣ��뵽Ŀ¼�²鿴ͼƬ��", (LPCWSTR)L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

	return 0;
}

/****************************************************************
* �������ƣ�
*      DigitRecognition()

* ����: ChenQP 2016-04-15
*
* ������
*     char * imageFilePath     -- ����ʶ���ļ�·��
*     char * templateFilePath  -- ѵ�����ļ�·��

*
*  ����ֵ��
*        ��
*
*  ���ܣ�
*     ���ڽ�ģ��ƥ�䷨�㷨
* ����˵����
*  const char*  -> C#  string  ע�⣺C#���Ͷ���
*
***************************************************************/
extern "C" __declspec(dllexport) int DigitRecognition(const char* imageFilePath,const char* templateFilePath)
{

	Classification classify;
	//CString filename;

	//return 1;

	//templateFilePath = "G:\\testDLL\\template.dat";
	//CFile TheFile(_T("E:\\template.dat"), CFile::modeRead);

	// char* ת���� LPCTSTR
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
		// �������ݵ�������
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

	// ���ڽ�ģ��ƥ�䷨
	result = classify.GetNumberByLeastDistance();

	//str.Format(_T("Ӧ����С���뷨��\n�Զ�����ʶ����Ϊ��%d"), result);
	//AfxMessageBox(str, MB_OK, NULL);

	return result;
}

// �����յ�ѵ���⣬���ڱ�������
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

// ������Ʒ��ѵ���� 
extern "C" __declspec(dllexport) int AddTrainingbase(const char* imageFilePath, int m_num, const char* templateFilePath)
{
	Classification classify;

	// 1.��ȡѵ���� ����classify.pattern
	// char* ת���� LPCTSTR
	int num_first = MultiByteToWideChar(0, 0, templateFilePath, -1, NULL, 0);
	wchar_t *wide_frist = new wchar_t[num_first];
	MultiByteToWideChar(0, 0, templateFilePath, -1, wide_frist, num_first);

	CFile TheFile_first(wide_frist, CFile::modeRead);

	//CFile TheFile(LPCWSTR("E:\\template.dat"), CFile::modeRead);
	CArchive ar_first(&TheFile_first, CArchive::load, 40960);
	TheFile_first.SeekToBegin();


	for (int i = 0; i<10; i++)
	{
		// �������ݵ�������
		ar_first >> classify.pattern[i].number;
		for (int n = 0; n<classify.pattern[i].number; n++)
			for (int j = 0; j<25; j++)
			{
				ar_first >> classify.pattern[i].feature[n][j];
			}
	}

	ar_first.Close();
	TheFile_first.Close();

	//// 2.�����ļ�
	classify.LoadFile(imageFilePath);

	// 3.��������������Ҫִ��
	classify.GetPosition();
	classify.SetFeature();

	// 4.���ִ���ѵ����
	if (classify.pattern[m_num].number>=200)
	{
		//MessageBox(NULL,_T("ģ��������ܳ���200����"), NULL, MB_OK);
	}
		
	else
	{
		if (classify.Saveable(m_num))
		{
			classify.Save(m_num);

			// templateFilePath
			// templateFilePath = "G:\\testDLL\\template.dat";
			// char* ת���� LPCTSTR
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
			//MessageBox(NULL,_T("����Ʒ�Ѵ�������Ʒ���У���ı���Ʒ��������"), NULL, MB_OK);
		}
			
	}



	return 0; //0 ��ʾ�ɹ�ִ��

}

