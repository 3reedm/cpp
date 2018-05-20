                        //Paint
#include <iostream>
#include <vector>
#include "figure.h"
#include "point.h"
#include "line.h"
#include "curve.h"
#include "triangle.h"
#include "tetragon.h"
#include "circle.h"

 int select_item(int ni);
int main(){
 bool flag=false;
 std::vector<figure*> Obj;
do{
 std::cout<<"___________________"<<std::endl;
 std::cout<<"Select object type:"<<std::endl;
 std::cout<<"1. Point"<<std::endl;
 std::cout<<"2. Line"<<std::endl;
 std::cout<<"3. Curve"<<std::endl;
 std::cout<<"4. Triangle"<<std::endl;
 std::cout<<"5. Tetragon"<<std::endl;
 std::cout<<"6. Circle"<<std::endl;
 int item=select_item(6);
     
 std::string name;
 apex* val;
 std::cout<<"Enter object name: ";
 std::cin>>name;
 std::cout<<std::endl;
 std::cin.get();
 if(item==1){
  double x, y;
  std::cout<<"Enter coordinates:";
  std::cout<<std::endl<<"x: ";
  std::cin>>x;
  std::cout<<"y: ";
  std::cin>>y;
  apex A(x,y);
 }
 if(item==2){
  double x[2], y[2];
  std::cout<<"Enter coordinates of apexes:";
  for(int i=0; i<2; i++){
    std::cout<<std::endl<<"x["<<i+1<<"]: ";
    std::cin>>x[i];
    std::cout<<"y["<<i+1<<"]: ";
    std::cin>>y[i];
  }
  apex A(x[0],y[0]), B(x[1],y[1]);
  apex p[]={A,B};
  val=p;
 }
 if(item==3){
  double x[4], y[4];
  std::cout<<"Enter coordinates of apexes:";
   for(int i=0; i<4; i++){
    std::cout<<std::endl<<"x["<<i+1<<"]: ";
    std::cin>>x[i];
    std::cout<<"y["<<i+1<<"]: ";
    std::cin>>y[i];
   }
  apex A(x[0],y[0]), B(x[1],y[1]), C(x[2],y[2]), D(x[3],y[3]);
  apex p[]={A,B,C,D};
  val=p;
 }
 if(item==4){
  double x[3], y[3];
  std::cout<<"Enter coordinates of apexes:";
  for(int i=0; i<3; i++){
   std::cout<<std::endl<<"x["<<i+1<<"]: ";
   std::cin>>x[i];
   std::cout<<"y["<<i+1<<"]: ";
   std::cin>>y[i];
  }
  apex A(x[0],y[0]), B(x[1],y[1]), C(x[2],y[2]);
  apex p[]={A,B,C};
  val=p;
 }
 if(item==5){
  double x[4], y[4];
  std::cout<<"Enter coordinates of apexes:";
   for(int i=0; i<4; i++){
    std::cout<<std::endl<<"x["<<i+1<<"]: ";
    std::cin>>x[i];
    std::cout<<"y["<<i+1<<"]: ";
    std::cin>>y[i];
   }
  apex A(x[0],y[0]), B(x[1],y[1]), C(x[2],y[2]), D(x[3],y[3]);
  apex p[]={A,B,C,D};
  val=p;
 }
 if(item==6){
  double x[2], y[2];
  std::cout<<"Enter coordinates of apexes:";
  for(int i=0; i<2; i++){
   std::cout<<std::endl<<"x["<<i+1<<"]: ";
   std::cin>>x[i];
   std::cout<<"y["<<i+1<<"]: ";
   std::cin>>y[i];
  }
  apex A(x[0],y[0]), B(x[1],y[1]);
  apex p[]={A,B};
  val=p;
 }  
  
 char choose;
 figure* nobj;
 switch(item){
  case 1:
   nobj=new point(name, val);
   break;
  case 2:
   nobj=new line(name, val);
   break;
  case 3:
   nobj=new curve(name, val);
   break;
  case 4:
   nobj=new triangle(name, val);
   break;
  case 5:
   nobj=new tetragon(name, val);
   break;
  case 6:
   nobj=new circle(name, val);
   break;
  }

  Obj.push_back(nobj);
  std::cout<<"Object added"<<std::endl<<std::endl;
  std::cout<<"It's all over? (Y/N)"<<std::endl;
  std::cin>>choose;
  if(choose=='Y'==1)
  flag=true;
 }while(!flag);

for (std::vector<figure*>::const_iterator it = Obj.begin(); it != Obj.end(); ++it) {
    (**it).print();
  }
std::cin.get();
std::cin.get();
return 0;
}

int select_item(int ni){
  std::cout<<"___________________"<<std::endl;
  int item;
  while(true){
   std::cin>>item;
   if((item>0) && (item<=ni) && (std::cin.peek()=='\n')){
    std::cin.get(); break;
   }
   else{
    std::cout<<"Error! (must be number from 1 to"<<ni<<"):"<<std::endl;
    std::cin.clear();
    while (std::cin.get()!='\n'){};
   }
  }
  return item;
}