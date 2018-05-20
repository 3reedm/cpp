#include "defaulthashfunction.h"

                                           // Реализация функций структуры DefaultHashFunction

// Получение хеш-кода, алгоритм RS
unsigned int DefaultHashFunction::getHashCode(char* key, unsigned int length)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    unsigned int i;

    for (i = 0; i < length; ++key, ++i) {
        hash = hash * a + static_cast<unsigned char>(*key);
        a = a * b;
    }

    return hash;
}

// Клонирование
HashFunction* DefaultHashFunction::clone()
{
    HashFunction* clone;
    clone = new DefaultHashFunction;
    return clone;
}
