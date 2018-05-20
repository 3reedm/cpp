#include "Pifagor.h"



void PifagorTree(int n, int a, Coord c0, double fi, double alpha){
	double b, c;
	Coord c1, c2, c3, c4;

	b = a*cos(alpha);
	c = a*sin(alpha);

	c1.x = c0.x + a*cos(fi);
	c1.y = c0.y + a*sin(fi);
	
	c3.x = c0.x + a*cos(fi+PI/2);
	c3.y = c0.y + a*sin(fi+PI/2);

	//c2.x = c1.x + 









}



