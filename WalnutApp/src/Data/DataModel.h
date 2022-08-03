#pragma once

#include "Sensor.h"

class DataModel
{
public:

	DataModel();
	~DataModel();

	vector<Sensor*> sensors;
};

