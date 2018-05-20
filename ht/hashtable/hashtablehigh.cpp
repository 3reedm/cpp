#include "hashtablehigh.h"
#include "hashtablelowopen.h"
#include "hashtablelowlist.h"
#include <string>

                                           // Реализация функций класса HashTableHigh

// Основные функции

// Конструктор для загрузки хеш-таблицы из файла, с помощью файла отступов
template<class K, class V>
HashTableHigh<K, V>::HashTableHigh(std::string fileNameOffsetsInput, std::string fileNameHashTableInput, std::string fileNameInfoInput, int addressingInput, int lengthBucketsInput, int bitsOffsetInput, int bitsKeyInput, int bitsValueInput, int* optionalParameters)
{
    addressing_ = addressingInput;
    if (addressing_ == 0) {
        if (optionalParameters != reinterpret_cast<int*>(0)) {
            int* factors = new int[2];
            factors[0] = optionalParameters[1];
            factors[1] = optionalParameters[2];
            hashTableLow_ = new HashTableLowOpen(fileNameOffsetsInput, fileNameHashTableInput, fileNameInfoInput, lengthBucketsInput, bitsOffsetInput, bitsKeyInput, bitsValueInput, optionalParameters[0], factors);
            delete[] factors;
        }
        else {
            int factors = 1;
            hashTableLow_ = new HashTableLowOpen(fileNameOffsetsInput, fileNameHashTableInput, fileNameInfoInput, lengthBucketsInput, bitsOffsetInput, bitsKeyInput, bitsValueInput, 0, &factors);
        }
    }
    else if (addressing_ == 1) {
        if (optionalParameters != reinterpret_cast<int*>(0))
            hashTableLow_ = new HashTableLowList(fileNameOffsetsInput, fileNameHashTableInput, fileNameInfoInput, lengthBucketsInput, bitsOffsetInput, bitsKeyInput, bitsValueInput, *optionalParameters);
        else
            hashTableLow_ = new HashTableLowList(fileNameOffsetsInput, fileNameHashTableInput, fileNameInfoInput, lengthBucketsInput, bitsOffsetInput, bitsKeyInput, bitsValueInput, 10);
    }
    else
        throw "Нет такой адресации";

    resultHigh_ = hashTableLow_->resultLow_;
    iteratorHigh_ = HashTableHigh::Iterator(this);
}

// Деструктор
template<class K, class V>
HashTableHigh<K, V>::~HashTableHigh()
{
    delete hashTableLow_;
}

// Конструктор копирования
template<class K, class V>
HashTableHigh<K, V>::HashTableHigh(const HashTableHigh& tableCopy)
{
    addressing_ = tableCopy.addressing_;

    if (addressing_ == 0)
        hashTableLow_ = new HashTableLowOpen(tableCopy.hashTableLow_);
    else
        hashTableLow_ = new HashTableLowList(tableCopy.hashTableLow_);
    resultHigh_ = tableCopy.resultHigh_;
    iteratorHigh_ = tableCopy.iteratorHigh_;
}

// Оператор произвольного доступа
template<class K, class V>
std::pair<K, V>& HashTableHigh<K, V>::operator [](const std::size_t& index)
{
    return iteratorHigh_[index];
}

// Добавление нового элемента
template<class K, class V>
bool HashTableHigh<K, V>::add(K key, V value, int bitsKey, int bitsValue)
{
    bool result = hashTableLow_->addLow(key, value, static_cast<std::size_t>(bitsKey), static_cast<std::size_t>(bitsValue));

    return result;
}

// Проверка, через ключ, на присутствие элемента в хеш-таблице
template<class K, class V>
bool HashTableHigh<K, V>::contains(K key, int bitsKey)
{
    bool result = hashTableLow_->containsLow(key, static_cast<std::size_t>(bitsKey));

    return result;
}

// Удаление элемента по ключу из хеш-таблицы
template<class K, class V>
bool HashTableHigh<K, V>::remove(K key, int bitsKey)
{
    bool result = hashTableLow_->removeLow(key, static_cast<std::size_t>(bitsKey));

    return result;
}

// Перестройка таблицы
template<class K, class V>
bool HashTableHigh<K, V>::resize(int* newLength)
{
    bool result = hashTableLow_->resizeLow(static_cast<std::size_t*>(newLength));

    return result;
}

// Получение элемента хеш-таблицы с помощью ключа
template<class K, class V>
std::pair<K, V> HashTableHigh<K, V>::get(K key, int bitsKey)
{
    return hashTableLow_->getLow(key, static_cast<std::size_t>(bitsKey));
}

// Получить размер таблицы
template<class K, class V>
std::size_t HashTableHigh<K, V>::size()
{
    std::size_t sizeTable = hashTableLow_->sizeLow();

    return sizeTable;
}

                                           // Реализация функций класса HashTableHigh::Iterator

// Конструктор через хеш-таблицу
template<class K, class V>
HashTableHigh<K, V>::Iterator::Iterator(HashTableHigh<K, V>* ptrCurrent)
{
    ptrCurrent_ = ptrCurrent;
}

// Конструктор копирования
template<class K, class V>
HashTableHigh<K, V>::Iterator::Iterator(const HashTableHigh<K, V>::Iterator& iteratorCoppy)
{
    ptrCurrent_ = iteratorCoppy.ptrCurrent_;
    this->index_ = iteratorCoppy.index_;
}

// Оператор разыменования 1
template<class K, class V>
typename HashTableHigh<K, V>::Iterator::reference HashTableHigh<K, V>::Iterator::operator *()
{
    std::size_t index = static_cast<std::size_t>(this->index_);

    return (*ptrCurrent_).hashTableLow_->getElement(index);
}

// Оператор разыменования 2
template<class K, class V>
typename HashTableHigh<K, V>::Iterator::pointer HashTableHigh<K, V>::Iterator::operator ->()
{
    std::size_t index = static_cast<std::size_t>(this->index_);

    return (ptrCurrent_->hashTableLow_)->getElement(index);
}

// Оператор произвольного доступа
template<class K, class V>
typename HashTableHigh<K, V>::Iterator::reference HashTableHigh<K, V>::Iterator::operator [](const std::size_t& index)
{
    std::size_t number = ptrCurrent_->getIndex(index);

    return (*ptrCurrent_).hashTableLow_->getElement(number);
}
