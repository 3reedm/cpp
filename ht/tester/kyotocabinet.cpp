#include "kyotocabinet.h"
#include <algorithm>

                                        // Реализация функций класса KyotoCabinet

// Конструктор
KyotoCabinet::KyotoCabinet()
{
    fileName_ = "table.kch";
    table_.open(fileName_, kyotocabinet::HashDB::OWRITER | kyotocabinet::HashDB::OREADER | kyotocabinet::HashDB::OCREATE);
}

// Конструктор копирования
KyotoCabinet::KyotoCabinet(const KyotoCabinet& tableCopy)
{
    fileName_ = tableCopy.fileName_;
    table_.open(fileName_, kyotocabinet::HashDB::OWRITER | kyotocabinet::HashDB::OREADER);
}

// Деструктор
KyotoCabinet::~KyotoCabinet()
{
    table_.close();
}

// Запись
bool KyotoCabinet::write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue)
{
    table_.add(ptrKey, static_cast<std::size_t>(bitsKey), ptrValue, static_cast<std::size_t>(bitsValue));
    return true;
}

// Чтение
bool KyotoCabinet::read(char* ptrKey, int bitsKey)
{
    std::string value;
    table_.get(ptrKey, &value);
    return true;
}

// Получение рандомного ключа
char* KyotoCabinet::getRandomKey(int bitsKey, int& bitsKeyReal)
{
    char* ptrKey = new char[bitsKey];
    std::size_t bitsKeyTmp = static_cast<std::size_t>(bitsKey);
    kyotocabinet::HashDB::Cursor cursor(&table_);
    int s = rand() % 100000, j;
    for (j = 0; j < s; ++j)
        cursor.step();
    std::fill(ptrKey, ptrKey + bitsKey, '\0');
    std::copy(cursor.get_key(&bitsKeyTmp), cursor.get_key(&bitsKeyTmp) + bitsKeyTmp, ptrKey);
	cursor.~Cursor();

    int i;
    bitsKeyReal = bitsKey;
    for (i = 0; i < bitsKey; ++i)
        if (ptrKey[i] == '\0') {
            bitsKeyReal = i;
            break;
        }

    return ptrKey;
}
