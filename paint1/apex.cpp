                        //Class apex
#include <iostream>
#include "apex.h"

//Constructor
apex::apex(double x1, double y1){
 x=x1;
 y=y1;
}

//Copy
apex::apex(apex& two){
 x=two.x;
 y=two.y;
}

//Move
void apex::move(apex& two){
 x+=two.x;
 y+=two.y;
}

//Print
std::ostream operator <<(std::ostream out, apex& one){
  out<<"("<<one.x<<";"<<one.y<<")"<<" ";
}

