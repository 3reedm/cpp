#include <iostream>
#include <cstdlib>
#include "hashtablehigh.h"
#include "hashtablelowopen.h"
#include "hashtablelowlist.h"

char* getRandomBytes(int);

// Главная функция
int main()
{
    int bitsOffset = 11, bitsKey = 16, bitsValue = 32, lengthBuckets = 65536, searchStrategy = 2, factors, sizeBlock = 10;
    HashTableLow* table;
    table = new HashTableLowOpen("offsets", "table", "meta", lengthBuckets, bitsOffset, bitsKey, bitsValue, searchStrategy, &factors);

    std::size_t j;
    char* ptrKey;
    char* ptrValue;
    for (j = static_cast<std::size_t>(0); j < static_cast<std::size_t>(100000); ++j) {
        ptrKey = getRandomBytes(bitsKey);
        ptrValue = getRandomBytes(bitsValue);
        table->addLow(ptrKey, ptrValue, static_cast<std::size_t>(bitsKey), static_cast<std::size_t>(bitsValue));
    }

    std::size_t i, sizeTable = table->sizeLow();

    //std::cout << sizeTable;

    std::pair<char*, char*> output;
    for (i = static_cast<std::size_t>(0); i < sizeTable; ++i) {
        output = (*table)[i];
        std::cout << output.first << " " << output.second << '\n';
    }

    table->~HashTableLow();
    return 0;
}

// Получение рандомного набора байтов, заданной длины
char* getRandomBytes(int length)
{
    char symbols[62] = {'a', 'b', 'c', 'd', 'e',
                        'f', 'j', 'h', 'i', 'j',
                        'k', 'l', 'm', 'n', 'o',
                        'p', 'q', 'r', 's', 't',
                        'u', 'v', 'w', 'x', 'y',
                        'z', 'A', 'B', 'C', 'D',
                        'E', 'F', 'J', 'H', 'I',
                        'J', 'K', 'L', 'M', 'N',
                        'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z', '0', '1', '2',
                        '3', '4', '5', '6', '7',
                        '8', '9'};

    char* randomBytes = new char[length];
    int i;
    for (i = 0; i < length; ++i)
        randomBytes[i] = symbols[rand() % 62];

    return randomBytes;
}
