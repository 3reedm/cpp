#ifndef HASHTABLELOWOPEN_H
#define HASHTABLELOWOPEN_H
#include "hashtablelow.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HashTableLowOpen : public HashTableLow
{
public:

    // Основные функции
    HashTableLowOpen(){}                                                                     // Конструктор по умолчанию
    HashTableLowOpen(std::string fileNameOffsetsInput, std::string fileNameHashTableInput,   // Конструктор для загрузки хеш-таблицы из файла, с помощью файла отступов
                     std::string fileNameInfoInput,                                          // Имя файла с метаинформацией
                     int lengthBucketsInput,                                                 // Размер массива для таблицы
                     int bitsOffsetInput, int bitsKeyInput, int bitsValueInput,              // Память, выделяемая для одного отступа\ключа\значения
                     int searchStrategyInput, int* factorsInput);                            // Стратегия поиска, коэффициенты (для линейного и квадратичного методов)
    HashTableLowOpen(const HashTableLowOpen& tableCopy);                                     // Конструктор копирования
    ~HashTableLowOpen();                                                                     // Деструктор

    bool addLow(char* ptrKey, char* ptrValue, std::size_t bitsKey, std::size_t bitsValue);   // Добавление нового элемента
    bool containsLow(char* ptrKey, std::size_t bitsKey);                                     // Проверка, через ключ, на присутствие элемента в хеш-таблице
    std::pair<char*, char*> getLow(char* ptrKey, std::size_t bitsKey);                       // Получение элемента хеш-таблицы с помощью ключа
    bool removeLow(char* ptrKey, std::size_t bitsKey);                                       // Удаление элемента по ключу из хеш-таблицы
    bool resizeLow(std::size_t* newLength);                                                  // Перестройка таблицы, если она переполнена
    std::size_t sizeLow();                                                                   // Размер хеш-таблицы

private:

    // Вспомогательные функции
    std::pair<char*, char*>* getElementLow(const std::size_t& index);                        // Получение элемента по индексу
    std::size_t getIndex(const std::size_t& index);                                          // Получение нужного индекса
    bool getStatus(char* ptrKey, std::size_t bitsKey, std::size_t& index);                   // Функция получения статуса присутствия элемента в хеш-таблице, если элемент есть, то его номер

    // Атрибуты
    int searchStrategy_;                                                                     // Стратегия поиска
    std::size_t* factors_;                                                                   // Дополнительные коэффициенты: стратегия линейная (0) - шаг; квадратичная (1) - 2 коэффициента
    std::vector<char*> offsets_;                                                             // Массив отступов (для нахождения конкретного элемента в файле хеш-таблицы)
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HASHTABLELOWOPEN_H
