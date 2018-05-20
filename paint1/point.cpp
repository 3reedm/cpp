                        //Class point
#include <iostream>
#include "point.h"

//Move
void point::move(apex& two){
  val->move(two);
}

//Print
void point::print()const{
  std::cout<<(*val);
}