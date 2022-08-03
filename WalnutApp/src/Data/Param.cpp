#include "Param.h"

//#include <iostream>


Param::Param(const string & n)
{
	name = n;
	//std::cout << "Param: " << name << " was created +++ " << endl;
	printf("\tParam: %s created +++ \n", name);
}

Param::~Param()
{
	//std::cout << "Param: " << name << " was deleted --- " << endl;
	printf("\tParam: %s Deleted --- \n", name);
}
