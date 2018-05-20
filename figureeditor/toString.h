#ifndef TO_STRING_H
#define TO_STRING_H

#include <string>
#include <sstream>
#include "Windows.h"
using namespace std;


template <typename T>
string toStr(T const& value) {
	stringstream sstr;	
	//sstr.setf(ios::fixed, ios::floatfield);
	//sstr.precision(3);
	sstr << value;
	return sstr.str();
}

inline wstring StoWS(const string s){
	int len, slength;
	slength = s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}

template <typename T>
wstring toWStr(T const& value) {
	stringstream sstr;
	sstr << value;
	return StoWS( sstr.str() );
}

template <typename T>
wstring toWStrHEX(T const& value) {
	stringstream sstr;
	sstr << std::hex << value;
	return StoWS( sstr.str() );
}


#endif