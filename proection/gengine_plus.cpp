#include "gengine_plus.h"

void gengine_plus::draw(figure one){
  Graphics g(hdc);
  Pen *p;
  g.SetSmoothingMode(SmoothingModeHighQuality);


  int sz=one.get_size();
  if(sz==8)
   p=new Pen(Color(50,50,100), 2);
  if(sz==5)
   p=new Pen(Color(50,100,50), 2);
  apex* tmp=new apex[sz];
  PointF* pf=new PointF[sz];
  tmp=one.get_val();
  double* sign;
  if(sz==8){
   sign=new double[6];
   sign=one.get_EV();
  }
  if(sz==5){
   sign=new double[5];
   sign=one.get_EV();
  }
 
  for(int i=0; i<sz; ++i){
   if(sz==8){
	pf[i].X=(REAL)tmp[i].x+2*Ox/3;
    pf[i].Y=(REAL)tmp[i].y+Oy;
   }
   if(sz==5){
	pf[i].X=(REAL)tmp[i].x+4*Ox/3;
    pf[i].Y=(REAL)tmp[i].y+Oy;
   }
  }

  if(sz==8){
   for(int i=0; i<6; ++i)
	 if(sign[0]>=0){
	  if(sign[2]<0)
       g.DrawLine(p, pf[6], pf[7]);
	  if(sign[3]<0)
       g.DrawLine(p, pf[4], pf[5]);
	  if(sign[5]<0)
       g.DrawLine(p, pf[4], pf[6]);
	  if(sign[4]<0)
       g.DrawLine(p, pf[5], pf[7]);
	 }

	 if(sign[1]>=0){
	  if(sign[3]<0)
       g.DrawLine(p, pf[0], pf[1]);
	  if(sign[2]<0)
       g.DrawLine(p, pf[2], pf[3]);
	  if(sign[5]<0)
       g.DrawLine(p, pf[0], pf[2]);
	  if(sign[4]<0)
       g.DrawLine(p, pf[1], pf[3]);
	 }

	 if(sign[2]>=0){
	  if(sign[5]<0)
       g.DrawLine(p, pf[2], pf[6]);
	  if(sign[4]<0)
       g.DrawLine(p, pf[3], pf[7]);
     //if(sign[1]<=0)
	   g.DrawLine(p, pf[2], pf[3]);
     //if(sign[0]<=0)
	   g.DrawLine(p, pf[6], pf[7]);
	 }

	 if(sign[3]>=0){
	  if(sign[5]<0)
       g.DrawLine(p, pf[0], pf[4]);
      if(sign[4]<0)
	   g.DrawLine(p, pf[1], pf[5]);
	  //if(sign[1]>=0)
       g.DrawLine(p, pf[0], pf[1]);
	  //if(sign[0]>=0)
       g.DrawLine(p, pf[4], pf[5]);
	 }

	 if(sign[4]>=0){
	  //if(sign[2]<=0)
       g.DrawLine(p, pf[3], pf[7]);
	  //if(sign[3]<=0)
       g.DrawLine(p, pf[1], pf[5]);
	  //if(sign[1]<=0)
       g.DrawLine(p, pf[3], pf[1]);
	  //if(sign[0]<=0)
       g.DrawLine(p, pf[7], pf[5]);
	 }

	 if(sign[5]>=0){
	  //if(sign[3]<=0)
       g.DrawLine(p, pf[0], pf[4]);
	  //if(sign[2]<=0)
       g.DrawLine(p, pf[2], pf[6]);
      //if(sign[1]<=0)
	   g.DrawLine(p, pf[0], pf[2]);
	  //if(sign[0]<=0)
       g.DrawLine(p, pf[4], pf[6]);
	 }
  }
  if(sz==5){
   for(int i=0; i<5; ++i){
    if(sign[0]>=0){
	 g.DrawLine(p, pf[0], pf[1]);
     g.DrawLine(p, pf[2], pf[3]);
	 g.DrawLine(p, pf[0], pf[2]);
	 g.DrawLine(p, pf[1], pf[3]);
	}

	if(sign[1]>=0){
	 if(sign[0]<=0)
      g.DrawLine(p, pf[0], pf[1]);
	 g.DrawLine(p, pf[1], pf[4]);
	 g.DrawLine(p, pf[0], pf[4]);
	}

	if(sign[2]>=0){
	 if(sign[1]<=0)
	  g.DrawLine(p, pf[0], pf[4]);
	 if(sign[0]<=0)
	  g.DrawLine(p, pf[0], pf[2]);
	 g.DrawLine(p, pf[2], pf[4]);
	}

	if(sign[3]>=0){
	 if(sign[2]<=0)
      g.DrawLine(p, pf[2], pf[4]);
	 if(sign[0]<=0)
	  g.DrawLine(p, pf[3], pf[2]);
	 g.DrawLine(p, pf[3], pf[4]);
	}

	if(sign[4]>=0){
	 if(sign[1]<=0)
	  g.DrawLine(p, pf[1], pf[4]);
     if(sign[3]<=0)
      g.DrawLine(p, pf[3], pf[4]);
	 if(sign[0]<=0)
	  g.DrawLine(p, pf[1], pf[3]);
	}
   }
  }
}
