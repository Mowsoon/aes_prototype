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




//--------------------------- AES 128 function -----------------------------

void init_key_128(const uint32_t* words, uint32_t** round_key) {
    for (int i = 0; i < NUMBER_KEYS_128; i++) {
        for (int j = 0; j < KEY_SIZE_128; j++) {
            round_key[i][j] = words[i * KEY_SIZE_128 + j];
        }
    }
}

void gen_words(const AES_KEY* key, uint32_t * words) {
    int key_size;
    switch (key->key_size) {
        case 16:
            key_size = KEY_SIZE_128;
        break;

        case 24:
            key_size = KEY_SIZE_192;
        break;

        case 32:
            key_size = KEY_SIZE_256;
        break;

        default:
            printf("Key Size 8: Invalid Key Size\n");
        exit(1);
    }
    int i;
    for (i = 0; i < 4; i++) {
        words[i] = (key->key[i * key_size] << 24) | (key->key[i * key_size + 1] << 16) |
                   (key->key[i * key_size + 2] << 8) | key->key[i * key_size + 3];
    }

    for (i = key_size ; i < NUMBER_WORDS_128; i++) {
        if (i % key_size == 0) {
            words[i] = words[i - key_size] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/key_size];
        }
        else {
            words[i] = words[i - key_size] ^ words[i - 1];
        }
    }
}

void gen_keys_128(AES_KEY* key, uint32_t** round_key) {
    uint32_t words[NUMBER_WORDS_128];
    gen_words(key, words);
    init_key_128(words, round_key);
}

void aes(AES_KEY* key) {
    uint32_t round_key[NUMBER_KEYS][ROUND_KEY_SIZE];
    gen_keys_128(key, round_key);

    printf("%02x\n", round_key[0][0]);
}
