#ifndef IFC_H
#define IFC_H

#include "Figure.h"
#include "Curve.h"
#include "Line.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "Arc.h"


/****************** IFC (InteractiveFigureConstructor) ********************/
/**************************** (Singletone) ********************************/

enum FigureType{FT_NULL, FT_POINT, FT_LINE, FT_ELLIPSE, FT_RECTANGLE, FT_CURVE, FT_ARC, FT_LAST};

class IFC{	
	vector<Coord> _coords, _additionalCoords;
	vector<Figure*> _tmpFigureParts;

	int _width;
	COLORREF _color;

	FigureType _task;
	int _coordsLeft;

	Coord _mouseCoords;

	void updateFigureParts();

	static FigureType _lastFigureType;
	static IFC* _selfPointer;
protected:
	IFC();
	~IFC();
	static void initialize();
public:
	static IFC* getInst();
	static void destroy();

	bool isInProcess() const;
	void setTask(FigureType, unsigned width, COLORREF color);
	void clearTask();
	
	void displayIntermediateRes(HDC hDC);

	//on move
	void updateMouseCoords(double, double);
	//on click
	Figure* fixCurrentCoord();//фиксирует последнюю координату. В случае если создание фигуры закончено возвращает фигуру, в противном случае 0.
};


#endif

