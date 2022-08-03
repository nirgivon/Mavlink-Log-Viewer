#pragma once

#include <vector>
#include <string>

using namespace std;


class Param
{
	public:
	string name;
	string units;
	vector<float> values;

	bool isSelected = false;
	bool prevIsSelected = false;
	//int graphIndex = -1;

	Param() {};
	Param(const string & n);
	~Param();
};

