#ifndef HASHTABLEHIGH_H
#define HASHTABLEHIGH_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "hashtablelow.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class K = char*, class V = char*>
class HashTableHigh
{
public:

    // Основные функции
    HashTableHigh(){}                                                                     // Конструктор по умолчанию
    HashTableHigh(std::string fileNameOffsetsInput, std::string fileNameHashTableInput,   // Конструктор для загрузки хеш-таблицы из файла, с помощью файла отступов
                  std::string fileNameInfoInput,                                          // Имя файла с метаинформацией
                  int addressingInput = 0,                                                // Тип адресации
                  int lengthBucketsInput = 16384,                                         // Размер массива для таблицы
                  int bitsOffsetInput = 8,                                                // Память, выделяемая для одного отступа
                  int bitsKeyInput = 16, int bitsValueInput = 32,                         // Память, выделяемая для одного ключа\значения
                  int *optionalParameters = reinterpret_cast<int*>(0));                   // Средний размер предполагаемого блока списка или стратегия и коэффициенты
    HashTableHigh(const HashTableHigh& tableCopy);                                        // Конструктор копирования
    ~HashTableHigh();                                                                     // Деструктор

    std::pair<K, V>& operator [](const std::size_t& index);                               // Оператор произвольного доступа
    bool add(K key, V value, int bitsKey, int bitsValue);                                 // Добавление нового элемента
    bool contains(K key, int bitsKey);                                                    // Проверка, через ключ, на присутствие элемента в хеш-таблице
    bool remove(K key, int bitsKey);                                                      // Удаление элемента по ключу из хеш-таблицы
    bool resize(int* newLength);                                                          // Перестройка таблицы, если она переполнена
    std::pair<K, V> get(K key, int bitsKey);                                              // Получение элемента хеш-таблицы с помощью ключа
    std::size_t size();                                                                   // Размер хеш-таблицы

    // Класс итератора
    class Iterator : public HashTableLow::Iterator                                        // Класс итератора для хеш-таблицы
    {
    public:
        // Структура iterator_traits
        typedef HashTableHigh value_type;
        typedef std::pair<K, V>& reference;
        typedef std::pair<K, V>* pointer;

        // Функции
        Iterator(){}                                                   // Конструктор по умолчанию
        Iterator(value_type* ptrCurrent);                              // Конструктор через хеш-таблицу
        Iterator(const Iterator& iteratorCoppy);                       // Конструктор копирования
        ~Iterator(){}                                                  // Деструктор
        reference operator *();                                        // Оператор разыменования 1
        pointer operator ->();                                         // Оператор разыменования 2
        reference operator [](const std::size_t& index);               // Оператор произвольного доступа
    private:
        HashTableHigh* ptrCurrent_;                                    // Хранимый объект
    };
private:
    int addressing_;                                                                      // Адресация
    std::pair<K, V> resultHigh_;                                                          // Контейнер для хранения элемента
    HashTableHigh::Iterator iteratorHigh_;                                                // Итератор
    HashTableLow* hashTableLow_;                                                          // Класс HashTableLow
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // HASHTABLEHIGH_H
