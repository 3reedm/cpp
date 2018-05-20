#include "leveldb.h"
#include <cstdlib>
#include <algorithm>
                                        // Реализация функций класса LevelDB

// Конструктор
LevelDB::LevelDB()
{
    leveldb::Options options;
    path_ = "tabledb";
    options.create_if_missing = true;
    leveldb::DB::Open(options, path_, &table_);
}

// Конструктор копирования
LevelDB::LevelDB(const LevelDB& tableCopy)
{
    path_ = tableCopy.path_;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::DB::Open(options, path_, &table_);
}

// Деструктор
LevelDB::~LevelDB()
{
    delete table_;
}

// Запись
bool LevelDB::write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue)
{
    leveldb::Slice key(ptrKey, bitsKey), value(ptrValue, bitsValue);
    table_->Put(leveldb::WriteOptions(), key, value);
    key.clear();
    value.clear();
    return true;
}

// Чтение
bool LevelDB::read(char* ptrKey, int bitsKey)
{
    leveldb::Slice key(ptrKey, bitsKey);
	std::string value;
    table_->Get(leveldb::ReadOptions(), key, &value);
    key.clear();
    return true;
}

// Получение рандомного ключа
char* LevelDB::getRandomKey(int bitsKey, int& bitsKeyReal)
{
    char* ptrKey = new char[bitsKey];
	std::string key;
    leveldb::Iterator* it = table_->NewIterator(leveldb::ReadOptions());
    int s = rand() % 100000, j;
    for (it->SeekToFirst(), j = 0; it->Valid(); it->Next(), ++j)
        if (j == s) {
			key = it->key().ToString();
            break;
        }
    std::fill(ptrKey, ptrKey + bitsKey, '\0');
    std::copy(key.begin(), key.begin() + key.size(), ptrKey);
    delete it;

    int i;
    bitsKeyReal = bitsKey;
    for (i = 0; i < bitsKey; ++i)
        if (ptrKey[i] == '\0') {
            bitsKeyReal = i;
            break;
        }

    return ptrKey;
}
