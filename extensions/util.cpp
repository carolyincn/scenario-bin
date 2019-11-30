// util.cpp

#include "util.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace std;

int getPrefixType(string prefix)
{
	int count = 0;
	for(int i = 0; i < prefix.size(); i ++){
		if(prefix[i] == '/')
			count += 1;
	}
	return count;
	// return count(prefix.begin(), prefix.end(), "/");
}

bool comparePrefix(string p1, string p2)
{
	string com;
	int mincount = p1.size() > p2.size() ? p2.size() : p1.size();
	// int i;
	for(int i = 0; i < mincount; i ++){
		if(p1[i] == p2[i])
			com = com + p1[i];
		else
			break;
	}
	if (com[com.size()-1] == '/')
		com.pop_back();
	if (getPrefixType(com) >= 2)
		return 1;
	else
		return 0;
}

// get the first n name of prefix
string getPrefixHeader(string prefix, int n)
{
	int count = 0;
	string header;
	for(int i = 0; i < prefix.size(); i ++){
		if(prefix[i] == '/'){
			count += 1;
			if(count > n)
				break;
			// count += 1;
		}
		header += prefix[i];
	}
	return header;
}

// integrate nack info by prefix and face, return "_prefix_ _face_ _number_\t..."
string packNackInfo(string s)
{
	int num = 0;
	string temp;
	NACKINFO info;
	vector<NACKINFO> nlist;
	bool fd;
	int j = 0;
	for(int i = 0; i < s.size(); i ++){
		if(s[i] != ' '){
			// temp = "";
			if(s[i] != '\t'){
				temp += s[i];
			} else{

				info.prefix = temp;
				temp = "";
			}
		} else{
			info.face = temp;
			temp = "";
			j ++;
			fd = 0;
			for(vector<NACKINFO>::iterator it=nlist.begin(); it != nlist.end(); it ++){
				if(comparePrefix(it->prefix, info.prefix)){
					// cout << "123" << endl;
					if(strcmp(it->face.c_str(), info.face.c_str()) == 0){
						// cout << "456" << endl;
						it->num += 1;
						fd = 1;
						break;
					}
				}
			}
			if(not fd){
				info.num = 1;
				temp = getPrefixHeader(info.prefix, 2);
				info.prefix = temp;
				temp = "";

				nlist.push_back(info);
			}
			
		}
	}
	
	string ret;
	for(vector<NACKINFO>::iterator it=nlist.begin(); it != nlist.end(); it ++){
			ret = ret + it->prefix + " " + it->face + " " + to_string(it->num) + "\t"; 
		// std::cout << it->prefix << " " << it->face << " " << it->num << std::endl;
		// printf("=========%s============", it->prefix.c_str());
	}
	return ret;
}