#ifndef MYTABLE_H
#define MYTABLE_H
#include "store.h"
#include "hashtablelow.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MyTable : public Store
{
public:
    MyTable(){}                                                           // Конструктор по умолчанию
    MyTable(int bitsKey, int bitsValue);                                  // Конструктор на основе данных
    MyTable(const MyTable& tableCopy);                                    // Конструктор копирования
    ~MyTable();                                                           // Деструктор
    bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue); // Запись данных
    bool read(char* ptrKey, int bitsKey);                                 // Чтение данных
    char* getRandomKey(int bitsKey, int& bitsKeyReal);                    // Получение рандомного ключа
private:
    HashTableLow* table_;                                                 // Хеш-таблица
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // MYTABLE_H
