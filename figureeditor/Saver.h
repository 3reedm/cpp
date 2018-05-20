#ifndef SAVER_H
#define SAVER_H

#include "Picture.h"
#include "Line.h"
#include "Rectangle.h"
#include "Arc.h"
#include "Ellipse.h"
#include "Curve.h"


enum TAG_NAME {TG_NULL, TG_PICTURE, TG_COMPLEX, TG_LINE, TG_RECTANGLE, TG_ELLIPSE, TG_ARC, TG_CURVE, TG_WIDTH, TG_COLOR};

class Saver{
	Picture* _picture;
	vector<Coord> _coords;
	COLORREF _color;
	unsigned _width;
	bool _creatingComplex;

	bool executeTag(string text, TAG_NAME name) const; //рекурсивная функция интерпретации тег-кода

	void preventiveTagActions( TAG_NAME tagName);
	void fillCoords(string coordsString);

	static TAG_NAME getTagName(string caption);
	static Saver* _selfPointer;
protected:
	static void initialize();
public:
	static Saver* getInst();
	static void destroy();

	//
	void savePicToFile(Picture* picture, LPCWSTR fileName) const;
	Picture* loadPicFromFile(LPCWSTR fileName) const;
};


#endif