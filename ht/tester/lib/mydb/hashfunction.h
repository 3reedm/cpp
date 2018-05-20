#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H
////////////////////////////////////////////////////////////////////////////////////////////////
struct HashFunction
{
    virtual ~HashFunction(){}                                             // Деструктор
    virtual unsigned int getHashCode(char* key, unsigned int length) = 0; // Получение хеш-кода
    virtual HashFunction* clone() = 0;                                    // Клонирование
};
////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HASHFUNCTION_H
