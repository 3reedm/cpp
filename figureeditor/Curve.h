#ifndef CURVE_H
#define CURVE_H

#include "Figure.h"


class F_Curve: public Figure{
	vector<Coord> _coords;
public:
	F_Curve(vector<Coord> coords, unsigned width=1, COLORREF color=0x00000000, bool dash=false);
	~F_Curve(){};

	string toString() const;
	void draw(HDC hDC) const;
	void drawSelector(HDC hDC) const;
	void move(double dx, double dy);
	Coord calculateCenter() const;
	bool isInRect( Coord r1, Coord r2 ) const;
};


#endif