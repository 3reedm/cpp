#include "tester.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>

// Тест на скорость записи
unsigned int Tester::writeTest(Store* ptrStore, int nWrites, int bitsKey, int bitsValue) {
    std::fstream fileResults;
    std::string fileResultsName = "resultsWrite";
    fileResults.open(fileResultsName.c_str(), std::ios_base::out | std::ios_base::app);

    int i, randBitsValue = rand() % (bitsValue - 99) + 100;
    char* key;
    char* value;

    std::clock_t time1, time2, sum = static_cast<std::clock_t>(0);
    for (i = 0; i < nWrites; ++i) {
        key = getRandomBytes(bitsKey);
        value = getRandomBytes(randBitsValue);

        time1 = clock();
        ptrStore->write(key, value, bitsKey, randBitsValue);
        time2 = clock();
        sum = sum + time2 - time1;

        std::cout << i << " ";

        delete[] key;
        delete[] value;
    }

    sum = static_cast<std::clock_t>(static_cast<float>(sum) / static_cast<float>(CLOCKS_PER_SEC));
    fileResults << sum << '\n';
    fileResults.close();

    return static_cast<unsigned int>(sum);
}

// Тест на скорость чтения
unsigned int Tester::readTest(Store* ptrStore, int nReads, int bitsKey) {
    std::fstream fileResults;
    std::string fileResultsName = "resultsRead";
    fileResults.open(fileResultsName.c_str(), std::ios_base::out | std::ios_base::app);

    int i, bitsKeyReal;
    char* key;

    std::clock_t time1, time2, sum = static_cast<std::clock_t>(0);
    for (i = 0; i < nReads; ++i) {
        key = ptrStore->getRandomKey(bitsKey, bitsKeyReal);

        time1 = clock();
        ptrStore->read(key, bitsKeyReal);
        time2 = clock();
        sum = sum + time2 - time1;

        std::cout << i << " ";

        delete[] key;
    }

    sum = static_cast<std::clock_t>(static_cast<float>(sum) / static_cast<float>(CLOCKS_PER_SEC));
    fileResults << sum << '\n';
    fileResults.close();

    return static_cast<unsigned int>(sum);
}

// Тест на скорость записи
unsigned int Tester::overwriteTest(Store* ptrStore, int nWrites, int bitsKey, int bitsValue) {
    std::fstream fileResults;
    std::string fileResultsName = "resultsOverwrite";
    fileResults.open(fileResultsName.c_str(), std::ios_base::out | std::ios_base::app);

    int i, bitsKeyReal, randBitsValue = rand() % (bitsValue - 99) + 100;
    char* key;
    char* value;

    std::clock_t time1, time2, sum = static_cast<std::clock_t>(0);
    for (i = 0; i < nWrites; ++i) {
        key = ptrStore->getRandomKey(bitsKey, bitsKeyReal);
        value = getRandomBytes(randBitsValue);

        time1 = clock();
        ptrStore->write(key, value, bitsKeyReal, randBitsValue);
        time2 = clock();
        sum = sum + time2 - time1;

        std::cout << i << " ";

        delete[] key;
        delete[] value;
    }

    sum = static_cast<std::clock_t>(static_cast<float>(sum) / static_cast<float>(CLOCKS_PER_SEC));
    fileResults << sum << '\n';
    fileResults.close();

    return static_cast<unsigned int>(sum);
}

// Получение рандомного набора байтов, заданной длины
char* Tester::getRandomBytes(int length)
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
