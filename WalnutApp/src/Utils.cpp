#include <Windows.h>
#include <string>
#include <shobjidl.h> 

#include <algorithm>
#include <locale>


#include "Utils.h"

#include <iostream>
#include <istream>
#include <fstream>


std::string sSelectedFile;
std::string sFilePath;

bool openFile()
{
    //  CREATE FILE OBJECT INSTANCE
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
        return FALSE;

    // CREATE FileOpenDialog OBJECT
    IFileOpenDialog* f_FileSystem;
    f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        CoUninitialize();
        return FALSE;
    }

    //  SHOW OPEN FILE DIALOG WINDOW
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
    IShellItem* f_Files;
    f_SysHr = f_FileSystem->GetResult(&f_Files);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  STORE AND CONVERT THE FILE NAME
    PWSTR f_Path;
    f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr)) {
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  FORMAT AND STORE THE FILE PATH
    std::wstring path(f_Path);
    std::string c(path.begin(), path.end());
    sFilePath = c;

    //  FORMAT STRING FOR EXECUTABLE NAME
    const size_t slash = sFilePath.find_last_of("/\\");
    sSelectedFile = sFilePath.substr(slash + 1);

    //  SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Files->Release();
    f_FileSystem->Release();
    CoUninitialize();
    return TRUE;
}


//--------------------------------------------------
// helper method to get locale from name
static std::locale getLocale(const string & locale) {
std::locale loc;
#if defined(TARGET_WIN32) && !_MSC_VER
	static bool printonce = true;
	if( printonce ){
		std::string current( setlocale(LC_ALL,NULL) );
		setlocale (LC_ALL,"");
		ofLogWarning("ofUtils") << "std::locale not supported. Using C locale  :" << current ;
		printonce = false;
	}
#else
	try {
		loc = std::locale(locale.c_str());
	}
	catch (...) {
		//ofLogWarning("ofUtils") << "Couldn't create locale " << locale << " using default, " << loc.name();
	}
#endif
	return loc;
}
//--------------------------------------------------
string ofTrimFront(const string & src, const string& locale){
    auto dst = src;
    std::locale loc = getLocale(locale);
    dst.erase(dst.begin(),std::find_if_not(dst.begin(),dst.end(),[&](char & c){return std::isspace<char>(c,loc);}));
    return dst;
}

//--------------------------------------------------
string ofTrimBack(const string & src, const string& locale){
    auto dst = src;
    std::locale loc = getLocale(locale);
	dst.erase(std::find_if_not(dst.rbegin(),dst.rend(),[&](char & c){return std::isspace<char>(c,loc);}).base(), dst.end());
	return dst;
}
//--------------------------------------------------
string ofTrim(const string & src, const string& locale){
    return ofTrimFront(ofTrimBack(src));
}
//--------------------------------------------------
vector <string> ofSplitString(const string & source, const string & delimiter, bool ignoreEmpty, bool trim) {
	vector<string> result;
	if (delimiter.empty()) {
		result.push_back(source);
		return result;
	}
	string::const_iterator substart = source.begin(), subend;
	while (true) {
		subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
		string sub(substart, subend);
		if(trim) {
			sub = ofTrim(sub);
		}
		if (!ignoreEmpty || !sub.empty()) {
			result.push_back(sub);
		}
		if (subend == source.end()) {
			break;
		}
		substart = subend + delimiter.size();
	}
	return result;
}


vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

size_t count_line(istream& is)
{
	// skip when bad
	if (is.bad()) return 0;
	// save state
	std::istream::iostate state_backup = is.rdstate();
	// clear state
	is.clear();
	streampos pos_backup = is.tellg();

	is.seekg(0);
	size_t line_cnt;
	size_t lf_cnt = std::count(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), '\n');
	line_cnt = lf_cnt;
	// if the file is not end with '\n' , then line_cnt should plus 1  
	is.unget();
	if (is.get() != '\n') { ++line_cnt; }

	// recover state
	is.clear(); // previous reading may set eofbit
	is.seekg(pos_backup);
	is.setstate(state_backup);

	return line_cnt;
}


