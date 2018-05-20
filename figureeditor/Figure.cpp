#include "Figure.h"



/******************************* Figure *******************************/

bool Figure::pointIsInRect(Coord p, Coord r1, Coord r2) const {
	if ( p.x >= min(r1.x, r2.x) && p.x <= max(r1.x, r2.x)  &&  p.y >= min(r1.y, r2.y) && p.y <= max(r1.y, r2.y) )				//min / max ?
		return true;
	return false;
}

//get
unsigned Figure::getWidth() const {
	return _width;
}

COLORREF Figure::getColor() const {
	return _color;
}

//set
void Figure::setWidth(unsigned value){
	_width = value;
}

void Figure::setColor(COLORREF value){
	_color = value;
}

vector<Figure*>* Figure::getNestedFigures() const {
	vector<Figure*>* v = new vector<Figure*>;
	return v;
}


/**************************** Complex Figure ***************************/

ComplexFigure::~ComplexFigure(){
	_figures.clear();
}

ComplexFigure::ComplexFigure(){
	_width = 1;
	_color = 0x00000000;
}

void ComplexFigure::addFigure(Figure* pFigure){
	_figures.push_back(pFigure);
}

void ComplexFigure::draw(HDC hDC) const {
	for (int i = 0; i < _figures.size(); i++)
		_figures[i]->draw(hDC);
}

void ComplexFigure::drawSelector(HDC hDC) const {
	for (int i = 0; i < _figures.size(); i++)
		_figures[i]->drawSelector(hDC);
}

void ComplexFigure::move(double dx, double dy){
	for (int i = 0; i < _figures.size(); i++)
		_figures[i]->move(dx, dy);
}

void ComplexFigure::setWidth(unsigned value){
	Figure::setWidth(value);
	for (int i = 0; i < _figures.size(); i++)
		_figures[i]->setWidth( value );
}

void ComplexFigure::setColor(COLORREF value){
	Figure::setColor(value);
	for (int i = 0; i < _figures.size(); i++)
		_figures[i]->setColor( value );
}

bool ComplexFigure::isInRect( Coord r1, Coord r2 ) const {
	for (int i = 0; i < _figures.size(); i++)
		if ( _figures[i]->isInRect( r1, r2 ) )
			return true;
	return false;
}

vector<Figure*>* ComplexFigure::getNestedFigures() const {
	vector<Figure*>* v = new vector<Figure*>;
	for (int i = 0; i < _figures.size(); i++ )
		v->push_back( _figures[i] );
	return v;
}

Coord ComplexFigure::calculateCenter() const {
	Coord result(0,0), simpleFigCenter;
	int count =  _figures.size();

	for ( int i = 0; i < count; i++ ){
		simpleFigCenter = _figures[i]->calculateCenter();
		result.x += simpleFigCenter.x;
		result.y += simpleFigCenter.y;
	}
	result.x /= count;
	result.y /= count;

	return result;
}

string ComplexFigure::toString() const {
	string res = "<complex>\n";

	for (int i = 0; i < _figures.size(); i++)
		res += _figures[i]->toString() + "\n";

	res += "</complex>";
	return res;
}