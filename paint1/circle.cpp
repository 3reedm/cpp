                        //Class circle
#include <iostream>
#include "circle.h"

//Move
void circle::move(apex& two){
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  val[i].move(two);
}

//Print
void circle::print()const{
 int n=sizeof(val)/sizeof(val[0]);
 for(int i=0;i<n;i++)
  std::cout<<val[i];
}
