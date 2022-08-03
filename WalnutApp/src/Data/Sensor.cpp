#include "Sensor.h"

//#include <iostream>

Sensor::Sensor(string n,int i)
{
	id = i;
	name = n;

	//cout << "Created: " << name << " Sensor ++ " << endl;
	printf("Sensor: %s Created ++ \n", name);
}

Sensor::~Sensor()
{
	for (auto v : params) {
		for (auto p : v)
		{
			delete p;
		}
	}
	params.clear();

	//cout << "Delete: " << name << " Sensor -- " << endl;
	printf("Sensor: %s Deleted -- \n", name);

}
