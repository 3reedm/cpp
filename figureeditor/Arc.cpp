#include "Arc.h"
#include "Ellipse.h"
#include "Line.h"


bool F_Arc::isInRect( Coord r1, Coord r2 ) const {
	Coord add1, add2;
	add1.x = _c1.x;
	add1.y = _c2.y;
	add2.x = _c2.x;
	add2.y = _c1.y;
	if ( pointIsInRect( _c1, r1, r2) || pointIsInRect( _c2, r1, r2) || pointIsInRect( add1, r1, r2) || pointIsInRect( add2, r1, r2) )
		return true;
	return false;
}

Coord F_Arc::calculateCenter() const {
	return Coord( (_c1.x + _c2.x)/2, (_c1.y + _c2.y)/2 );
}

F_Arc::F_Arc(Coord e1, Coord e2, Coord r1, Coord r2, unsigned width, COLORREF color, bool dash){
	_width = width;
	_color = color;
	_c1 = e1;
	_c2 = e2;
	_r1 = r1;
	_r2 = r2;
	_dashed = dash;
}

void F_Arc::draw(HDC hDC) const {
	HPEN hPen;
	if (_dashed)
		hPen = CreatePen(PS_DASH, _width, _color);
	else
		hPen = CreatePen(PS_SOLID, _width, _color);
	SelectObject(hDC, hPen);

	Arc( hDC, _c1.x, _c1.y, _c2.x, _c2.y, _r1.x, _r1.y, _r2.x, _r2.y );

	DeleteObject(hPen);
}

void F_Arc::drawSelector(HDC hDC) const {
	Coord center = this->calculateCenter();
	F_Line radius1( center.x, center.y, _r1.x, _r1.y, 1, 0, true );
	F_Line radius2( center.x, center.y, _r2.x, _r2.y, 1, 0, true );
	radius1.draw(hDC);
	radius2.draw(hDC);

	HPEN hPen = CreatePen(PS_SOLID, 1, 0x00000000);
	SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(0x00ffffff);
	SelectObject(hDC, hBrush);

	Rectangle(hDC, (int)_c1.x - 5, (int)_c1.y - 5, (int)_c1.x + 5, (int)_c1.y + 5);
	Rectangle(hDC, (int)_c2.x - 5, (int)_c2.y - 5, (int)_c2.x + 5, (int)_c2.y + 5);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(_color);
	SelectObject(hDC, hBrush);
	Rectangle(hDC, (int)_r1.x - 5, (int)_r1.y - 5, (int)_r1.x + 5, (int)_r1.y + 5);
	Rectangle(hDC, (int)_r2.x - 5, (int)_r2.y - 5, (int)_r2.x + 5, (int)_r2.y + 5);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void F_Arc::move(double dx, double dy){
	_c1.x += dx;
	_c1.y += dy;
	_c2.x += dx;
	_c2.y += dy;
	_r1.x += dx;
	_r1.y += dy;
	_r2.x += dx;
	_r2.y += dy;
}

string F_Arc::toString() const {
	return string("<arc>"+_c1.toString()+_c2.toString()+_r1.toString()+_r2.toString()+"<w>"+toStr(_width)+"</w><c>"+toStr(_color)+"</c></arc>");
}