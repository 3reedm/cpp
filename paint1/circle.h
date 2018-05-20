//////////definition of class circle///////////
#ifndef CIRCLE_H                             //
#define CIRCLE_H                             //
#include <string>                            //
#include "figure.h"                          //
///////////////////////////////////////////////
class circle: public figure{                 //
public:                                      //
 circle(std::string name1): name(name1){}    ////////////////////////
 circle(std::string name1, apex* val1): name(name1), val(val1){}   //
 const std::string& GetName()const{return name;}                   //
 apex* GetVal()const{return val;}            ////////////////////////
 void move(apex&);                           //
 void print()const;                          //
protected:                                   //
 std::string name;                           //
 apex *val;                                  //
};                                           //
///////////////////////////////////////////////
#endif /*CIRCLE_H*/                          //
///////////////////////////////////////////////
