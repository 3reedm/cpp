#ifndef DEFAULTHASHFUNCTION_H
#define DEFAULTHASHFUNCTION_H
/////////////////////////////////////////////////////////////////////////////////////
#include "hashfunction.h"
/////////////////////////////////////////////////////////////////////////////////////
struct DefaultHashFunction : public HashFunction
{
    DefaultHashFunction(){}                                   // Конструктор по умолчанию
    ~DefaultHashFunction(){}                                  // Деструктор
    unsigned int getHashCode(char* key, unsigned int length); // Получение хеш-кода
    HashFunction* clone();                                    // Клонирование
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // DEFAULTHASHFUNCTION_H
