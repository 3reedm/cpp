                        //Class triangle
#include <iostream>
#include "triangle.h"

//Move
void triangle::move(apex& two){
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  val[i].move(two);
}

//Print
void triangle::print()const{
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  std::cout<<val[i];
}