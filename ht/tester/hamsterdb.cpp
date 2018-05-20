#include "hamsterdb.h"
#include <cstdlib>
#include <algorithm>
#include <cstring>

                                        // Реализация функций класса Hamsterdb

// Конструктор по умолчанию
Hamsterdb::Hamsterdb()
{
    ham_env_t* env;
    ham_env_create(&env, "htable.db", 0, 0664, 0);
    ham_env_create_db(env, &table_, 1, 0, 0);
}

// Конструктор копирования
Hamsterdb::Hamsterdb(const Hamsterdb& tableCopy)
{
    table_ = tableCopy.table_;
}

// Деструктор
Hamsterdb::~Hamsterdb()
{
    ham_db_close(table_, HAM_AUTO_CLEANUP);
}

// Запись
bool Hamsterdb::write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue)
{
    ham_key_t  key;
    ham_record_t value;
    memset(&key, 0, sizeof(key));
    memset(&value, 0, sizeof(value));

    key.data = ptrKey;
    key.size = bitsKey + 1;
    value.data = ptrValue;
    value.size = bitsValue + 1;

    ham_db_insert(table_, NULL, &key, &value, 0);
    return true;
}

// Чтение
bool Hamsterdb::read(char* ptrKey, int bitsKey)
{
    ham_key_t  key;
    ham_record_t value;
    memset(&key, 0, sizeof(key));
    memset(&value, 0, sizeof(value));

    key.data = ptrKey;
    key.size = bitsKey + 1;

    ham_db_find(table_, NULL, &key, &value, 0);
    return true;
}

// Получение рандомного ключа
char* Hamsterdb::getRandomKey(int bitsKey, int& bitsKeyReal)
{
    char* ptrKey = new char[bitsKey];
    ham_key_t  key;
    ham_record_t value;
    memset(&key, 0, sizeof(key));
    memset(&value, 0, sizeof(value));
    ham_cursor_t* cursor;

	ham_cursor_create(&cursor, table_, 0, 0);
    int j, s = rand() % 100000;
    for (j = 0; j < s; ++j)
        ham_cursor_move(cursor, &key, &value, HAM_CURSOR_NEXT);
    std::fill(ptrKey, ptrKey + bitsKey, '\0');
    std::copy(static_cast<char*>(key.data), static_cast<char*>(key.data) + key.size, ptrKey);
    ham_cursor_close(cursor);

    int i;
    bitsKeyReal = bitsKey;
    for (i = 0; i < bitsKey; ++i)
        if (ptrKey[i] == '\0') {
            bitsKeyReal = i;
            break;
        }

    return ptrKey;
}
