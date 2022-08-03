#pragma once

#include <vector>

using namespace std;

union ByteToL
{
	unsigned int uiResult;
	int iResult;
	float fResult;
	char buff[4];
};


extern std::string sSelectedFile;
extern std::string sFilePath;
bool openFile();



std::string ofTrim(const std::string & src, const std::string & locale = "");
std::string ofTrimBack(const std::string & src, const std::string & locale = "");
std::string ofTrimFront(const std::string & src, const std::string & locale = "");
vector <string> ofSplitString(const string& source, const string& delimiter, bool ignoreEmpty = false, bool trim = false);



size_t count_line(istream& is);
vector<string> split(string s, string delimiter);
