#ifndef HASHTABLELOWLIST_H
#define HASHTABLELOWLIST_H
#include "hashtablelow.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HashTableLowList : public HashTableLow
{
public:

    // Основные функции
    HashTableLowList(){}                                                                     // Конструктор по умолчанию
    HashTableLowList(std::string fileNameOffsetsInput, std::string fileNameHashTableInput,   // Конструктор для загрузки хеш-таблицы из файла, с помощью файла отступов
                     std::string fileNameInfoInput,                                          // Имя файла с метаинформацией
                     int lengthBucketsInput,                                                 // Размер массива для таблицы
                     int bitsOffsetInput, int bitsKeyInput, int bitsValueInput,              // Память, выделяемая для одного отступа\ключа\значения
                     int sizeBlockInput);                                                    // Предполагаемый размер одного блока списка
    HashTableLowList(const HashTableLowList& tableCopy);                                     // Конструктор копирования
    ~HashTableLowList();                                                                     // Деструктор

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
    bool getStatus(char* ptrKey, std::size_t bitsKey,                                        // Функция получения статуса присутствия элемента в хеш-таблице,
                   std::size_t& index, std::size_t& indexList);                              // если элемент есть, то его номер и номер списка, в котором он находится

    // Атрибуты
    std::size_t sizeBlock_;                                                                  // Средний предполагаемый размер одного блока списка
    std::size_t nLinks_;                                                                     // Число использованных дополнительных списков
    std::vector<bool> offsetsFlag_;                                                          // Индикатор доступности дополнительного списка
    std::vector<std::list<char*> > offsets_;                                                 // Массив отступов (для нахождения конкретного элемента в файле хеш-таблицы)
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HASHTABLELOWLIST_H
