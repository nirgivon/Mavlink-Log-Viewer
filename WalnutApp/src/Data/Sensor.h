#pragma once

#include "Param.h"

#include <unordered_map>

using namespace std;

class Sensor
{
public:
	int id = 0;
	int count = 0;
	string name;

	string FMTU;
	string MULT;

	//Sensor();
	Sensor(string n, int i);
	~Sensor();

	vector<vector<Param*>> params;
	//vector<Param*> params;
};

