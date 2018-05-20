#include "mytable.h"
#include "hashtablelowopen.h"
#include "hashtablelowlist.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
                                        // Реализация функций класса MyTable

// Конструктор
MyTable::MyTable(int bitsKey, int bitsValue)
{
    int factor = 1;
    table_ = new HashTableLowOpen("offsets", "table", "meta", 1048576, 12, bitsKey, bitsValue, 2, &factor);
}

// Конструктор копирования
MyTable::MyTable(const MyTable& tableCopy)
{
    table_ = tableCopy.table_;
}

// Деструктор
MyTable::~MyTable()
{
    table_->~HashTableLow();
}

// Запись
bool MyTable::write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue)
{
    table_->addLow(ptrKey, ptrValue, static_cast<std::size_t>(bitsKey), static_cast<std::size_t>(bitsValue));
    return true;
}

// Чтение
bool MyTable::read(char* ptrKey, int bitsKey)
{
    table_->getLow(ptrKey, static_cast<std::size_t>(bitsKey));
    return true;
}

// Получение рандомного ключа
char* MyTable::getRandomKey(int bitsKey, int& bitsKeyReal)
{
    int size = static_cast<int>(table_->sizeLow()), index = rand() % size;
    char* ptrKey = new char[bitsKey];
    char* ptrKeyTmp = (*table_)[index].first;

    std::copy(ptrKeyTmp, ptrKeyTmp + bitsKey, ptrKey);

    int i;
    bitsKeyReal = bitsKey;
    for (i = 0; i < bitsKey; ++i)
        if (ptrKey[i] == '\0') {
            bitsKeyReal = i;
            break;
        }

    return ptrKey;
}
