#include "DataModel.h"

//#include <iostream>

DataModel::DataModel()
{
	//cout << "Data Model Created" << endl;
	printf("Data Model Created\n");
}

DataModel::~DataModel()
{
	for (auto s : sensors)
	{
		delete s;
	}
	sensors.clear();

	//cout << "Data Model Deleted" << endl;
	printf("Data Model Deleted\n");
}
