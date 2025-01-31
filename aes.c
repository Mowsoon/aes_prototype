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

void sub_block(uint32_t block[BLOCK_ELEMENT]) {
    for (int i = 0; i < BLOCK_ELEMENT; i++)
    {
        block[i] = sub_word(block[i]);
    }
}


void initiate_starting_words(AES_KEY* key, uint32_t words[NUMBER_WORDS]) {
    for (int i = 0; i < KEY_WORDS_RATIO; i++) {
        words[i] = (key->key[i * WORD_SIZE] << 24) | (key->key[i * WORD_SIZE + 1] << 16) |
                   (key->key[i * WORD_SIZE + 2] << 8) | key->key[i * WORD_SIZE + 3];
    }
}

void gen_words(AES_KEY* key, uint32_t words[NUMBER_WORDS]) {
    initiate_starting_words(key, words);

    for (int i = KEY_WORDS_RATIO; i < NUMBER_WORDS; i++) {
        if ((i % KEY_WORDS_RATIO) == 0) {
            words[i] = words[i - KEY_WORDS_RATIO] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/KEY_WORDS_RATIO];
        } else {
            words[i] = words[i - KEY_WORDS_RATIO] ^ words[i - 1];
        }
    }
}

void init_round_keys(const uint32_t words[NUMBER_WORDS], uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT]) {
    for (int i = 0; i < NUMBER_KEYS; i++) {
        for (int j = 0; j < BLOCK_ELEMENT; j++) {
            round_key[i][j] = words[i * WORD_SIZE + j];
        }
    }
}

void gen_keys_128(AES_KEY* key, uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT]) {
    uint32_t words[NUMBER_WORDS];
    gen_words(key, words);
    init_round_keys(words, round_key);
}



void crypting(uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT], uint32_t block [BLOCK_ELEMENT]) {
    int i;
    for (i = 0; i < BLOCK_ELEMENT; i++) {
        block[i] = block[i] ^ round_key[0][i];
    }
    for (i = 1; i < NUMBER_KEYS - 1; i++) {
        sub_block(block);
    }

}

void aes(AES_KEY* key, uint32_t block [BLOCK_ELEMENT]) {
    uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT];
    gen_keys_128(key, round_key);
}
