#include "stdafx.h"
#include "Classification.h"


Classification::Classification()
{
}


Classification::~Classification()
{
}

/******************************************************************
*   �������ƣ�LeastDistance()
*   �������ͣ�number_no���ṹ��
*   �������ܣ���С���뷨 ,�����������ͱ��
******************************************************************/
number_no Classification::LeastDistance()
{

	double min = 10000000000;
	number_no number_no;
	for (int n = 0; n<10; n++)
	{
		for (int i = 0; i<pattern[n].number; i++)
		{
			if (pipei(pattern[n].feature[i], testsample)<min)
			{
				// ƥ�����Сֵ
				min = pipei(pattern[n].feature[i], testsample);
				number_no.number = n;   // ��Ʒ���
				number_no.no = i;       // ��Ʒ���
			}
		}
	}
	return number_no;// ������д���ֵ��������
}

int Classification::GetNumberByLeastDistance()
{
	double min = 10000000000;
	number_no number_no;
	for (int n = 0; n<10; n++)
	{
		for (int i = 0; i<pattern[n].number; i++)
		{
			if (pipei(pattern[n].feature[i], testsample)<min)
			{
				//ƥ�����Сֵ
				min = pipei(pattern[n].feature[i], testsample);
				number_no.number = n;//��Ʒ���
				number_no.no = i;//��Ʒ���
			}
		}
	}

	return number_no.number;//������д���ֵ��������
}


/****************************************************************
*   �������ƣ�pipei(double s1[], double s2[])
*   �������ͣ�double
*   ����˵����double s1[], double s2[]:������Ʒ������
*   �������ܣ�����������Ʒ��ƥ��̶� ,����������Ʒ�ĵ�ƥ��̶ȡ�
****************************************************************/
double Classification::pipei(double s1[], double s2[])
{
	double count = 0.0;
	for (int i = 0; i<25; i++)
	{
		count += (s1[i] - s2[i])*(s1[i] - s2[i]);
	}
	return count;
}

