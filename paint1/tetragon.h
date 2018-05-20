//////////definition of class tetragon/////////
#ifndef TETRAGON_H                           //
#define TETRAGON_H                           //
#include <string>                            //
#include "figure.h"                          //
///////////////////////////////////////////////
class tetragon: public figure{               //
public:                                      //
 tetragon(std::string name1): name(name1){}  ////////////////////////
 tetragon(std::string name1, apex* val1): name(name1), val(val1){} //
 const std::string& GetName()const{return name;}                   //
 apex* GetVal()const{return val;}            ////////////////////////
 void move(apex&);                           //
 void print()const;                          //
protected:                                   //
 std::string name;                           //
 apex *val;                                  //
};                                           //
///////////////////////////////////////////////
#endif /*TETRAGON_H*/                        //
///////////////////////////////////////////////
