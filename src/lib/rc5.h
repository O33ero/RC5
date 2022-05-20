#pragma once

#include <cstdint>

class rc5 // RC5-64/16/16
{
private:
    const int W = 64;                            // половина длины блока в битах (рекомендуется размер машинного слова)
    const int R = 16;                            // число раундов
    const int b = 16;                            // длина ключа в байтах

    const uint64_t P64 = 0xB7E151628AED2A6B;     // 64-битная константа
    const uint64_t Q64 = 0x9E3779B97F4A7C15;     // 64-битная константа

    uint64_t* L;                                 // массив для ключа пользователя
    uint64_t* S;                                 // таблица расширенных ключей
    int table_size;                              // размер таблицы
    int machine_word;                            // кол-во байтов в одном машинном слове
    int L_size;                                  // размер массива L


private:
    int get_max(int a, int b);

    uint64_t rotl(uint64_t a, int n);

    uint64_t rotr(uint64_t a, int n);

    // Свертка 64 бит по 8-ми байтам
    uint64_t bytes_to_uint64(uint8_t b[], int p);

    // Развертка 64 бит по 8-ми байтам
    void uint64_to_bytes(uint64_t a, uint8_t b[], int p);

public:
    rc5(uint8_t key[]);

    void encrypt(uint8_t inBuf[], uint8_t outBuf[]);

    void decrypt(uint8_t inBuf[], uint8_t outBuf[]);

};