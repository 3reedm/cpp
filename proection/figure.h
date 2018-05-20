//////////definition of class figure///////////
#ifndef FIGURE_H                             //
#define FIGURE_H                             //
#include <vector>                            //
#include "apex.h"                            //
class gengine;                               //
///////////////////////////////////////////////
class figure{                                //
public:                                      //
 figure(){}                                  //
 ~figure(){as.clear();}                      //
 apex* get_val();                            // 
 int get_size(){return as.size();}           //
 double* get_EV();                           //
 void set_rob(int);                          //
 void add(apex);                             //
 void draw(gengine*);                        //
 void create();                              //
 void twist(int);                            //
 void distance(int);                         //
private:                                     //
 std::vector<std::vector<double> > V;        //
 std::vector<double> EV;                     //
 std::vector<apex> as;                       //
};                                           //
///////////////////////////////////////////////
#endif /*FIGURE_H*/                          //