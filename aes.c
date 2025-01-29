#include "aes.h"

void generate_aes_key_struct(AES_KEY* key, size_t key_len) {
    if (key_len != 128 && key_len != 192 && key_len != 256) {
        printf("Key size must be 128 or 192 or 256\n");
        exit(1);
    }
    key->key_size = key_len;
    for (size_t i = 0; i < key_len / 8; i++) {
        key->key[i] = rand() % 256;
    }
}
