#ifndef HAMSTERDB_H
#define HAMSTERDB_H
#include "store.h"
#include <ham/hamsterdb.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hamsterdb : public Store
{
public:
    Hamsterdb();                                                          // Конструктор по умолчанию
    Hamsterdb(const Hamsterdb& tableCopy);                                // Конструктор копирования
    ~Hamsterdb();                                                         // Деструктор
    bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue); // Запись данных
    bool read(char* ptrKey, int bitsKey);                                 // Чтение данных
    char* getRandomKey(int bitsKey, int& bitsKeyReal);                    // Получение рандомного ключа
private:
    ham_db_t* table_;                                                     // Хеш-таблица
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HAMSTERDB_H
