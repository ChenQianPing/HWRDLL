#pragma once

#include "GetFeature.h"

struct number_no
{
	int number;
	int no;
};

class Classification : public GetFeature
{
public:
	Classification();
	~Classification();

	// ����������Ʒ��ƥ��̶� ,����������Ʒ�ĵ�ƥ��̶ȡ�
	double pipei(double s1[], double s2[]);
	// ��С���뷨 ,�����������ͱ�� ���ڽ�ƥ��ģ�巨
	number_no LeastDistance();
	// �������ڽ�ƥ��ģ�巨Result
	int GetNumberByLeastDistance();

};

