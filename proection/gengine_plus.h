//////////definition of class gengine_plus/////
#ifndef GENGINE_PLUS_H                       //
#define GENGINE_PLUS_H                       //
#include "gengine.h"                         //
#include <GdiPlus.h>                         //
#pragma comment(lib, "GdiPlus.lib")          //
using namespace Gdiplus;                     //
///////////////////////////////////////////////
class gengine_plus: public gengine{          //
public:                                      //
 gengine_plus(){}                            //////////////////////////////
 void start(){GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);}//
 void stop(){GdiplusShutdown(gdiplusToken);} /////////////////////////////// 
 void draw(figure);                          //
protected:                                   //
 GdiplusStartupInput gdiplusStartupInput;    //
 ULONG_PTR gdiplusToken;                     // 
};                                           //
///////////////////////////////////////////////
#endif /*GENGINE_PLUS_H*/                    //
/////////////////////////////////////////////// 