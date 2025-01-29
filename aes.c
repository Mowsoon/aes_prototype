#include "aes_128.h"

void generate_aes_key(AES_KEY* key, size_t key_len) {
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
        break;
        case 24:
            printf("192\n");
        break;
        case 32:
            printf("256\n");
        break;
        default:
            printf("Key is NULL\n");
        exit(1);
    }
}