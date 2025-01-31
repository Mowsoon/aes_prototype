#include "aes.h"


void generate_aes_key(AES_KEY* key) {
    key->key_size = KEY_SIZE;
    for (size_t i = 0; i < KEY_SIZE; i++) {
        key->key[i] = rand() % 256;
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


void initiate_starting_words(AES_KEY* key, uint32_t words[NUMBER_WORDS]) {
    for (int i = 0; i < KEY_WORDS; i++) {
        words[i] = (key->key[i * WORD_SIZE] << 24) | (key->key[i * WORD_SIZE + 1] << 16) |
                   (key->key[i * WORD_SIZE + 2] << 8) | key->key[i * WORD_SIZE + 3];
    }
}

void gen_words(AES_KEY* key, uint32_t words[NUMBER_WORDS]) {
    initiate_starting_words(key, words);

    for (int i = KEY_WORDS; i < NUMBER_WORDS; i++) {
        if ((i % KEY_WORDS) == 0) {
            words[i] = words[i - KEY_WORDS] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/KEY_WORDS];
        } else {
            words[i] = words[i - KEY_WORDS] ^ words[i - 1];
        }
    }
}


void gen_keys_128(AES_KEY* key, uint32_t** round_key) {
    uint32_t words[NUMBER_WORDS];
}

void aes(AES_KEY* key) {
    uint32_t round_key[NUMBER_KEYS][ROUND_KEY_SIZE];
    gen_keys_128(key, round_key);

    printf("%02x\n", round_key[0][0]);
}
