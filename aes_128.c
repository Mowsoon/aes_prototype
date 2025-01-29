#include "aes_128.h"


void init_key_128(const uint32_t words[WORDS_128], uint32_t round_key[KEYS_128][KEY_SIZE]) {
    for (int i = 0; i < KEYS_128; i++) {
        for (int j = 0; j < KEY_SIZE; j++) {
            round_key[i][j] = words[i * KEY_SIZE + j];
        }
    }
}

void gen_keys128(AES_KEY* key, uint32_t round_key[KEYS_128][KEY_SIZE]) {
    int i;
    uint32_t words[WORDS_128];
    for (i = 0; i < 4; i++) {
        words[i] = (key->key[i * 4] << 24) | (key->key[i * 4 + 1] << 16) |
                   (key->key[i * 4 + 2] << 8) | key->key[i * 4 + 3];
    }

    for (i = 4 ; i < WORDS_128; i++) {
        if (i % 4 == 0) {
            words[i] = words[i - 4] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/4];
        }
        else {
            words[i] = words[i - 4] ^ words[i - 1];
        }
    }
    init_key_128(words, round_key);

}


void aes128(AES_KEY* key) {
    uint32_t round_key[KEYS_128][KEY_SIZE];
    gen_keys128(key, round_key);
}