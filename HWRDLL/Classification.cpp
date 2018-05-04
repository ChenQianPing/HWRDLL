#include "stdafx.h"
#include "Classification.h"


Classification::Classification()
{
}


Classification::~Classification()
{
}

/******************************************************************
*   函数名称：LeastDistance()
*   函数类型：number_no，结构体
*   函数功能：最小距离法 ,返回数字类别和编号
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
				// 匹配的最小值
				min = pipei(pattern[n].feature[i], testsample);
				number_no.number = n;   // 样品类别
				number_no.no = i;       // 样品序号
			}
		}
	}
	return number_no;// 返回手写数字的类别和序号
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
				//匹配的最小值
				min = pipei(pattern[n].feature[i], testsample);
				number_no.number = n;//样品类别
				number_no.no = i;//样品序号
			}
		}
	}

	return number_no.number;//返回手写数字的类别和序号
}


/****************************************************************
*   函数名称：pipei(double s1[], double s2[])
*   函数类型：double
*   参数说明：double s1[], double s2[]:两个样品的特征
*   函数功能：计算两个样品的匹配程度 ,返回两各样品的的匹配程度。
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

