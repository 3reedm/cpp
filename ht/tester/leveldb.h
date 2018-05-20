#ifndef LEVELDB_H
#define LEVELDB_H
#include "store.h"
#include "leveldb/db.h"
#include <string>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LevelDB : public Store
{
public:
    LevelDB();                                                            // Конструктор по умолчанию
    LevelDB(const LevelDB& tableCopy);                                    // Конструктор копирования
    ~LevelDB();                                                           // Деструктор
    bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue); // Запись данных
    bool read(char* ptrKey, int bitsKey);                                 // Чтение данных
    char* getRandomKey(int bitsKey, int& bitsKeyReal);                    // Получение рандомного ключа
private:
    leveldb::DB* table_;                                                  // Хеш-таблица
    std::string path_;                                                    // Путь к базе данныхz
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // LEVELDB_H
