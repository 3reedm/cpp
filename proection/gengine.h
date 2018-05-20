//////////definition of class gengine//////////
#ifndef GENGINE_H                            //
#define GENGINE_H                            //
#include <windows.h>                         //
#include "figure.h"                          //
///////////////////////////////////////////////
class gengine{                               //
public:                                      //
 virtual ~gengine(){}                        //
 void getHDC(HDC hdc1){hdc=hdc1;}            //
 void getO(int a, int b){Ox=a; Oy=b;}        //
 virtual void start()=0;                     //
 virtual void stop()=0;                      //
 virtual void draw(figure)=0;                //
protected:                                   // 
 HDC hdc;                                    //
 int Ox,Oy;                                  //
};                                           //
///////////////////////////////////////////////
#endif /*GENGINE_H*/                         //
///////////////////////////////////////////////  