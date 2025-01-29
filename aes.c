#include "aes.h"


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

void init_key(const uint32_t words[WORDS_128], uint32_t round_key[KEYS_128][KEY_SIZE]) {
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
    init_key(words, round_key);

}



void aes128(AES_KEY* key) {
    uint32_t round_key[KEYS_128][KEY_SIZE];
    gen_keys128(key, round_key);

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