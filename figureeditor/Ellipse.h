#ifndef F_Ellipse_H
#define F_Ellipse_H

#include "Figure.h"


class F_Ellipse: public Figure{
	Coord _c1, _c2;
public:
	F_Ellipse(double x1, double y1, double x2, double y2, unsigned width=1, COLORREF color=0x00000000, bool dash=false);
	~F_Ellipse(){};

	string toString() const;
	void draw(HDC hDC) const;
	void drawSelector(HDC hDC) const;
	void move(double dx, double dy);
	Coord calculateCenter() const;
	bool isInRect( Coord r1, Coord r2 ) const;
};


#endif