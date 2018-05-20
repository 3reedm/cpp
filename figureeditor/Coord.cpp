#include "Coord.h"

Coord::Coord(double X, double Y){
	x=X;
	y=Y;
}

Coord::Coord(string s){
	string sX = "", sY = "";
	int l = s.length();
	char ch;
	bool writeY = false;
	
	for (int i = 1; i < l; i++){
		ch = s[i];
		if ( isdigit(ch) || ch == '.' || ch=='-')
			if (writeY) 
				sY += ch;
			else 
				sX += ch;
		if ( ch == ',')
			writeY = true;
	}

	x = atof(sX.c_str());
	y = atof(sY.c_str());
}

string Coord::toString() const {
	return string("(" + toStr(x) + ", " + toStr(y) + ")");
}