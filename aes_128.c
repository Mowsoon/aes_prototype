#include "aes_128.h"


void init_key_128(const uint32_t words[WORDS_128], uint32_t round_key[KEYS_128][KEY_SIZE]) {
    for (int i = 0; i < KEYS_128; i++) {
        for (int j = 0; j < KEY_SIZE; j++) {
            round_key[i][j] = words[i * KEY_SIZE + j];
        }
    }
}

uint32_t rotate_left(uint32_t word) {
    return (word << 8) | (word >> 24);
}

uint32_t sub_word(uint32_t word) {
    uint8_t *byte = (uint8_t *)&word;
    for (int i = 0; i < 4; i++) {
        byte[i] = SBOX[byte[i]];
    }
    return word;
}


void gen_words128(AES_KEY* key, uint32_t words[WORDS_128]) {
    int i;
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
}

void gen_keys128(AES_KEY* key, uint32_t round_key[KEYS_128][KEY_SIZE]) {
    uint32_t words[WORDS_128];
    gen_words128(key, words);
    init_key_128(words, round_key);
}


void aes128(AES_KEY* key) {
    uint32_t round_key[KEYS_128][KEY_SIZE];
    gen_keys128(key, round_key);

    printf("%02x\n", round_key[0][0]);
}