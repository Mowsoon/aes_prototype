#include "aes.h"

void aes128();



void generate_aes_key_struct(AES_KEY* key, size_t key_len) {
    if (key_len != 128 && key_len != 192 && key_len != 256) {
        printf("Key size must be 128 or 192 or 256\n");
        exit(1);
    }
    //converting size into bytes
    key_len /= 8;
    key->key_size = key_len;
    for (size_t i = 0; i < key_len; i++) {
        key->key[i] = rand() % 256;
    }
}


void aes(AES_KEY* key) {
    switch (key->key_size) {
        case 16:
            aes128(key);
        case 24:
            printf("192\n");
        case 32:
            printf("256\n");
        default:
            printf("Key is NULL\n");
            exit(1);
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

void gen_word128(AES_KEY* key, uint32_t words[KEYS_128]) {
    int i;

    for (i = 0; i < 4; i++) {
        words[i] = (key->key[i * 4] << 24) | (key->key[i * 4 + 1] << 16) |
                   (key->key[i * 4 + 2] << 8) | key->key[i * 4 + 3];
    }

    for (i = 4 ; i < KEYS_128; i++) {
        if (i % 4 == 0) {
            words[i] = words[i - 4] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/4];
        }
        else {
            words[i] = words[i - 4] ^ words[i - 1];
        }
    }
}

void aes128(AES_KEY* key) {
    uint32_t words[KEYS_128];
    gen_word128(key, words);

    for (int i = 0; i < KEYS_128; i++) {
        printf("Word %d: %08x\n", i, words[i]);
    }
}