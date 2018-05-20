#ifndef COORD_H
#define COORD_H
#define PI 3.1415926

#include "toString.h"


class Coord{
public:
	double x, y;

	Coord(double x=0, double y=0);
	Coord(string s);

	string toString() const;
};

#endif
