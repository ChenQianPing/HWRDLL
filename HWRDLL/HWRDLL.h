// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HWRDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HWRDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef HWRDLL_EXPORTS
#define HWRDLL_API __declspec(dllexport)
#else
#define HWRDLL_API __declspec(dllimport)
#endif

// �����Ǵ� HWRDLL.dll ������
class HWRDLL_API CHWRDLL {
public:
	CHWRDLL(void);
	// TODO:  �ڴ�������ķ�����
};

extern HWRDLL_API int nHWRDLL;

HWRDLL_API int fnHWRDLL(void);
