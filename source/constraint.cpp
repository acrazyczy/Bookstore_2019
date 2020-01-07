//
// Created by crazy_cloud on 2020/1/7.
//

#include "constraint.h"

std::string convert_double_to_string(const double &val)
{
	std::string ret = std::to_string(val);
	for (int i = 0;i < 4;++ i) ret.pop_back();
	return ret;
}