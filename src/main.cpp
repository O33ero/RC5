#include "lib/rc5.h"

#include <iostream>
#include <cstdint>

using namespace std;

#define PRINT(ARR, N)				\
for(int i = 0; i < N; i++) {		\
	printf("0x%02x ", ARR[i]);		\
}									\
printf("| ");						\
for(int i = 0; i < N; i++) {		\
	printf("%c ", ARR[i]);			\
}									\
cout << endl;


uint8_t* get_slice(uint8_t src[], int from, int to) {
	uint8_t* result = new uint8_t[to - from + 1];
	for (int i = from, j = 0; i <= to; i++, j++) {
		result[j] = src[i];
	}
	return result;
}

void set_zero(uint8_t arr[], int last_index) {
	for (int i = last_index + 1; i < 16; i++) {
		arr[i] = 0x00;
	}
}

int main() {

	// User varible
	uint8_t OPEN[] = { // Открытый текст (любой размер)
		'A', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'0', 'B', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'0', '1', 'C', '3', '4', '5', '6', '7', '8', '9', '0',
	};

	uint8_t KEY[] = { // Ключ (не больше 16 байт)
		'1', '2', '3', '4', '5', '6', '7', '8', '7', '6', '5', '4', '3', '2', '1', '0',
	};
	// * * *


	// Core program
	int datasize = sizeof(OPEN) / sizeof(*OPEN);
	int keysize = sizeof(KEY) / sizeof(*KEY);


	if (keysize > 16)
		throw length_error("Key length should be less then 16 bytes");
	if (keysize < 16)
		set_zero(KEY, sizeof(KEY) / sizeof(*KEY));

	rc5 r(KEY);
	int count_of_blocks = datasize / 16 + 1;
	int tsize = datasize;
	int from = 0;
	int to = datasize > 16 ? 15 : datasize - 1;
	for (int i = 0; i < count_of_blocks && tsize > 0; i++) {
		tsize -= 16; // вычитаем размер блока

		uint8_t* TEMP = get_slice(OPEN, from, to);
		if (to - from + 1 < 16)
			set_zero(TEMP, to - from);

		uint8_t CRYPT[16];
		uint8_t DECRYPT[16];

		cout << "Start crypting block..." << endl;

		cout << "Source:      ";
		PRINT(TEMP, 16)

			r.encrypt(TEMP, CRYPT);
		cout << "Crypted:     ";
		PRINT(CRYPT, 16)

			r.decrypt(CRYPT, DECRYPT);
		cout << "Decrypted:   ";
		PRINT(DECRYPT, 16)

			cout << "End crypting." << endl << endl;

		if (tsize < 16) {
			from += 16;
			to = datasize - 1;
		}
		else {
			from += 16;
			to += 16;
		}
	}
	// * * *
	return 0;
}