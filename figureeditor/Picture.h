#ifndef PICTURE_H
#define PICTURE_H

#include "Figure.h"


class Picture{
public:
	vector<Figure*> figures;
	vector<int> selectedIDs;

	~Picture();

	void addFigure(Figure*);
	void clear();
	void draw(HDC) const;

	// операции выделения
	bool trySelectByRect( Coord r1, Coord r2 );
	void selectByID(int id, bool add = false); 	//add - добавлять к существующему списку или очищать его
	void selectLast(bool add = false);
	void unselect();

	// операции с выделенными фигурами
	void selected_Move(double dx, double dy);
	void selected_SetWidth(unsigned value);
	void selected_SetColor(COLORREF color);
	void selected_Merge();
	void selected_Divide();
	void selected_Delete();
	void selected_LayerUp();
	void selected_LayerDown();
};

#endif