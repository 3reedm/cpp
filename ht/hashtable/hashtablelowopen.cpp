#include "hashtablelowopen.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

// Вспомогательная хеш-функция
unsigned int getSecondHashCode(unsigned int firstHash)
{
    if ((firstHash % static_cast<unsigned int>(2)) == static_cast<unsigned int>(0))
        ++firstHash;

    return firstHash;
}

                                           // Реализация функций класса HashTableLowOpen

// Основные функции

// Конструктор на основе данных
HashTableLowOpen::HashTableLowOpen(std::string fileNameOffsetsInput, std::string fileNameHashTableInput, std::string fileNameInfoInput, int lengthBucketsInput, int bitsOffsetInput, int bitsKeyInput, int bitsValueInput, int searchStrategyInput, int* factorsInput)
{
    fileNameOffsets_ = fileNameOffsetsInput;
    fileNameHashTable_ = fileNameHashTableInput;
    fileNameInfo_ = fileNameInfoInput;

    fileOffsets_.open(fileNameOffsets_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileHashTable_.open(fileNameHashTable_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileInfo_.open(fileNameInfo_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);

    unsigned int fileInfoSize;
    fileInfo_.seekg(static_cast<std::size_t>(0), std::ios_base::end);
    fileInfoSize = static_cast<unsigned int>(fileInfo_.tellg()) / 36;

    std::size_t i;
    if (fileInfoSize == 0) {
        lengthBuckets_ = static_cast<std::size_t>(lengthBucketsInput);
        bitsOffset_ = static_cast<std::size_t>(bitsOffsetInput);
        bitsKey_ = static_cast<std::size_t>(bitsKeyInput);
        bitsValue_ = static_cast<std::size_t>(bitsValueInput);
        searchStrategy_ = static_cast<std::size_t>(searchStrategyInput);
        if (searchStrategy_ == 0) {
            factors_ = new std::size_t;
            *factors_ = static_cast<std::size_t>(*factorsInput);
        }
        if (searchStrategy_ == 1) {
            factors_ = new std::size_t[2];
            factors_[0] = static_cast<std::size_t>(factorsInput[0]);
            factors_[1] = static_cast<std::size_t>(factorsInput[1]);
        }
        if (searchStrategy_ == 2)
            factors_ = new std::size_t;

        int fillPos;
        char infoBuff[static_cast<std::size_t>(21)];
        if (!fileInfo_.good())
            fileInfo_.clear();
        fileInfo_.seekp(static_cast<std::size_t>(0));
        fillPos = snprintf(infoBuff, static_cast<std::size_t>(21), "%zu", lengthBuckets_);
        std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(20), '\0');
        fileInfo_.write(infoBuff, static_cast<long>(20));

        fillPos = snprintf(infoBuff, static_cast<std::size_t>(6), "%zu", bitsOffset_);
        std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(5), '\0');
        fileInfo_.write(infoBuff, static_cast<long>(5));

        fillPos = snprintf(infoBuff, static_cast<std::size_t>(6), "%zu", bitsKey_);
        std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(5), '\0');
        fileInfo_.write(infoBuff, static_cast<long>(5));

        fillPos = snprintf(infoBuff, static_cast<std::size_t>(6), "%zu", bitsValue_);
        std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(5), '\0');
        fileInfo_.write(infoBuff, static_cast<long>(5));

        snprintf(infoBuff, static_cast<std::size_t>(2), "%zu", searchStrategy_);
        fileInfo_.write(infoBuff, static_cast<long>(1));

        if (searchStrategy_ == 0) {
            fillPos = snprintf(infoBuff, static_cast<std::size_t>(3), "%zu", *factors_);
            std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(2), '\0');
            fileInfo_.write(infoBuff, static_cast<long>(2));
        }
        if (searchStrategy_ == 1) {
            fillPos = snprintf(infoBuff, static_cast<std::size_t>(3), "%zu", factors_[0]);
            std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(2), '\0');
            fileInfo_.write(infoBuff, static_cast<long>(2));

            fillPos = snprintf(infoBuff, static_cast<std::size_t>(3), "%zu", factors_[1]);
            std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(2), '\0');
            fileInfo_.write(infoBuff, static_cast<long>(2));
        }

        resultLow_.first = new char[bitsKey_ + static_cast<std::size_t>(1)];
        resultLow_.second = new char[bitsValue_ + static_cast<std::size_t>(1)];
        resultLow_.first[bitsKey_] = '\0';
        resultLow_.second[bitsValue_] = '\0';

        offsets_.resize(lengthBuckets_);
        nBuckets_ = static_cast<std::size_t>(0);

        fileOffsets_.seekp(static_cast<std::size_t>(0));
        for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i) {
            offsets_[i] = new char[bitsOffset_];
            std::fill(offsets_[i], offsets_[i] + bitsOffset_, '\0');
            fileOffsets_.write(offsets_[static_cast<std::size_t>(0)], static_cast<long>(bitsOffset_));
        }

        fileHashTable_.seekp(static_cast<std::size_t>(0));
        char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
        std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
        for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i)
            fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));

        delete[] ptrKeyValueBuff;
    }

    else {
        if (!fileInfo_.good())
            fileInfo_.clear();
        char lengthBuff[static_cast<std::size_t>(20)];
        fileInfo_.seekg(static_cast<std::size_t>(0));
        fileInfo_.read(lengthBuff, static_cast<long>(20));
        lengthBuckets_ = static_cast<std::size_t>(strtol(lengthBuff, reinterpret_cast<char**>('\0'), 10));

        char bitsBuff[static_cast<std::size_t>(5)];
        fileInfo_.read(bitsBuff, static_cast<long>(5));
        bitsOffset_ = static_cast<std::size_t>(strtol(bitsBuff, reinterpret_cast<char**>('\0'), 10));

        fileInfo_.read(bitsBuff, static_cast<long>(5));
        bitsKey_ = static_cast<std::size_t>(strtol(bitsBuff, reinterpret_cast<char**>('\0'), 10));

        fileInfo_.read(bitsBuff, static_cast<long>(5));
        bitsValue_ = static_cast<std::size_t>(strtol(bitsBuff, reinterpret_cast<char**>('\0'), 10));

        char strategyBuff[static_cast<std::size_t>(2)];
        strategyBuff[1] = '\0';
        fileInfo_.read(strategyBuff, static_cast<long>(1));
        searchStrategy_ = static_cast<std::size_t>(strtol(strategyBuff, reinterpret_cast<char**>('\0'), 10));

        if (searchStrategy_ == 0) {
            factors_ = new std::size_t;
            char factorBuff[static_cast<std::size_t>(2)];
            fileInfo_.read(factorBuff, static_cast<long>(2));
            *factors_ = static_cast<std::size_t>(strtol(factorBuff, reinterpret_cast<char**>('\0'), 10));
        }

        if (searchStrategy_ == 1) {
            factors_ = new std::size_t[2];
            char factorBuff[static_cast<std::size_t>(2)];
            fileInfo_.read(factorBuff, static_cast<long>(2));
            factors_[0] = static_cast<std::size_t>(strtol(factorBuff, reinterpret_cast<char**>('\0'), 10));

            fileInfo_.read(factorBuff, static_cast<long>(2));
            factors_[1] = static_cast<std::size_t>(strtol(factorBuff, reinterpret_cast<char**>('\0'), 10));
        }

        if (searchStrategy_ == 2)
            factors_ = new std::size_t;

        resultLow_.first = new char[bitsKey_ + static_cast<std::size_t>(1)];
        resultLow_.second = new char[bitsValue_ + static_cast<std::size_t>(1)];
        resultLow_.first[bitsKey_] = '\0';
        resultLow_.second[bitsValue_] = '\0';

        char* ptrOffsetBuff = new char[bitsOffset_];
        offsets_.resize(lengthBuckets_);
        for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i) {
            offsets_[i] = new char[bitsOffset_];
            std::fill(offsets_[i], offsets_[i] + bitsOffset_, '\0');
        }
        nBuckets_ = static_cast<std::size_t>(0);

        i = static_cast<std::size_t>(0);
        long buffer;
        fileOffsets_.seekg(static_cast<std::size_t>(0));
        while (fileOffsets_.read(ptrOffsetBuff, static_cast<long>(bitsOffset_))) {
            if (ptrOffsetBuff[0] == 'D')
                std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, offsets_[i]);
            else {
                buffer = strtol(ptrOffsetBuff, reinterpret_cast<char**>('\0'), 10);

                if (buffer != 0) {
                    ++nBuckets_;
                    std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, offsets_[i]);
                }
            }
            ++i;
        }

        delete[] ptrOffsetBuff;
    }

    hashFunction_ = new DefaultHashFunction;
    iteratorLow_ = HashTableLow::Iterator(this);
}

// Конструктор копирования
HashTableLowOpen::HashTableLowOpen(const HashTableLowOpen& tableCopy) : HashTableLow(tableCopy)
{
    std::size_t i, size = tableCopy.offsets_.size();
    for (i = static_cast<std::size_t>(0); i < size; ++i) {
        offsets_[i] = new char[bitsOffset_];
        std::copy(tableCopy.offsets_[i], tableCopy.offsets_[i] + bitsOffset_, offsets_[i]);
    }
    searchStrategy_ = tableCopy.searchStrategy_;
    if (searchStrategy_ == 0) {
        factors_ = new std::size_t;
        *factors_ = *tableCopy.factors_;
    }
    if (searchStrategy_ == 1) {
        factors_ = new std::size_t[2];
        factors_[0] = tableCopy.factors_[0];
        factors_[1] = tableCopy.factors_[1];
    }
    if (searchStrategy_ == 2)
        factors_ = new std::size_t;
}

// Деструктор
HashTableLowOpen::~HashTableLowOpen()
{
    std::size_t i, size = offsets_.size();
    for (i = static_cast<std::size_t>(0); i < size; ++i)
        delete[] offsets_[i];
    if ((searchStrategy_ == 0) || (searchStrategy_ == 2))
        delete factors_;
    if (searchStrategy_ == 1)
        delete[] factors_;
}

// Добавление нового элемента
bool HashTableLowOpen::addLow(char* ptrKey, char* ptrValue, std::size_t bitsKey, std::size_t bitsValue)
{
    if ((bitsKey > bitsKey_) || (bitsValue > bitsValue_))
        throw "Длина ключа или значения больше возможной";

    std::size_t number, shift;
    if (!getStatus(ptrKey, bitsKey, number)) {
        unsigned int lengthKey = static_cast<unsigned int>(bitsKey);

        double loadFactor = (static_cast<double>(nBuckets_) + 1.0) / static_cast<double>(lengthBuckets_);
        std::size_t newLength;
        if (loadFactor > 0.8) {
            newLength = lengthBuckets_ * static_cast<std::size_t>(2);
            resizeLow(&newLength);
        }

        std::size_t index;
        index = static_cast<std::size_t>(hashFunction_->getHashCode(ptrKey, lengthKey)) % lengthBuckets_;

        std::size_t i, h;
        int fillPos;
        char* ptrOffsetBuff = new char[bitsOffset_ + static_cast<std::size_t>(1)];

        if (searchStrategy_ == 2)
            *factors_ = static_cast<std::size_t>(getSecondHashCode(static_cast<unsigned int>(index)));
        for (h = index, i = static_cast<std::size_t>(1); i < lengthBuckets_; ++i) {
            if ((offsets_[h][0] == '\0') || (offsets_[h][0] == 'D')) {
                shift = h * bitsOffset_;

                if (!fileOffsets_.good())
                    fileOffsets_.clear();
                fileOffsets_.seekp(shift);

                if (h == static_cast<std::size_t>(0))
                    shift = static_cast<std::size_t>(1);
                else
                    shift = h * (bitsKey_ + bitsValue_);
                fillPos = snprintf(ptrOffsetBuff, bitsOffset_ + static_cast<std::size_t>(1), "%d", shift);
                std::fill(ptrOffsetBuff + static_cast<std::size_t>(fillPos), ptrOffsetBuff + bitsOffset_, '\0');

                std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, offsets_[h]);
                fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

                if (!fileHashTable_.good())
                    fileHashTable_.clear();
                if (h == static_cast<std::size_t>(0))
                    shift = static_cast<std::size_t>(0);
                fileHashTable_.seekp(shift);
                char* ptrKeyBuff = new char[bitsKey_];
                std::copy(ptrKey, ptrKey + bitsKey, ptrKeyBuff);
                std::fill(ptrKeyBuff + bitsKey, ptrKeyBuff + bitsKey_, '\0');

                char* ptrValueBuff = new char[bitsValue_];
                std::copy(ptrValue, ptrValue + bitsValue, ptrValueBuff);
                std::fill(ptrValueBuff + bitsValue, ptrValueBuff + bitsValue_, '\0');

                fileHashTable_.write(ptrKeyBuff, static_cast<long>(bitsKey_));
                fileHashTable_.write(ptrValueBuff, static_cast<long>(bitsValue_));

                ++nBuckets_;

                delete[] ptrKeyBuff;
                delete[] ptrValueBuff;
                delete[] ptrOffsetBuff;

                return true;
            }

            if ((searchStrategy_ == 0) || (searchStrategy_ == 2))
                h = (index + *factors_ * i) % lengthBuckets_;
            if (searchStrategy_ == 1)
                h = (index + factors_[0] * i + factors_[1] * i * i) % lengthBuckets_;
        }
        return false;
    }

    // Замена значения
    else {
        if (!fileHashTable_.good())
            fileHashTable_.clear();
        shift = number * (bitsKey_ + bitsValue_) + bitsKey_;
        fileHashTable_.seekp(shift);

        char* ptrValueBuff = new char[bitsValue_];
        std::copy(ptrValue, ptrValue + bitsValue, ptrValueBuff);
        std::fill(ptrValueBuff + bitsValue, ptrValueBuff + bitsValue_, '\0');

        fileHashTable_.write(ptrValueBuff, static_cast<long>(bitsValue_));

        delete[] ptrValueBuff;

        return true;
    }
}

// Проверка, через ключ, на присутствие элемента в хеш-таблице
bool HashTableLowOpen::containsLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        throw "Длина ключа больше возможной!";

    std::size_t index;
    return getStatus(ptrKey, bitsKey, index);
}

// Удаление элемента по ключу из хеш-таблицы
bool HashTableLowOpen::removeLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        return false;

    std::size_t index;

    if (getStatus(ptrKey, bitsKey, index)) {
        if (!fileOffsets_.good())
            fileOffsets_.clear();
        if (!fileHashTable_.good())
            fileHashTable_.clear();

        fileOffsets_.seekp(index * bitsOffset_);
        fileHashTable_.seekp(index * (bitsKey_ + bitsValue_));

        char* ptrOffsetBuff = new char[bitsOffset_];
        ptrOffsetBuff[0] = 'D';
        std::fill(ptrOffsetBuff + static_cast<std::size_t>(1), ptrOffsetBuff + bitsOffset_, '\0');
        char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
        std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');

        fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));
        fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));

        offsets_[index][0] = 'D';
        std::fill(offsets_[index] + static_cast<std::size_t>(1), offsets_[index] + bitsOffset_, '\0');
        --nBuckets_;

        delete[] ptrOffsetBuff;
        delete[] ptrKeyValueBuff;

        return true;
    }

    else
        return false;
}

// Перестройка таблицы
bool HashTableLowOpen::resizeLow(std::size_t* newLength)
{
    std::string tmpFileNameHashTable;
    std::fstream tmpFileHashTable;

    fileHashTable_.close();
    fileOffsets_.close();

    tmpFileNameHashTable = fileNameHashTable_;
    fileNameHashTable_ = fileNameHashTable_ + "_changed";
    fileNameOffsets_ = fileNameOffsets_ + "_changed";

    tmpFileHashTable.open(tmpFileNameHashTable.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileOffsets_.open(fileNameOffsets_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileHashTable_.open(fileNameHashTable_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);

    std::size_t tmpLengthBuckets;
    tmpLengthBuckets = lengthBuckets_;
    lengthBuckets_ = *newLength;
    nBuckets_ = static_cast<std::size_t>(0);

    std::vector<char*> tmpOffsets;
    tmpOffsets.resize(tmpLengthBuckets);

    std::size_t i;
    for (i = static_cast<std::size_t>(0); i < tmpLengthBuckets; ++i) {
        tmpOffsets[i] = new char[bitsOffset_];
        std::copy(offsets_[i], offsets_[i] + bitsOffset_, tmpOffsets[i]);
    }

    offsets_.resize(lengthBuckets_);
    for (i = tmpLengthBuckets; i < lengthBuckets_; ++i)
        offsets_[i] = new char[bitsOffset_];

    fileOffsets_.seekp(static_cast<std::size_t>(0));
    char* ptrOffsetBuff = new char[bitsOffset_];
    std::fill(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, '\0');
    for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i) {
        std::fill(offsets_[i], offsets_[i] + bitsOffset_, '\0');
        fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));
    }

    fileHashTable_.seekp(static_cast<std::size_t>(0));
    char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
    std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
    for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i)
        fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));

    delete[] ptrKeyValueBuff;
    delete[] ptrOffsetBuff;

    std::size_t l, offset, bitsKey = static_cast<std::size_t>(0), bitsValue = static_cast<std::size_t>(0);
    char* ptrKeyBuff = new char[bitsKey_];
    char* ptrValueBuff = new char[bitsValue_];
    for (i = static_cast<std::size_t>(0); i < tmpLengthBuckets; ++i)
        if ((tmpOffsets[i][0] != '\0') && (tmpOffsets[i][0] != 'D')) {
            if (!tmpFileHashTable.good())
                tmpFileHashTable.clear();

            offset = static_cast<std::size_t>(strtol(tmpOffsets[i], reinterpret_cast<char**>('\0'), 10));
            if (offset == static_cast<std::size_t>(1))
                offset = static_cast<std::size_t>(0);

            tmpFileHashTable.seekg(offset);

            tmpFileHashTable.read(ptrKeyBuff, static_cast<long>(bitsKey_));
            tmpFileHashTable.read(ptrValueBuff, static_cast<long>(bitsValue_));

            bitsKey = bitsKey_;
            bitsValue = bitsValue_;
            for (l = static_cast<std::size_t>(0); l < bitsKey_; ++l)
                if (ptrKeyBuff[l] == '\0') {
                    bitsKey = l;
                    break;
                }

            for (l = static_cast<std::size_t>(0); l < bitsValue_; ++l)
                if (ptrValueBuff[l] == '\0') {
                    bitsValue = l;
                    break;
                }

            addLow(ptrKeyBuff, ptrValueBuff, bitsKey, bitsValue);
        }

    delete[] ptrKeyBuff;
    delete[] ptrValueBuff;

    for (i = static_cast<std::size_t>(0); i < tmpLengthBuckets; ++i)
        delete[] tmpOffsets[i];

    int fillPos;
    char infoBuff[static_cast<std::size_t>(21)];
    if (!fileInfo_.good())
        fileInfo_.clear();
    fileInfo_.seekp(static_cast<std::size_t>(0));
    fillPos = snprintf(infoBuff, static_cast<std::size_t>(21), "%zu", lengthBuckets_);
    std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(20), '\0');
    fileInfo_.write(infoBuff, static_cast<long>(20));

    tmpFileHashTable.close();

    return true;
}

// Получение элемента хеш-таблицы с помощью ключа
std::pair<char*, char*> HashTableLowOpen::getLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        throw "Элемент не найден в хеш-таблице!";

    std::size_t index;

    if (getStatus(ptrKey, bitsKey, index)) {
        if (!fileHashTable_.good())
            fileHashTable_.clear();
        fileHashTable_.seekg(index * (bitsKey_ + bitsValue_));
        fileHashTable_.read(resultLow_.first, static_cast<long>(bitsKey_));
        fileHashTable_.read(resultLow_.second, static_cast<long>(bitsValue_));

        return resultLow_;
    }

    else
        throw "Элемент не найден в хеш-таблице!";
}

// Получить размер таблицы
std::size_t HashTableLowOpen::sizeLow()
{
    return nBuckets_;
}


// Вспомогательные функции

// Получение элемента по индексу
std::pair<char*, char*>* HashTableLowOpen::getElementLow(const std::size_t& index)
{
    std::size_t offset = static_cast<std::size_t>(strtol(offsets_[index], reinterpret_cast<char**>('\0'), 10));
    if (offset == static_cast<std::size_t>(1))
        offset = static_cast<std::size_t>(0);

    if (!fileHashTable_.good())
        fileHashTable_.clear();
    fileHashTable_.seekg(offset);
    fileHashTable_.read(resultLow_.first, static_cast<long>(bitsKey_));
    fileHashTable_.read(resultLow_.second, static_cast<long>(bitsValue_));

    return &resultLow_;
}

// Получение нужного индекса
std::size_t HashTableLowOpen::getIndex(const std::size_t& index)
{
    std::size_t i, found = static_cast<std::size_t>(0);
    for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i) {
        if ((offsets_[i][0] != '\0') && (offsets_[i][0] != 'D'))
            ++found;
        if (index + static_cast<std::size_t>(1) == found)
            return i;
    }

    throw "Выход за пределы массива";
}

// Статус присутствия элемента и его номер
bool HashTableLowOpen::getStatus(char* ptrKey, std::size_t bitsKey, std::size_t& index)
{
    std::size_t number;
    unsigned int lengthKey = static_cast<unsigned int>(bitsKey);

    number = static_cast<std::size_t>(hashFunction_->getHashCode(ptrKey, lengthKey)) % lengthBuckets_;

    bool bFlag;  
    std::size_t i, h, shift, changeBitsKey = bitsKey_ - bitsKey;
    char* ptrKeyChangeBuff = new char[changeBitsKey];
    char* ptrKeyBuff = new char[bitsKey_];
    if (changeBitsKey != static_cast<std::size_t>(0))
        std::fill(ptrKeyChangeBuff, ptrKeyChangeBuff + changeBitsKey, '\0');

    if (searchStrategy_ == 2)
        *factors_ = static_cast<std::size_t>(getSecondHashCode(static_cast<unsigned int>(number)));
    for (h = number, i = static_cast<std::size_t>(1); i < lengthBuckets_; ++i) {
        if (offsets_[h][0] != '\0') {
            if (offsets_[h][0] == 'D')
                continue;
            shift = h * (bitsKey_ + bitsValue_);
            fileHashTable_.seekg(shift);
            fileHashTable_.read(ptrKeyBuff, static_cast<long>(bitsKey_));

            if (changeBitsKey != static_cast<std::size_t>(0))
                bFlag = std::equal(ptrKey, ptrKey + bitsKey, ptrKeyBuff) & std::equal(ptrKeyBuff + bitsKey, ptrKeyBuff + bitsKey_, ptrKeyChangeBuff);
            else
                bFlag = std::equal(ptrKey, ptrKey + bitsKey_, ptrKeyBuff);

            if (bFlag) {
                index = h;

                if (changeBitsKey != static_cast<std::size_t>(0))
                    delete[] ptrKeyChangeBuff;
                else
                    delete ptrKeyChangeBuff;
                delete[] ptrKeyBuff;

                return true;
            }
        }
        else {
            if (changeBitsKey != static_cast<std::size_t>(0))
                delete[] ptrKeyChangeBuff;
            else
                delete ptrKeyChangeBuff;
            delete[] ptrKeyBuff;

            return false;
        }

        if ((searchStrategy_ == 0) || (searchStrategy_ == 2))
            h = (number + *factors_ * i) % lengthBuckets_;
        if (searchStrategy_ == 1)
            h = (number + factors_[0] * i + factors_[1] * i * i) % lengthBuckets_;
    }

    if (changeBitsKey != static_cast<std::size_t>(0))
        delete[] ptrKeyChangeBuff;
    else
        delete ptrKeyChangeBuff;
    delete[] ptrKeyBuff;

    return false;
}
