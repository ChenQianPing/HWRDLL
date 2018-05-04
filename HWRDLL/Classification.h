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

	// 计算两个样品的匹配程度 ,返回两各样品的的匹配程度。
	double pipei(double s1[], double s2[]);
	// 最小距离法 ,返回数字类别和编号 最邻近匹配模板法
	number_no LeastDistance();
	// 返回最邻近匹配模板法Result
	int GetNumberByLeastDistance();

};

