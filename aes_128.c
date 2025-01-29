#include "aes_128.h"


void init_key_128(const uint32_t words[WORDS_128], uint32_t round_key[KEYS_128][KEY_SIZE]) {
    for (int i = 0; i < KEYS_128; i++) {
        for (int j = 0; j < KEY_SIZE; j++) {
            round_key[i][j] = words[i * KEY_SIZE + j];
        }
    }
}