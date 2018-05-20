#ifndef KYOTOCABINET_H
#define KYOTOCABINET_H
#include "store.h"
#include <kchashdb.h>
#include <string>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class KyotoCabinet : public Store
{
public:
    KyotoCabinet();                                                       // Конструктор по умолчанию
    KyotoCabinet(const KyotoCabinet& tableCopy);                          // Конструктор копирования
    ~KyotoCabinet();                                                      // Деструктор
    bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue); // Запись данных
    bool read(char* ptrKey, int bitsKey);                                 // Чтение данных
    char* getRandomKey(int bitsKey, int& bitsKeyReal);                    // Получение рандомного ключа
private:
    kyotocabinet::HashDB table_;                                          // Хеш-таблица
    std::string fileName_;                                                // Имя файла базы данных
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // KYOTOCABINET_H
