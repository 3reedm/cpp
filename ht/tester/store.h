#ifndef STORE_H
#define STORE_H
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct Store
{
    virtual ~Store(){}                                                                // Деструктор
    virtual bool write(char* ptrKey, char* ptrValue, int bitsKey, int bitsValue) = 0; // Запись данных
    virtual bool read(char* ptrKey, int bitsKey) = 0;                                 // Чтение данных
    virtual char* getRandomKey(int bitsKey, int& bitsKeyReal) = 0;                    // Получение рандомного ключа
};
///////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // STORE_H
