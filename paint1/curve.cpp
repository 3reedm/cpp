                        //Class curve
#include <iostream>
#include "curve.h"

//Move
void curve::move(apex& two){
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  val[i].move(two);
}

//Print
void curve::print()const{
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  std::cout<<val[i];
}