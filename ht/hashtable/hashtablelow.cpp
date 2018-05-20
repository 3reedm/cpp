#include "hashtablelow.h"
#include <cstdlib>
#include <iostream>
                                           // Реализация функций класса HashTableLow

// Основные функции

// Конструктор копирования
HashTableLow::HashTableLow(const HashTableLow& tableCopy)
{
    lengthBuckets_ = tableCopy.lengthBuckets_;
    nBuckets_ = tableCopy.nBuckets_;
    fileNameHashTable_ = tableCopy.fileNameHashTable_;
    fileNameOffsets_ = tableCopy.fileNameOffsets_;
    fileNameInfo_ = tableCopy.fileNameInfo_;
    fileOffsets_.close();
    fileOffsets_.open(fileNameOffsets_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileHashTable_.close();
    fileHashTable_.open(fileNameHashTable_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileInfo_.close();
    fileInfo_.open(fileNameInfo_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    bitsOffset_ = tableCopy.bitsOffset_;
    bitsValue_ = tableCopy.bitsValue_;
    bitsKey_ = tableCopy.bitsKey_;
    resultLow_.first = new char[bitsKey_ + static_cast<std::size_t>(1)];
    resultLow_.second = new char[bitsValue_ + static_cast<std::size_t>(1)];
    resultLow_.first[bitsKey_] = '\0';
    resultLow_.second[bitsValue_] = '\0';
    std::copy(tableCopy.resultLow_.first, tableCopy.resultLow_.first + bitsKey_, resultLow_.first);
    std::copy(tableCopy.resultLow_.second, tableCopy.resultLow_.second + bitsValue_, resultLow_.second);
    hashFunction_ = tableCopy.hashFunction_->clone();
    iteratorLow_ = tableCopy.iteratorLow_;
}

// Деструктор
HashTableLow::~HashTableLow()
{
    fileHashTable_.close();
    fileOffsets_.close();
    fileInfo_.close();
    hashFunction_->~HashFunction();
}

// Оператор произвольного доступа
std::pair<char*, char*>& HashTableLow::operator [](const std::size_t& index)
{
    return iteratorLow_[index];
}

                                           // Реализация функций класса HashTableLow::Iterator

// Конструктор через хеш-таблицу
HashTableLow::Iterator::Iterator(HashTableLow::Iterator::value_type* ptrCurrent)
{
    ptrCurrent_ = ptrCurrent;
}

// Конструктор копирования
HashTableLow::Iterator::Iterator(const HashTableLow::Iterator& iteratorCoppy)
{
    ptrCurrent_ = iteratorCoppy.ptrCurrent_;
    index_ = iteratorCoppy.index_;
}

// Оператор присваивания
HashTableLow::Iterator HashTableLow::Iterator::operator =(const HashTableLow::Iterator& iteratorAssign)
{
    ptrCurrent_ = iteratorAssign.ptrCurrent_;
    index_ = iteratorAssign.index_;

    return *this;
}

// Оператор разыменования 1
HashTableLow::Iterator::reference HashTableLow::Iterator::operator *()
{
    std::size_t index = ptrCurrent_->getIndex(static_cast<std::size_t>(index_));

    return *ptrCurrent_->getElementLow(index);
}

// Оператор разыменования 2
HashTableLow::Iterator::pointer HashTableLow::Iterator::operator ->()
{
    std::size_t index = ptrCurrent_->getIndex(static_cast<std::size_t>(index_));

    return ptrCurrent_->getElementLow(index);
}

// Оператор произвольного доступа
HashTableLow::Iterator::reference HashTableLow::Iterator::operator [](const std::size_t& index)
{
    std::size_t number = ptrCurrent_->getIndex(index);

    return *ptrCurrent_->getElementLow(number);
}

// Оператор инкрементирования
HashTableLow::Iterator HashTableLow::Iterator::operator ++()
{
    ++index_;

    if (index_ < static_cast<size_type>(ptrCurrent_->nBuckets_))
        return *this;

    else
        throw "Выход за пределы массива";
}

// Оператор декрементирования
HashTableLow::Iterator HashTableLow::Iterator::operator --()
{
    if (index_ >= static_cast<size_type>(1)) {
        --index_;
        return *this;
    }

    else
        throw "Выход за пределы массива";
}

// Оператор сдвига на число вправо
HashTableLow::Iterator HashTableLow::Iterator::operator +(const HashTableLow::Iterator::size_type& shift)
{
    index_ = index_ + shift;

    if (index_ < static_cast<size_type>(ptrCurrent_->nBuckets_))
        return *this;

    else
        throw "Выход за пределы массива";
}

// Оператор сдвига на число влево
HashTableLow::Iterator HashTableLow::Iterator::operator -(const HashTableLow::Iterator::size_type& shift)
{
    if (index_ >= shift) {
        index_ = index_ - shift;
        return *this;
    }

    else
        throw "Выход за пределы массива";
}

// Оператор сдвига на число вправо (сокращённая форма)
HashTableLow::Iterator& HashTableLow::Iterator::operator +=(const HashTableLow::Iterator::size_type& shift)
{
    index_ = index_ + shift;

    if (index_ < static_cast<size_type>(ptrCurrent_->nBuckets_))
        return *this;

    else
        throw "Выход за пределы массива";
}

// Оператор сдвига на число влево (сокращённая форма)
HashTableLow::Iterator& HashTableLow::Iterator::operator -=(const HashTableLow::Iterator::size_type& shift)
{
    if (index_ >= shift) {
        index_ = index_ - shift;
        return *this;
    }

    else
        throw "Выход за пределы массива";
}

// Разница между итераторами с точностью до знака
HashTableLow::Iterator::difference_type HashTableLow::Iterator::operator -(const HashTableLow::Iterator& iteratorSubtract)
{
    size_type difference = index_ - iteratorSubtract.index_;

    return static_cast<std::ptrdiff_t>(difference);
}

// Оператор эквивалентности
bool HashTableLow::Iterator::operator ==(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ == iteratorCompare.index_);

    return result;
}

// Оператор неэквивалентности
bool HashTableLow::Iterator::operator !=(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ != iteratorCompare.index_);

    return result;
}

// Оператор больше
bool HashTableLow::Iterator::operator >(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ > iteratorCompare.index_);

    return result;
}

// Оператор меньше
bool HashTableLow::Iterator::operator <(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ < iteratorCompare.index_);

    return result;
}

// Оператор больше или равно
bool HashTableLow::Iterator::operator >=(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ >= iteratorCompare.index_);

    return result;
}

// Оператор меньше или равно
bool HashTableLow::Iterator::operator <=(const HashTableLow::Iterator& iteratorCompare)
{
    bool result = (index_ <= iteratorCompare.index_);

    return result;
}
