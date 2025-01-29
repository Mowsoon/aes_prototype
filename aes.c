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

void aes128(AES_KEY* key) {
    uint32_t words[KEYS_128];
    int i;

    for (i = 0; i < 4; i++) {
        words[i] = (key->key[i * 4] << 24) | (key->key[i * 4 + 1] << 16) |
                   (key->key[i * 4 + 2] << 8) | key->key[i * 4 + 3];
    }


}