#include "Curve.h"


bool F_Curve::isInRect( Coord r1, Coord r2 ) const {
	for (int i=0; i<_coords.size(); i++)
		if (pointIsInRect( _coords[i], r1, r2))
			return true;
	return false;
}

Coord F_Curve::calculateCenter() const {
	int n = _coords.size();
	double xSum = 0, ySum = 0;
	for (int i=0; i<n; i++){
		xSum += _coords[i].x;
		ySum += _coords[i].y;
	}
	return Coord(xSum/n, ySum/n);
}

F_Curve::F_Curve(vector<Coord> coords, unsigned width, COLORREF color, bool dash){
	_width = width;
	_color = color;
	_coords = coords;
	_dashed = dash;
}

void F_Curve::draw(HDC hDC) const {
	if (_width <= 2 && _coords.size() == 1){
		SetPixel(hDC, _coords[0].x, _coords[0].y, _color);
		return;
	}

	HPEN hPen;
	if (_dashed)
		hPen = CreatePen(PS_DASH, _width, _color);
	else
		hPen = CreatePen(PS_SOLID, _width, _color);
	SelectObject(hDC, hPen);

	Coord c1, c2 = _coords[0];
	for (int i=0; i<_coords.size(); i++){
		c1 = c2;
		c2 = _coords[i];

		MoveToEx(hDC, c1.x, c1.y, NULL);
		LineTo(hDC, c2.x, c2.y);
	}

	DeleteObject(hPen);
}

void F_Curve::drawSelector(HDC hDC) const {
	HPEN hPen = CreatePen(PS_SOLID, 1, 0x00000000);
	SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(0x00ffffff);
	SelectObject(hDC, hBrush);

	Coord c1 = _coords[0];
	Coord c2 = _coords[_coords.size()-1];

	Rectangle(hDC, (int)c1.x - 5, (int)c1.y - 5, (int)c1.x + 5, (int)c1.y + 5);
	Rectangle(hDC, (int)c2.x - 5, (int)c2.y - 5, (int)c2.x + 5, (int)c2.y + 5);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void F_Curve::move(double dx, double dy){
	for (int i=0; i<_coords.size(); i++){
		_coords[i].x += dx;
		_coords[i].y += dy;
	}
}

string F_Curve::toString() const {
	string res = "<curve>";

	for (int i = 0; i < _coords.size(); i++)
		res += _coords[i].toString();

	res += "<w>" + toStr(_width) + "</w><c>" + toStr(_color) + "</c></curve>";
	return res;
}