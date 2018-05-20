///////////////definition of class apex//////////////////
#ifndef APEX_H                                         //
#define APEX_H                                         //
/////////////////////////////////////////////////////////
class apex{                                            //
public:                                                //
 apex(double x1=0,double y1=0);                        //
 apex(apex&);                                          //
 void move(apex&);                                     //
 friend std::ostream operator <<(std::ostream, apex&); //
protected:                                             //
 double x,y;                                           //
};                                                     //
/////////////////////////////////////////////////////////
#endif /*APEX_H*/                                      //
/////////////////////////////////////////////////////////
