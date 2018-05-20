#ifndef ARC_H
#define ARC_H

#include "Figure.h"


class F_Arc: public Figure{
	Coord _c1, _c2, _r1, _r2;
public:
	F_Arc(Coord e1, Coord e2, Coord r1, Coord r2, unsigned width=1, COLORREF color=0x00000000, bool dash=false);
	~F_Arc(){};

	void draw(HDC hDC) const;
	void drawSelector(HDC hDC) const;
	string toString() const;
	void move(double dx, double dy);
	Coord calculateCenter() const;
	bool isInRect( Coord r1, Coord r2 ) const;
};




#endif