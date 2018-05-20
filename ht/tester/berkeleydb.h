#ifndef BERKELEYDB_H
#define BERKELEYDB_H
#include "store.h"
#include <db_cxx.h>
#include <string>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BerkeleyDB : public Store
{
public:
    BerkeleyDB();                                                         // Конструктор по умолчанию
    BerkeleyDB(const BerkeleyDB& tableCopy);                              // Конструктор копирования
    ~BerkeleyDB();                                                        // Деструктор
    bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue); // Запись данных
    bool read(char* ptrKey, int bitsKey);                                 // Чтение данных
    char* getRandomKey(int bitsKey, int& bitsKeyReal);                    // Получение рандомного ключа
private:
    Db table_;                                                            // Хеш-таблица
    std::string fileName_;                                                // Имя файла
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // BERKELEYDB_H
