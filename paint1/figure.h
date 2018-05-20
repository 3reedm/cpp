//////////definition of class figure///////////
#ifndef FIGURE_H                             //
#define FIGURE_H                             //
#include "apex.h"                            //
#include <string>                            //
///////////////////////////////////////////////
class figure{                                //
public:                                      //
 virtual ~figure(){}                         //
 virtual const std::string& GetName()const=0;//
 virtual apex* GetVal()const=0;              //
 virtual void move(apex&)=0;                 //
 virtual void print()const=0;                //
 void SetColor(unsigned new_color=255255255){//
  m_color=new_color;                         //
 }                                           //
 void SetWidth(unsigned new_width=1){        //
  m_width=new_width;                         //
 }                                           //
protected:                                   //
 unsigned m_color;                           //
 unsigned m_width;                           //
};                                           //
///////////////////////////////////////////////
#endif /*FIGURE_H*/                          //
///////////////////////////////////////////////
