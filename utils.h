#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <cmath>
#include <iostream>

#include <string>

#ifndef UTILS_H
#define UTILS_H

using namespace std;

#define rand_f() ((float) rand() / (RAND_MAX))

static string toString(float f){
	stringstream sss;
	sss << f;
	return sss.str();
}

#endif