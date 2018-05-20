#include "Line.h"


bool F_Line::isInRect( Coord r1, Coord r2 ) const {
	if ( pointIsInRect( _c1, r1, r2) || pointIsInRect( _c2, r1, r2) )
		return true;
	return false;
}

Coord F_Line::calculateCenter() const {
	return Coord( (_c1.x + _c2.x)/2, (_c1.y + _c2.y)/2 );
}

F_Line::F_Line(double x1, double y1, double x2, double y2, unsigned width, COLORREF color, bool dash){
	_width = width;
	_color = color;
	_c1 = Coord (x1, y1);
	_c2 = Coord (x2, y2);
	_dashed = dash;
}

void F_Line::draw(HDC hDC) const {
	HPEN hPen;
	if (_dashed)
		hPen = CreatePen(PS_DASH, _width, _color);
	else
		hPen = CreatePen(PS_SOLID, _width, _color);
	SelectObject(hDC, hPen);

	MoveToEx(hDC, _c1.x, _c1.y, NULL);
	LineTo(hDC, _c2.x, _c2.y);

	DeleteObject(hPen);
}

void F_Line::drawSelector(HDC hDC) const {
	HPEN hPen = CreatePen(PS_SOLID, 1, 0x00000000);
	SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(0x00ffffff);
	SelectObject(hDC, hBrush);

	Rectangle(hDC, (int)_c1.x - 5, (int)_c1.y - 5, (int)_c1.x + 5, (int)_c1.y + 5);
	Rectangle(hDC, (int)_c2.x - 5, (int)_c2.y - 5, (int)_c2.x + 5, (int)_c2.y + 5);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void F_Line::move(double dx, double dy){
	_c1.x += dx;
	_c1.y += dy;
	_c2.x += dx;
	_c2.y += dy;
}

string F_Line::toString() const {
	return string("<line>" + _c1.toString() + _c2.toString()  + "<w>" + toStr(_width) + "</w><c>" + toStr(_color) + "</c></line>");
}