#include "berkeleydb.h"
#include <cstdlib>

                                        // Реализация функций класса BerkeleyDB
// Конструктор
BerkeleyDB::BerkeleyDB() : table_(NULL, 0), fileName_("table.db")
{
    table_.open(NULL, fileName_.c_str(), NULL, DB_HASH, DB_CREATE, 0);
}

// Конструктор копирования
BerkeleyDB::BerkeleyDB(const BerkeleyDB& tableCopy) : table_(NULL, 0), fileName_(tableCopy.fileName_)
{
    table_.open(NULL, fileName_.c_str(), NULL, DB_HASH, 0, 0);
}

// Деструктор
BerkeleyDB::~BerkeleyDB()
{
    table_.close(0);
}

// Запись
bool BerkeleyDB::write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue)
{
    Dbt key(ptrKey, bitsKey), value(ptrValue, bitsValue + 1);
    table_.put(NULL, &key, &value, DB_OVERWRITE_DUP);
    return true;
}

// Чтение
bool BerkeleyDB::read(char* ptrKey, int bitsKey)
{
    char* ptrValue = new char[301];
    Dbt key(ptrKey, bitsKey), value(ptrValue, 301);
    value.set_flags(DB_DBT_USERMEM);
    table_.get(NULL, &key, &value, 0);
    delete[] ptrValue;
    return true;
}

// Получение рандомного ключа
char* BerkeleyDB::getRandomKey(int bitsKey, int& bitsKeyReal)
{
    Dbc* cursorp;
    char* ptrKey = new char[bitsKey];
    Dbt key(ptrKey, bitsKey), value;

    int i;
    table_.cursor(NULL, &cursorp, 0);
    for (i = 0; i < rand() % 100000; ++i)
        cursorp->get(&key, &value, DB_NEXT);
    cursorp->close();

    std::copy(static_cast<char*>(key.get_data()), static_cast<char*>(key.get_data()) + static_cast<int>(key.get_size()) - 1, ptrKey);
    bitsKeyReal = bitsKey;
    for (i = 0; i < bitsKey; ++i)
        if (ptrKey[i] == '\0') {
            bitsKeyReal = i;
            break;
        }

    return ptrKey;
}
