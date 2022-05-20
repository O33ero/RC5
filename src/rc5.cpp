#include "lib/rc5.h"

using namespace std;

rc5::rc5(uint8_t key[]) {

    machine_word = W >> 3;
    L_size = b % machine_word > 0 ? b / machine_word + 1 : b / machine_word;
    L = new uint64_t[L_size];

    for (int i = b - 1; i >= 0; i--)
        L[i / machine_word] = rotl(L[i / machine_word], 8) + (uint64_t)key[i];

    table_size = 2 * (R + 1);
    S = new uint64_t[table_size];
    S[0] = P64;

    for (int i = 1; i < table_size; i++)
        S[i] = S[i - 1] + Q64;

    uint64_t x = 0, y = 0;
    int i = 0, j = 0, n;

    n = 3 * get_max(table_size, L_size);

    for (int k = 0; k < n; k++) {
        x = S[i] = rotl((S[i] + x + y), 3);
        y = L[j] = rotl((L[j] + x + y), (int)(x + y));
        i = (i + 1) % table_size;
        j = (j + 1) % L_size;
    }
}

int rc5::get_max(int a, int b) {
    return a > b ? a : b;
}

uint64_t rc5::rotl(uint64_t a, int n) {
    return (a << n | a >> (W - n));
}

uint64_t rc5::rotr(uint64_t a, int n) {
    return (a >> n | a << (W - n));
}

uint64_t rc5::bytes_to_uint64(uint8_t b[], int p) {
    uint64_t r = 0;
    for (int i = p + 7; i > p; i--) {
        r |= (uint64_t)b[i];
        r <<= 8;
    }
    r |= (uint64_t)b[p];
    return r;
}

void rc5::uint64_to_bytes(uint64_t a, uint8_t b[], int p) {
    for (int i = 0; i < 7; i++) {
        b[p + i] = (uint8_t)(a & 0xFF);
        a >>= 8;
    }
    b[p + 7] = (uint8_t)(a & 0xFF);
}

void rc5::encrypt(uint8_t inBuf[], uint8_t outBuf[]) {
    uint64_t A = bytes_to_uint64(inBuf, 0);
    uint64_t B = bytes_to_uint64(inBuf, 8);

    A = A + S[0];
    B = B + S[1];

    for (int i = 1; i < R + 1; i++) {
        A = rotl((A ^ B), B) + S[2 * i];
        B = rotl((B ^ A), A) + S[2 * i + 1];
    }

    uint64_to_bytes(A, outBuf, 0);
    uint64_to_bytes(B, outBuf, 8);
}

void rc5::decrypt(uint8_t inBuf[], uint8_t outBuf[]) {
    uint64_t A = bytes_to_uint64(inBuf, 0);
    uint64_t B = bytes_to_uint64(inBuf, 8);

    for (int i = R; i > 0; i--) {
        B = rotr((B - S[2 * i + 1]), A) ^ A;
        A = rotr((A - S[2 * i]), B) ^ B;
    }

    A = A - S[0];
    B = B - S[1];

    uint64_to_bytes(A, outBuf, 0);
    uint64_to_bytes(B, outBuf, 8);
}