                        //Class line
#include <iostream>
#include "line.h"

//Move
void line::move(apex& two){
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  val[i].move(two);
}

//Print
void line::print()const{
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  std::cout<<val[i];
}