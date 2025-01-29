#include "aes.h"


// --------------------------- general function -----------------------------

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

void aes(AES_KEY* key) {
    switch (key->key_size) {
        case 16:
            break;
        case 24:
            break;
        case 32:
            //#include "aes_256.h"
                break;
        default:
            printf("Key is NULL\n");
        exit(1);
    }
}


//--------------------------- AES 128 function -----------------------------

void init_key_128(const uint32_t words[NUMBER_WORDS_128], uint32_t round_key[NUMBER_KEYS_128][KEY_SIZE_128]) {
    for (int i = 0; i < NUMBER_KEYS_128; i++) {
        for (int j = 0; j < KEY_SIZE_128; j++) {
            round_key[i][j] = words[i * KEY_SIZE_128 + j];
        }
    }
}

void gen_words128(AES_KEY* key, uint32_t words[NUMBER_WORDS_128]) {
    int i;
    for (i = 0; i < 4; i++) {
        words[i] = (key->key[i * KEY_SIZE_128] << 24) | (key->key[i * KEY_SIZE_128 + 1] << 16) |
                   (key->key[i * KEY_SIZE_128 + 2] << 8) | key->key[i * KEY_SIZE_128 + 3];
    }

    for (i = KEY_SIZE_128 ; i < NUMBER_WORDS_128; i++) {
        if (i % KEY_SIZE_128 == 0) {
            words[i] = words[i - KEY_SIZE_128] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/KEY_SIZE_128];
        }
        else {
            words[i] = words[i - KEY_SIZE_128] ^ words[i - 1];
        }
    }
}

void gen_keys128(AES_KEY* key, uint32_t round_key[NUMBER_KEYS_128][KEY_SIZE_128]) {
    uint32_t words[NUMBER_WORDS_128];
    gen_words128(key, words);
    init_key_128(words, round_key);
}

void aes_128(AES_KEY* key) {
    uint32_t round_key[NUMBER_KEYS_128][KEY_SIZE_128];
    gen_keys128(key, round_key);

    printf("%02x\n", round_key[0][0]);
}

//--------------------------- AES 192 function -----------------------------

void init_key_192(const uint32_t words[NUMBER_WORDS_192], uint32_t round_key[NUMBER_KEYS_192][KEY_SIZE_192]) {
    for (int i = 0; i < NUMBER_KEYS_192; i++) {
        for (int j = 0; j < KEY_SIZE_192; j++) {
            round_key[i][j] = words[i * KEY_SIZE_192+ j];
        }
    }
}
