#include "Ellipse.h"



bool F_Ellipse::isInRect( Coord r1, Coord r2 ) const {
	Coord add1, add2;
	add1.x = _c1.x;
	add1.y = _c2.y;
	add2.x = _c2.x;
	add2.y = _c1.y;
	if ( pointIsInRect( _c1, r1, r2) || pointIsInRect( _c2, r1, r2) || pointIsInRect( add1, r1, r2) || pointIsInRect( add2, r1, r2) )
		return true;
	return false;
}

Coord F_Ellipse::calculateCenter() const {
	return Coord( (_c1.x + _c2.x)/2, (_c1.y + _c2.y)/2 );
}

F_Ellipse::F_Ellipse(double x1, double y1, double x2, double y2, unsigned width, COLORREF color, bool dash){
	_width = width;
	_color = color;
	_c1 = Coord (x1, y1);
	_c2 = Coord (x2, y2);
	_dashed = dash;
}

void F_Ellipse::draw(HDC hDC) const {
	HPEN hPen;
	if (_dashed)
		hPen = CreatePen(PS_DASH, _width, _color);
	else
		hPen = CreatePen(PS_SOLID, _width, _color);
	SelectObject(hDC, hPen);

	HBRUSH hBrush = ( HBRUSH ) GetStockObject( HOLLOW_BRUSH );
	SelectObject(hDC, hBrush);

	Ellipse(hDC, _c1.x, _c1.y, _c2.x, _c2.y);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void F_Ellipse::drawSelector(HDC hDC) const {
	HPEN hPen = CreatePen(PS_SOLID, 1, 0x00000000);
	SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(0x00ffffff);
	SelectObject(hDC, hBrush);

	Rectangle(hDC, (int)_c1.x - 5, (int)_c1.y - 5, (int)_c1.x + 5, (int)_c1.y + 5);
	Rectangle(hDC, (int)_c2.x - 5, (int)_c2.y - 5, (int)_c2.x + 5, (int)_c2.y + 5);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}


void F_Ellipse::move(double dx, double dy){
	_c1.x += dx;
	_c1.y += dy;
	_c2.x += dx;
	_c2.y += dy;
}

string F_Ellipse::toString() const {
	return string("<ellipse>" + _c1.toString() + _c2.toString()  + "<w>" + toStr(_width) + "</w><c>" + toStr(_color) + "</c></ellipse>");
}