#ifndef FIGURE_H
#define FIGURE_H

#include "windows.h"
#include "math.h"
#include "Coord.h"
#include <vector>
using namespace std;


/******************************* Figure *******************************/

class Figure{
protected:
	unsigned _width;
	COLORREF _color;
	bool _dashed;

	bool pointIsInRect(Coord p, Coord r1, Coord r2) const;
public:
	virtual ~Figure(){};

	virtual void draw(HDC) const = 0;
	virtual void drawSelector(HDC) const = 0;
	virtual void move(double, double) = 0;
	virtual bool isInRect( Coord r1, Coord r2 ) const = 0;
	//get
	virtual string toString() const = 0;
	unsigned getWidth() const;
	COLORREF getColor() const;
	virtual Coord calculateCenter() const = 0;
	virtual vector<Figure*>* getNestedFigures() const;	//используется для разбития сложных фигур
	//set
	virtual void setWidth(unsigned);
	virtual void setColor(COLORREF);
};



/**************************** Complex Figure ***************************/

class ComplexFigure: public Figure{
	vector<Figure*> _figures;

public:
	ComplexFigure();
	~ComplexFigure();

	void addFigure(Figure*);

	void draw(HDC hDC) const;
	void drawSelector(HDC hDC) const;
	void move(double dx, double dy);
	bool isInRect( Coord r1, Coord r2 ) const;
	//get
	string toString() const;
	Coord calculateCenter() const;
	vector<Figure*>* getNestedFigures() const;
	//set
	void setWidth(unsigned);
	void setColor(COLORREF);
};


#endif