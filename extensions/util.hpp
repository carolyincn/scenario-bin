// util.hpp

#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string.h>

using namespace std;

typedef struct 
{
	string prefix;
	string face;
	int num;
}NACKINFO;

int getPrefixType(string prefix);
bool comparePrefix(string p1, string p2);
string getPrefixHeader(string prefix, int n);
string packNackInfo(string s);

// extern vector<vector<string>> vs;


#endif // UTIL_H_