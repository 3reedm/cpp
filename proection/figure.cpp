                                     //Class figure

#include <iostream>
#include "figure.h"
#include "gengine.h"


void figure::add(apex a){ 
 as.push_back(a);
}

double* figure::get_EV(){
  double* tmp;
  if(as.size()==8){
   tmp=new double[6];
   for(int i=0; i<6; ++i)
    tmp[i]=EV[i];
  }
  if(as.size()==5){
   tmp=new double[5];
   for(int i=0; i<5; ++i)
    tmp[i]=EV[i];
  }
  return tmp;
}

apex* figure::get_val(){
 unsigned i,n=as.size();
 apex* tmp=new apex[n];
 for(i=0; i<n; ++i)
  tmp[i]=as[i];
 return tmp;
}

void figure::draw(gengine* ge){
  ge->draw(*this);
}

void figure::set_rob(int n){
 std::vector<double> D;
 if(n==8){
  D.push_back(2); D.push_back(0); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(-2); D.push_back(0); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(2); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(-2); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(0); D.push_back(2); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(0); D.push_back(-2); D.push_back(-1);
  V.push_back(D);
  D.clear();
  EV.push_back(0); EV.push_back(0); EV.push_back(0); EV.push_back(0); EV.push_back(2); EV.push_back(-2);
 }
 if(n==5){ 
  D.push_back(0); D.push_back(-2); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(2); D.push_back(4); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(-4); D.push_back(2); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(0); D.push_back(2); D.push_back(-4); D.push_back(-1);
  V.push_back(D);
  D.clear();
  D.push_back(4); D.push_back(2); D.push_back(0); D.push_back(-1);
  V.push_back(D);
  D.clear();
  EV.push_back(0); EV.push_back(4); EV.push_back(0); EV.push_back(-4); EV.push_back(0);
 }	
}

void figure::create(){
  double tmp[4]; 
  for(unsigned i=0; i<as.size(); ++i){
   tmp[0]=as[i].x;
   tmp[1]=as[i].y;
   tmp[2]=as[i].z;
   tmp[3]=as[i].s;
   as[i].x=tmp[0]*1+tmp[1]*0+tmp[2]*0+tmp[3]*0;
   as[i].y=tmp[0]*0+tmp[1]*1+tmp[2]*0+tmp[3]*0;
   as[i].z=tmp[0]*0+tmp[1]*0+tmp[2]*1+tmp[3]*0;
   if(as.size()==8)
    as[i].s=tmp[0]*0+tmp[1]*0+tmp[2]*0+tmp[3]*0.015;
   if(as.size()==5)
    as[i].s=tmp[0]*0+tmp[1]*0+tmp[2]*0+tmp[3]*0.0135;
   as[i].x=as[i].x/as[i].s;
   as[i].y=as[i].y/as[i].s;
   as[i].z=as[i].z/as[i].s;
  }
  set_rob(as.size());
}

void figure::twist(int n){
  double tmp[3];
  std::vector<std::vector<double> > tm(V);
  for(unsigned i=0; i<as.size(); ++i){
   tmp[0]=as[i].x;
   tmp[1]=as[i].y;
   tmp[2]=as[i].z;
   if(n==0){
    as[i].x=tmp[0]*1+tmp[1]*0+tmp[2]*0;
    as[i].y=tmp[0]*0+tmp[1]*cos(3.14/180)+tmp[2]*sin(3.14/180);
    as[i].z=tmp[0]*0+tmp[1]*(-sin(3.14/180))+tmp[2]*cos(3.14/180);
   }
   else if(n==1){
	as[i].x=tmp[0]*cos(3.14/180)+tmp[1]*0+tmp[2]*(-sin(3.14/180));
    as[i].y=tmp[0]*0+tmp[1]*1+tmp[2]*0;
    as[i].z=tmp[0]*sin(3.14/180)+tmp[1]*0+tmp[2]*cos(3.14/180);
   }
   else if(n==2){
    as[i].x=tmp[0]*cos(3.14/180)+tmp[1]*sin(3.14/180)+tmp[2]*0;
    as[i].y=tmp[0]*(-sin(3.14/180))+tmp[1]*cos(3.14/180)+tmp[2]*0;
    as[i].z=tmp[0]*0+tmp[1]*0+tmp[2]*1;
   }
  }
  if(as.size()==8){
   if(n==0)
    for(int j=0; j<6; ++j){
	 V[j][0]=tm[j][0]*1+tm[j][1]*0+tm[j][2]*0;
	 V[j][1]=tm[j][0]*0+tm[j][1]*cos(3.14/180)+tm[j][2]*sin(3.14/180);
	 V[j][2]=tm[j][0]*0+tm[j][1]*(-sin(3.14/180))+tm[j][2]*cos(3.14/180);
    }
   if(n==1)
    for(int j=0; j<6; ++j){
     V[j][0]=tm[j][0]*cos(3.14/180)+tm[j][1]*0+tm[j][2]*(-sin(3.14/180));
	 V[j][1]=tm[j][0]*0+tm[j][1]*1+tm[j][2]*0;
	 V[j][2]=tm[j][0]*sin(3.14/180)+tm[j][1]*0+tm[j][2]*cos(3.14/180);
    }
   if(n==2)
    for(int j=0; j<6; ++j){
	 V[j][0]=tm[j][0]*cos(3.14/180)+tm[j][1]*sin(3.14/180)+tm[j][2]*0;
	 V[j][1]=tm[j][0]*(-sin(3.14/180))+tm[j][1]*cos(3.14/180)+tm[j][2]*0;
	 V[j][2]=tm[j][0]*0+tm[j][1]*0+tm[j][2]*1;
    }
   for(int j=0; j<6; ++j)
     EV[j]=V[j][2];
  }
  if(as.size()==5){
   if(n==0)
    for(int j=0; j<5; ++j){
	 V[j][0]=tm[j][0]*1+tm[j][1]*0+tm[j][2]*0;
	 V[j][1]=tm[j][0]*0+tm[j][1]*cos(3.14/180)+tm[j][2]*sin(3.14/180);
	 V[j][2]=tm[j][0]*0+tm[j][1]*(-sin(3.14/180))+tm[j][2]*cos(3.14/180);
    }
   if(n==1)
    for(int j=0; j<5; ++j){
     V[j][0]=tm[j][0]*cos(3.14/180)+tm[j][1]*0+tm[j][2]*(-sin(3.14/180));
	 V[j][1]=tm[j][0]*0+tm[j][1]*1+tm[j][2]*0;
	 V[j][2]=tm[j][0]*sin(3.14/180)+tm[j][1]*0+tm[j][2]*cos(3.14/180);
    }
   if(n==2)
    for(int j=0; j<5; ++j){
	 V[j][0]=tm[j][0]*cos(3.14/180)+tm[j][1]*sin(3.14/180)+tm[j][2]*0;
	 V[j][1]=tm[j][0]*(-sin(3.14/180))+tm[j][1]*cos(3.14/180)+tm[j][2]*0;
	 V[j][2]=tm[j][0]*0+tm[j][1]*0+tm[j][2]*1;
    }
   for(int j=0; j<5; ++j)
     EV[j]=V[j][2];
  }
}


void figure::distance(int n){
  double tmp[3]; 
  for(unsigned i=0; i<as.size(); ++i){
   tmp[0]=as[i].x;
   tmp[1]=as[i].y;
   tmp[2]=as[i].z;
   if(n==0){
    as[i].x=tmp[0]*1.01+tmp[1]*0+tmp[2]*0;
    as[i].y=tmp[0]*0+tmp[1]*1.01+tmp[2]*0;
    as[i].z=tmp[0]*0+tmp[1]*0+tmp[2]*1.01;
   }
   if(n==1){
    as[i].x=tmp[0]/1.01+tmp[1]*0+tmp[2]*0;
    as[i].y=tmp[0]*0+tmp[1]/1.01+tmp[2]*0;
    as[i].z=tmp[0]*0+tmp[1]*0+tmp[2]/1.01;
   }
  }
}
