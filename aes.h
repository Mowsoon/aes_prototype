#ifndef AES_H
#define AES_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    uint8_t key[32];
    size_t key_size;
} AES_KEY;



void generate_aes_key_struct(AES_KEY *key, size_t key_len);

#endif
