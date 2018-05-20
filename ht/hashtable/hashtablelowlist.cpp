#include "hashtablelowlist.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

                                           // Реализация функций класса HashTableLowList

// Основные функции

// Конструктор на основе данных
HashTableLowList::HashTableLowList(std::string fileNameOffsetsInput, std::string fileNameHashTableInput, std::string fileNameInfoInput, int lengthBucketsInput, int bitsOffsetInput, int bitsKeyInput, int bitsValueInput, int sizeBlockInput)
{
    fileNameOffsets_ = fileNameOffsetsInput;
    fileNameHashTable_ = fileNameHashTableInput;
    fileNameInfo_ = fileNameInfoInput;

    fileOffsets_.open(fileNameOffsets_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileHashTable_.open(fileNameHashTable_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    fileInfo_.open(fileNameInfo_.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);

    unsigned int fileInfoSize;
    fileInfo_.seekg(static_cast<std::size_t>(0), std::ios_base::end);
    fileInfoSize = static_cast<unsigned int>(fileInfo_.tellg()) / 38;

    if (fileInfoSize == 0) {
        lengthBuckets_ = static_cast<std::size_t>(lengthBucketsInput);
        bitsOffset_ = static_cast<std::size_t>(bitsOffsetInput);
        bitsKey_ = static_cast<std::size_t>(bitsKeyInput);
        bitsValue_ = static_cast<std::size_t>(bitsValueInput);
        sizeBlock_ = static_cast<std::size_t>(sizeBlockInput + 1);

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

        fillPos = snprintf(infoBuff, static_cast<std::size_t>(4), "%zu", sizeBlock_);
        std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(3), '\0');
        fileInfo_.write(infoBuff, static_cast<long>(3));

        resultLow_.first = new char[bitsKey_ + static_cast<std::size_t>(1)];
        resultLow_.second = new char[bitsValue_ + static_cast<std::size_t>(1)];
        resultLow_.first[bitsKey_] = '\0';
        resultLow_.second[bitsValue_] = '\0';

        offsets_.resize(lengthBuckets_);
        offsetsFlag_.resize(lengthBuckets_, true);
        nBuckets_ = static_cast<std::size_t>(0);
        nLinks_ = static_cast<std::size_t>(0);

        char* ptrOffsetBuff = new char[bitsOffset_];
        std::size_t i, allSize;
        std::fill(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, '\0');
        fileOffsets_.seekp(static_cast<std::size_t>(0));
        allSize = lengthBuckets_ * sizeBlock_ * static_cast<std::size_t>(2);
        for (i = static_cast<std::size_t>(0); i < allSize; ++i) {
            if (!fileOffsets_.good())
                fileOffsets_.clear();
            fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));
        }

        char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
        std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
        fileHashTable_.seekp(static_cast<std::size_t>(0));
        allSize = lengthBuckets_ * (sizeBlock_ - static_cast<std::size_t>(1)) * static_cast<std::size_t>(2);
        for (i = static_cast<std::size_t>(0); i < allSize; ++i) {
            if (!fileHashTable_.good())
                fileHashTable_.clear();
            fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));
        }

        delete[] ptrOffsetBuff;
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

        char blockBuff[static_cast<std::size_t>(3)];
        fileInfo_.read(blockBuff, static_cast<long>(3));
        sizeBlock_ = static_cast<std::size_t>(strtol(blockBuff, reinterpret_cast<char**>('\0'), 10));

        resultLow_.first = new char[bitsKey_ + static_cast<std::size_t>(1)];
        resultLow_.second = new char[bitsValue_ + static_cast<std::size_t>(1)];
        resultLow_.first[bitsKey_] = '\0';
        resultLow_.second[bitsValue_] = '\0';

        char* ptrOffsetBuff = new char[bitsOffset_];

        offsets_.resize(lengthBuckets_);
        offsetsFlag_.resize(lengthBuckets_, true);
        nBuckets_ = static_cast<std::size_t>(0);
        nLinks_ = static_cast<std::size_t>(0);

        std::size_t i, j;
        long buffer;
        std::vector<std::size_t> links;
        std::vector<std::size_t> shifts;

        if (!fileOffsets_.good())
            fileOffsets_.clear();
        fileOffsets_.seekg(static_cast<std::size_t>(0));
        for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i)
            for (j = static_cast<std::size_t>(0); j < sizeBlock_; ++j) {
                fileOffsets_.read(ptrOffsetBuff, static_cast<long>(bitsOffset_));
                buffer = strtol(ptrOffsetBuff, reinterpret_cast<char**>('\0'), 10);

                if (buffer != 0) {
                    if (j != sizeBlock_ - static_cast<std::size_t>(1)) {
                        offsets_[i].push_back(new char[bitsOffset_]);
                        std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, *--offsets_[i].end());
                        ++nBuckets_;
                    }
                    else {
                        ++nLinks_;
                        links.push_back(i);
                        shifts.push_back(buffer);
                    }
                }
            }

        std::size_t sizeLinks = links.size(), sizeEraseLinks, shift, index, number;
        std::vector<std::size_t> eraseLinks;
        sizeEraseLinks = static_cast<std::size_t>(0);
        while (sizeLinks != static_cast<std::size_t>(0)) {
            if (sizeEraseLinks != static_cast<std::size_t>(0)) {
                sizeEraseLinks = static_cast<std::size_t>(0);
                eraseLinks.clear();
            }

            for (i = static_cast<std::size_t>(0); i < sizeLinks; ++i) {
                index = links[i];
                shift = shifts[i];
                number = (shift / bitsOffset_ / sizeBlock_) % lengthBuckets_;
                offsetsFlag_[number] = false;
                if (!fileOffsets_.good())
                    fileOffsets_.clear();
                fileOffsets_.seekg(shift);
                for (j = static_cast<std::size_t>(0); j < sizeBlock_; ++j) {
                    fileOffsets_.read(ptrOffsetBuff, static_cast<long>(bitsOffset_));
                    buffer = strtol(ptrOffsetBuff, reinterpret_cast<char**>('\0'), 10);

                    if (buffer != 0) {
                        if (j != sizeBlock_ - static_cast<std::size_t>(1)) {
                            offsets_[index].push_back(new char[bitsOffset_]);
                            std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, *--offsets_[index].end());
                            ++nBuckets_;
                        }
                        else {
                            ++nLinks_;
                            shifts[i] = buffer;
                            break;
                        }
                    }

                    if (j == sizeBlock_ - static_cast<std::size_t>(1))
                        eraseLinks.push_back(i);
                }
            }

            sizeEraseLinks = eraseLinks.size();
            for (i = static_cast<std::size_t>(0); i < sizeEraseLinks; ++i) {
                links.erase(links.begin() + eraseLinks[i] - i);
                shifts.erase(shifts.begin() + eraseLinks[i] - i);
            }

            sizeLinks = sizeLinks - sizeEraseLinks;
        }

        delete[] ptrOffsetBuff;
    }

    hashFunction_ = new DefaultHashFunction;
    iteratorLow_ = HashTableLow::Iterator(this);
}

// Конструктор копирования
HashTableLowList::HashTableLowList(const HashTableLowList& tableCopy) : HashTableLow(tableCopy)
{
    std::size_t i, j, size = tableCopy.offsets_.size(), sizeList;
    sizeBlock_ = tableCopy.sizeBlock_;
    nLinks_ = tableCopy.nLinks_;
    std::list<char*>::iterator offsetIterator;
    std::list<char*>::const_iterator coppiedOffsetIterator;

    for (i = static_cast<std::size_t>(0); i < size; ++i) {
        sizeList = tableCopy.offsets_[i].size();
        for (j = static_cast<std::size_t>(0); j < sizeList; ++j)
            offsets_[i].push_back(new char[bitsOffset_]);
        offsetsFlag_[i] = tableCopy.offsetsFlag_[i];
    }

    for (i = static_cast<std::size_t>(0); i < size; ++i)
        for (offsetIterator = offsets_[i].begin(), coppiedOffsetIterator = tableCopy.offsets_[i].begin(); offsetIterator != offsets_[i].end(), coppiedOffsetIterator != tableCopy.offsets_[i].end(); ++offsetIterator, ++coppiedOffsetIterator)
            std::copy(*coppiedOffsetIterator, *coppiedOffsetIterator + bitsOffset_, *offsetIterator);
}

// Деструктор
HashTableLowList::~HashTableLowList()
{
    std::size_t i, size = offsets_.size();
    std::list<char*>::iterator offsetIterator;
    for (i = static_cast<std::size_t>(0); i < size; ++i)
        for (offsetIterator = offsets_[i].begin(); offsetIterator != offsets_[i].end(); ++offsetIterator)
            delete[] *offsetIterator;
}

// Добавление нового элемента
bool HashTableLowList::addLow(char* ptrKey, char* ptrValue, std::size_t bitsKey, std::size_t bitsValue)
{
    if ((bitsKey > bitsKey_) || (bitsValue > bitsValue_))
        throw "Длина ключа или значения больше возможной";

    std::size_t number, numberList, shift;
    if (!getStatus(ptrKey, bitsKey, number, numberList)) {
        unsigned int lengthKey = static_cast<unsigned int>(bitsKey);
        std::size_t index;

        double loadFactor = (static_cast<double>(nBuckets_) + 1.0) / static_cast<double>(lengthBuckets_) / static_cast<double>(sizeBlock_ - static_cast<std::size_t>(1));
        double linkFactor = (static_cast<double>(nLinks_) + 1.0) / static_cast<double>(lengthBuckets_);
        std::size_t newLength[2];
        if (loadFactor >= 0.8 && linkFactor >= 0.8) {
            newLength[0] = lengthBuckets_ * static_cast<std::size_t>(2);
            newLength[1] = sizeBlock_ * static_cast<std::size_t>(2);
            resizeLow(newLength);
        }
        else if (loadFactor >= 0.8 && linkFactor < 0.8) {
            newLength[0] = lengthBuckets_ * static_cast<std::size_t>(2);
            newLength[1] = sizeBlock_;
            resizeLow(newLength);
        }
        else if (loadFactor < 0.8 && linkFactor >= 0.8) {
            newLength[0] = lengthBuckets_;
            newLength[1] = sizeBlock_ * static_cast<std::size_t>(2);
            resizeLow(newLength);
        }
        index = static_cast<std::size_t>(hashFunction_->getHashCode(ptrKey, lengthKey)) % lengthBuckets_;

        std::size_t newIndex, i, indicator, sizeList;
        int fillPos;
        sizeList = offsets_[index].size();
        indicator = sizeList % (sizeBlock_ - static_cast<std::size_t>(1));

        char* ptrOffsetBuff = new char[bitsOffset_ + static_cast<std::size_t>(1)];
        if ((indicator == static_cast<std::size_t>(0)) && (sizeList != static_cast<std::size_t>(0))) {
            for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i)
                if (offsetsFlag_[i]) {
                    newIndex = lengthBuckets_ + i;
                    offsetsFlag_[i] = false;
                    ++nLinks_;
                    break;
                }

            if (!fileOffsets_.good())
                fileOffsets_.clear();
            std::size_t offset = static_cast<std::size_t>(strtol(*--offsets_[index].end(), reinterpret_cast<char**>('\0'), 10)) / (bitsKey_ + bitsValue_);
            shift = (offset + offset / (sizeBlock_ - static_cast<std::size_t>(1)) + static_cast<std::size_t>(1)) * bitsOffset_;
            fileOffsets_.seekp(shift);
            shift = newIndex * sizeBlock_ * bitsOffset_;
            fillPos = snprintf(ptrOffsetBuff, bitsOffset_ + static_cast<std::size_t>(1), "%zu", shift);
            std::fill(ptrOffsetBuff + static_cast<std::size_t>(fillPos), ptrOffsetBuff + bitsOffset_, '\0');
            fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

            fileOffsets_.seekp(shift);
            shift = newIndex * (sizeBlock_ - static_cast<std::size_t>(1)) * (bitsKey_ + bitsValue_);
            fillPos = snprintf(ptrOffsetBuff, bitsOffset_ + static_cast<std::size_t>(1), "%zu", shift);
            std::fill(ptrOffsetBuff + static_cast<std::size_t>(fillPos), ptrOffsetBuff + bitsOffset_, '\0');
            offsets_[index].push_back(new char[bitsOffset_]);
            std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, *--offsets_[index].end());
            fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

            if (!fileHashTable_.good())
                fileHashTable_.clear();
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

            delete[] ptrOffsetBuff;
            delete[] ptrKeyBuff;
            delete[] ptrValueBuff;

            return true;
        }

        else {
            if (!fileOffsets_.good())
                fileOffsets_.clear();
            if (sizeList == static_cast<std::size_t>(0)) {
                shift = index * sizeBlock_ * bitsOffset_;
                fileOffsets_.seekp(shift);
                if (index == static_cast<std::size_t>(0))
                    shift = static_cast<std::size_t>(1);
                else
                    shift = index * (sizeBlock_ - static_cast<std::size_t>(1)) * (bitsKey_ + bitsValue_);
            }

            else {
                std::size_t offset = static_cast<std::size_t>(strtol(*--offsets_[index].end(), reinterpret_cast<char**>('\0'), 10)) / (bitsKey_ + bitsValue_);
                if (index == static_cast<std::size_t>(0) && sizeList == static_cast<std::size_t>(1))
                    shift = bitsOffset_;
                else
                    shift = (offset + offset / (sizeBlock_ - static_cast<std::size_t>(1)) + static_cast<std::size_t>(1)) * bitsOffset_;
                fileOffsets_.seekp(shift);
                if (index == static_cast<std::size_t>(0) && sizeList == static_cast<std::size_t>(1))
                    shift = bitsKey_ + bitsValue_;
                else
                    shift = (offset + static_cast<std::size_t>(1)) * (bitsKey_ + bitsValue_);
            }

            fillPos = snprintf(ptrOffsetBuff, bitsOffset_ + static_cast<std::size_t>(1), "%zu", shift);
            std::fill(ptrOffsetBuff + static_cast<std::size_t>(fillPos), ptrOffsetBuff + bitsOffset_, '\0');
            offsets_[index].push_back(new char[bitsOffset_]);
            std::copy(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, *--offsets_[index].end());
            fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

            if (!fileHashTable_.good())
                fileHashTable_.clear();
            if (index == static_cast<std::size_t>(0) && sizeList == static_cast<std::size_t>(0))
                fileHashTable_.seekp(static_cast<std::size_t>(0));
            else
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

            delete[] ptrOffsetBuff;
            delete[] ptrKeyBuff;
            delete[] ptrValueBuff;

            return true;
        }

        delete[] ptrOffsetBuff;
    }

    // Замена значения
    else {
        if (!fileHashTable_.good())
            fileHashTable_.clear();
        shift = number + bitsKey_;
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
bool HashTableLowList::containsLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        throw "Длина ключа больше возможной!";

    std::size_t index, indexLink;
    return getStatus(ptrKey, bitsKey, index, indexLink);
}

// Удаление элемента по ключу из хеш-таблицы
bool HashTableLowList::removeLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        return false;

    std::size_t index, indexList;
    if (getStatus(ptrKey, bitsKey, index, indexList)) {
        std::size_t shift, sizeList;
        sizeList = offsets_[indexList].size();

        char* ptrOffsetBuff = new char[bitsOffset_ + static_cast<std::size_t>(1)];
        std::fill(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, '\0');

        if (!fileOffsets_.good())
            fileOffsets_.clear();
        if (sizeList == static_cast<std::size_t>(1)) {
            delete[] *--offsets_[indexList].end();
            offsets_[indexList].pop_back();

            shift = indexList * sizeBlock_ * bitsOffset_;
            fileOffsets_.seekp(shift);
            fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

            if (!fileHashTable_.good())
                fileHashTable_.clear();
            fileHashTable_.seekp(index);
            char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
            std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
            fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));

            delete[] ptrKeyValueBuff;
        }
        else {
            std::size_t i, offset, indicator, elementIndicator;
            indicator = sizeList % (sizeBlock_ - static_cast<std::size_t>(1)) ;
            elementIndicator = static_cast<std::size_t>(strtol(*--offsets_[indexList].end(), reinterpret_cast<char**>('\0'), 10));

            if (indicator == static_cast<std::size_t>(1)) {
                offset = static_cast<std::size_t>(strtol(*----offsets_[indexList].end(), reinterpret_cast<char**>('\0'), 10)) / (bitsKey_ + bitsValue_) / (sizeBlock_ - static_cast<std::size_t>(1));
                shift = offset * sizeBlock_ * bitsOffset_;
                fileOffsets_.seekp(shift);
                fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));
                offset = static_cast<std::size_t>(strtol(*--offsets_[indexList].end(), reinterpret_cast<char**>('\0'), 10)) / (bitsKey_ + bitsValue_) / (sizeBlock_ - static_cast<std::size_t>(1));
                i = offset % lengthBuckets_;
                offsetsFlag_[i] = true;
                --nLinks_;
            }

            if (index == elementIndicator) {
                offset = index / (bitsKey_ + bitsValue_);
                shift = (offset + offset / (sizeBlock_ - static_cast<std::size_t>(1))) * bitsOffset_;
                fileOffsets_.seekp(shift);
                delete[] *--offsets_[indexList].end();
                offsets_[indexList].pop_back();
                fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

                if (!fileHashTable_.good())
                    fileHashTable_.clear();
                fileHashTable_.seekp(index);
                char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
                std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
                fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));
                delete[] ptrKeyValueBuff;
            }

            else {
                offset = static_cast<std::size_t>(strtol(*--offsets_[indexList].end(), reinterpret_cast<char**>('\0'), 10)) / (bitsKey_ + bitsValue_);
                shift = (offset + offset / (sizeBlock_ - static_cast<std::size_t>(1))) * bitsOffset_;
                delete[] *--offsets_[indexList].end();
                offsets_[indexList].pop_back();
                fileOffsets_.seekp(shift);
                fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));

                shift = offset * (bitsKey_ + bitsValue_);
                char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
                if (!fileHashTable_.good())
                    fileHashTable_.clear();
                fileHashTable_.seekg(shift);
                fileHashTable_.read(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));

                fileHashTable_.seekp(index);
                fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));
                delete[] ptrKeyValueBuff;
            }
        }

        --nBuckets_;
        delete[] ptrOffsetBuff;

        return true;
    }

    return false;
}

// Перестройка таблицы
bool HashTableLowList::resizeLow(std::size_t* newLength)
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

    std::size_t tmpLengthBuckets, tmpSizeBlock;
    tmpLengthBuckets = lengthBuckets_;
    tmpSizeBlock = sizeBlock_;
    lengthBuckets_ = *newLength;
    sizeBlock_ = *++newLength;
    nBuckets_ = static_cast<std::size_t>(0);
    nLinks_ = static_cast<std::size_t>(0);

    std::vector<std::list<char*> > tmpOffsets;
    tmpOffsets.resize(tmpLengthBuckets);

    std::size_t i;
    for (i = static_cast<std::size_t>(0); i < tmpLengthBuckets; ++i)
        tmpOffsets[i].splice(tmpOffsets[i].begin(), offsets_[i]);

    char* ptrOffsetBuff = new char[bitsOffset_];
    offsets_.resize(lengthBuckets_);
    offsetsFlag_.clear();
    offsetsFlag_.resize(lengthBuckets_, true);

    fileOffsets_.seekp(static_cast<std::size_t>(0));
    std::fill(ptrOffsetBuff, ptrOffsetBuff + bitsOffset_, '\0');
    std::size_t allSize;
    allSize = lengthBuckets_ * sizeBlock_ * static_cast<std::size_t>(2);
    for (i = static_cast<std::size_t>(0); i < allSize; ++i) {
        if (!fileOffsets_.good())
            fileOffsets_.clear();
        fileOffsets_.write(ptrOffsetBuff, static_cast<long>(bitsOffset_));
    }
    delete[] ptrOffsetBuff;

    fileHashTable_.seekp(static_cast<std::size_t>(0));
    char* ptrKeyValueBuff = new char[bitsKey_ + bitsValue_];
    std::fill(ptrKeyValueBuff, ptrKeyValueBuff + bitsKey_ + bitsValue_, '\0');
    allSize = lengthBuckets_ * sizeBlock_ * static_cast<std::size_t>(2);
    for (i = static_cast<std::size_t>(0); i < allSize; ++i) {
        if (!fileOffsets_.good())
            fileOffsets_.clear();
        fileHashTable_.write(ptrKeyValueBuff, static_cast<long>(bitsKey_ + bitsValue_));
    }
    delete[] ptrKeyValueBuff;

    std::size_t l, offset, bitsKey = static_cast<std::size_t>(0), bitsValue = static_cast<std::size_t>(0);
    char* ptrKeyBuff = new char[bitsKey_];
    char* ptrValueBuff = new char[bitsValue_];
    allSize = tmpLengthBuckets * (tmpSizeBlock - static_cast<std::size_t>(1)) * static_cast<std::size_t>(2);
    if (!tmpFileHashTable.good())
        tmpFileHashTable.clear();
    std::list<char*>::iterator tmpOffsetIterator;
    for (i = static_cast<std::size_t>(0); i < tmpLengthBuckets; ++i)
        for (tmpOffsetIterator = tmpOffsets[i].begin(); tmpOffsetIterator != tmpOffsets[i].end(); ++tmpOffsetIterator) {
            offset = static_cast<std::size_t>(strtol(*tmpOffsetIterator, reinterpret_cast<char**>('\0'), 10));
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
        while (!tmpOffsets[i].empty()) {
            delete[] *--tmpOffsets[i].end();
            tmpOffsets[i].pop_back();
        }

    int fillPos;
    char infoBuff[static_cast<std::size_t>(21)];
    if (!fileInfo_.good())
        fileInfo_.clear();
    fileInfo_.seekp(static_cast<std::size_t>(0));
    fillPos = snprintf(infoBuff, static_cast<std::size_t>(21), "%zu", lengthBuckets_);
    std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(20), '\0');
    fileInfo_.write(infoBuff, static_cast<long>(20));

    fileInfo_.seekp(static_cast<std::size_t>(35));
    fillPos = snprintf(infoBuff, static_cast<std::size_t>(4), "%zu", sizeBlock_);
    std::fill(infoBuff + static_cast<std::size_t>(fillPos), infoBuff + static_cast<std::size_t>(3), '\0');
    fileInfo_.write(infoBuff, static_cast<long>(3));

    tmpFileHashTable.close();

    return true;
}

// Получение элемента хеш-таблицы с помощью ключа
std::pair<char*, char*> HashTableLowList::getLow(char* ptrKey, std::size_t bitsKey)
{
    if (bitsKey > bitsKey_)
        throw "Элемент не найден в хеш-таблице!";

    std::size_t index, indexList;

    if (getStatus(ptrKey, bitsKey, index, indexList)) {
        if (!fileHashTable_.good())
            fileHashTable_.clear();       
        fileHashTable_.seekg(index);
        fileHashTable_.read(resultLow_.first, static_cast<long>(bitsKey_));
        fileHashTable_.read(resultLow_.second, static_cast<long>(bitsValue_));

        return resultLow_;
    }

    else
        throw "Элемент не найден в хеш-таблице!";
}

// Получить размер таблицы
std::size_t HashTableLowList::sizeLow()
{
    return nBuckets_;
}


// Вспомогательные функции

// Получение элемента по индексу
std::pair<char*, char*>* HashTableLowList::getElementLow(const std::size_t& index)
{
    if (!fileHashTable_.good())
        fileHashTable_.clear();
    fileHashTable_.seekg(index);
    fileHashTable_.read(resultLow_.first, static_cast<long>(bitsKey_));
    fileHashTable_.read(resultLow_.second, static_cast<long>(bitsValue_));

    return &resultLow_;
}

// Получение нужного индекса
std::size_t HashTableLowList::getIndex(const std::size_t& index)
{
    std::size_t i, j, found = static_cast<std::size_t>(0), round = static_cast<std::size_t>(0), sizeRound, sizeList, sizeShift;
    bool bFound = true;
    std::list<char*>::iterator offsetIterator;

    while (bFound) {
        for (i = static_cast<std::size_t>(0); i < lengthBuckets_; ++i) {
            sizeList = offsets_[i].size();
            sizeShift = (sizeBlock_ - static_cast<std::size_t>(1)) * round;

            if (sizeList > sizeShift) {
                offsetIterator = offsets_[i].begin();

                for (j = static_cast<std::size_t>(0); j < sizeShift; ++j)
                    ++offsetIterator;

                sizeRound = sizeList - sizeShift;
                if ((sizeRound / (sizeBlock_ - static_cast<std::size_t>(1))) != static_cast<std::size_t>(0))
                    sizeRound = sizeBlock_ - static_cast<std::size_t>(1);

                for (j = static_cast<std::size_t>(0); j < sizeRound; ++j, ++offsetIterator) {
                    ++found;
                    if (index + static_cast<std::size_t>(1) == found) {
                        i = static_cast<std::size_t>(strtol(*offsetIterator, reinterpret_cast<char**>('\0'), 10));
                        if (i == static_cast<std::size_t>(1))
                            i = static_cast<std::size_t>(0);
                        return i;
                    }
                }

                bFound = false;
            }
        }

        if (!bFound) {
            bFound = true;
            ++round;
        }

        else
            bFound = false;
    }

    throw "Выход за пределы массива";
}

// Статус присутствия элемента и его номер
bool HashTableLowList::getStatus(char* ptrKey, std::size_t bitsKey, std::size_t& index, std::size_t& indexList)
{
    unsigned int lengthKey = static_cast<unsigned int>(bitsKey);

    indexList = static_cast<std::size_t>(hashFunction_->getHashCode(ptrKey, lengthKey)) % lengthBuckets_;

    std::size_t changeBitsKey = bitsKey_ - bitsKey;
    char* ptrKeyChangeBuff = new char[changeBitsKey];
    char* ptrKeyBuff = new char[bitsKey_];
    if (changeBitsKey != static_cast<std::size_t>(0))
        std::fill(ptrKeyChangeBuff, ptrKeyChangeBuff + changeBitsKey, '\0');

    std::size_t shift;
    std::list<char*>::iterator offsetIterator;
    bool bFlag = false;
    for (offsetIterator = offsets_[indexList].begin(); offsetIterator != offsets_[indexList].end(); ++offsetIterator) {
        shift = static_cast<std::size_t>(strtol(*offsetIterator, reinterpret_cast<char**>('\0'), 10));
        if (shift == static_cast<std::size_t>(1))
            shift = static_cast<std::size_t>(0);

        if (!fileHashTable_.good())
            fileHashTable_.clear();
        fileHashTable_.seekg(shift);
        fileHashTable_.read(ptrKeyBuff, static_cast<long>(bitsKey_));

        if (changeBitsKey != static_cast<std::size_t>(0))
            bFlag = std::equal(ptrKey, ptrKey + bitsKey, ptrKeyBuff) & std::equal(ptrKeyBuff + bitsKey, ptrKeyBuff + bitsKey_, ptrKeyChangeBuff);
        else
            bFlag = std::equal(ptrKey, ptrKey + bitsKey_, ptrKeyBuff);

        if (bFlag) {
            index = shift;

            if (changeBitsKey != static_cast<std::size_t>(0))
                delete[] ptrKeyChangeBuff;
            else
                delete ptrKeyChangeBuff;
            delete[] ptrKeyBuff;

            return true;
        }
    }

    if (changeBitsKey != static_cast<std::size_t>(0))
        delete[] ptrKeyChangeBuff;
    else
        delete ptrKeyChangeBuff;
    delete[] ptrKeyBuff;

    return false;
}
