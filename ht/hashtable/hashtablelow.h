#ifndef HASHTABLELOW_H
#define HASHTABLELOW_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <typeinfo>
#include "hashfunction.h"
#include "defaulthashfunction.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HashTableLow
{
public:

    // Основные функции
    HashTableLow(){}                                                                                   // Конструктор по умолчанию
    HashTableLow(const HashTableLow& tableCopy);                                                       // Конструктор копирования
    virtual ~HashTableLow();                                                                           // Деструктор
    std::pair<char*, char*>& operator [](const std::size_t& index);                                    // Оператор произвольного доступа
    virtual bool addLow(char* ptrKey, char* ptrValue, std::size_t bitsKey, std::size_t bitsValue) = 0; // Добавление нового элемента
    virtual bool containsLow(char* ptrKey, std::size_t bitsKey) = 0;                                   // Проверка, через ключ, на присутствие элемента в хеш-таблице
    virtual bool removeLow(char* ptrKey, std::size_t bitsKey) = 0;                                     // Удаление элемента по ключу из хеш-таблицы
    virtual bool resizeLow(std::size_t* newLength) = 0;                                                // Перестройка таблицы, если она переполнена
    virtual std::pair<char*, char*> getLow(char* ptrKey, std::size_t bitsKey) = 0;                     // Получение элемента хеш-таблицы с помощью ключа
    virtual std::size_t sizeLow() = 0;                                                                 // Размер хеш-таблицы

    class Iterator : public std::iterator<std::random_access_iterator_tag, HashTableLow>               // Класс итератора для хеш-таблицы
    {
    public:
        // Структура iterator_traits
        typedef HashTableLow value_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::pair<char*, char*>& reference;
        typedef std::pair<char*, char*>* pointer;
        typedef std::size_t size_type;
        typedef std::random_access_iterator_tag iterator_category;

        // Функции
        Iterator(){}                                                   // Конструктор по умолчанию
        Iterator(value_type* ptrCurrent);                              // Конструктор через хеш-таблицу
        Iterator(const Iterator& iteratorCoppy);                       // Конструктор копирования
        ~Iterator(){}                                                  // Деструктор
        Iterator operator =(const Iterator& iteratorAssign);           // Оператор присваивания
        reference operator *();                                        // Оператор разыменования 1
        pointer operator ->();                                         // Оператор разыменования 2
        reference operator [](const std::size_t& index);               // Оператор произвольного доступа
        Iterator operator ++();                                        // Оператор инкрементирования
        Iterator operator --();                                        // Оператор декрементирования
        Iterator operator +(const size_type& shift);                   // Оператор сдвига на число вправо
        Iterator operator -(const size_type& shift);                   // Оператор сдвига на число влево
        Iterator& operator +=(const size_type& shift);                 // Оператор сдвига на число вправо (сокращённая форма)
        Iterator& operator -=(const size_type& shift);                 // Оператор сдвига на число влево (сокращённая форма)
        difference_type operator -(const Iterator& iteratorSubtract);  // Разница между итераторами с точностью до знака
        bool operator ==(const Iterator& iteratorCompare);             // Оператор эквивалентности
        bool operator !=(const Iterator& iteratorCompare);             // Оператор неэквивалентности
        bool operator >(const Iterator& iteratorCompare);              // Оператор больше
        bool operator <(const Iterator& iteratorCompare);              // Оператор меньше
        bool operator >=(const Iterator& iteratorCompare);             // Оператор больше или равно
        bool operator <=(const Iterator& iteratorCompare);             // Оператор меньше или равно
    protected:
        size_type index_;                                              // Индекс текущего элемента
    private:
        HashTableLow* ptrCurrent_;                                     // Хранимый объект
    };

protected:

    // Вспомогательные функции
    virtual std::pair<char*, char*>* getElementLow(const std::size_t& index) = 0;                       // Получение элемента по индексу
    virtual std::size_t getIndex(const std::size_t& index) = 0;                                         // Получение нужного индекса

    // Атрибуты
    std::size_t nBuckets_, lengthBuckets_;                                                              // Число хранимых элементов в хеш-таблице, общая длина хеш-таблицы
    std::size_t bitsOffset_, bitsKey_, bitsValue_;                                                      // Память для хранения одного отступа\ключа\значения
    std::fstream fileOffsets_;                                                                          // Общий файл для чтения\записи отступов
    std::fstream fileHashTable_;                                                                        // Общий файл для чтения\записи в хеш-таблицу
    std::fstream fileInfo_;                                                                             // Общий файл для чтения\записи метаинформации
    std::string fileNameOffsets_, fileNameHashTable_, fileNameInfo_;                                    // Имена файлов отступа\хеш-таблицы
    std::pair<char*, char*> resultLow_;                                                                 // Контейнер для хранения элемента
    HashTableLow::Iterator iteratorLow_;                                                                // Итератор
    HashFunction* hashFunction_;                                                                        // Хеш-функция
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HASHTABLELOW_H
