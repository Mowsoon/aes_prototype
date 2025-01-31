#include "aes.h"

// Function that generate a random 256bits key
void generate_aes_key(AES_KEY* key) {
    key->key_size = KEY_SIZE;
    for (size_t i = 0; i < KEY_SIZE; i++) {
        key->key[i] = rand() % 256;
    }
}

//Function that rotate a word of 32bits from one byte to the left
uint32_t rotate_left(uint32_t word) {
    return (word << 8) | (word >> 24);
}

// function that take a word of 32bits in the Sbox and return the new word
uint32_t sub_word(uint32_t word) {
    uint8_t *byte = (uint8_t *)&word;
    for (int i = 0; i < 4; i++) {
        byte[i] = SBOX[byte[i]];
    }
    return word;
}

// function that take a block of 32bits in the Sbox and return the new block
void sub_block(uint32_t block[BLOCK_ELEMENT]) {
    for (int i = 0; i < BLOCK_ELEMENT; i++)
    {
        block[i] = sub_word(block[i]);
    }
}

// function that do the transposition on each row of a block
void transpose_block(uint32_t block[BLOCK_ELEMENT]) {
    block[1] = (block[1] << 8) | (block[1] >> 24);
    block[2] = (block[2] << 16) | (block[2] >> 16);
    block[3] = (block[3] << 24) | (block[3] >> 8);
}

// function that xor the block with a specific key
void combine_block(uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT], uint32_t block [BLOCK_ELEMENT], int key_index) {
    for (int i = 0; i < BLOCK_ELEMENT; i++) {
        block[i] = block[i] ^ round_key[key_index][i];
    }
}

// function that do the multiplication between bytes
uint8_t galois_mul(const uint8_t a, const uint8_t b) {
    if(b == 0x01) return a;
    if(b == 0x02) return (a << 1) ^ ((a & 0x80) ? 0x1B : 0x00);
    return galois_mul(a, 0x02) ^ a;
}

// function that do the matrix multiplication for a column of the block with a row of the mix matrix
// return the result which will be a bytes of the new block
uint8_t matrix_mul(const uint8_t a[BLOCK_ELEMENT][BLOCK_ELEMENT], const int colums, const int shift) {
    return galois_mul(a[colums][0], MIX_MATRIX[shift * BLOCK_ELEMENT]) ^
                        galois_mul(a[colums][1], MIX_MATRIX[shift * BLOCK_ELEMENT + 1]) ^
                            galois_mul(a[colums][2], MIX_MATRIX[shift * BLOCK_ELEMENT + 2]) ^
                                galois_mul(a[colums][3], MIX_MATRIX[shift * BLOCK_ELEMENT + 3]);
}

// function that return a row of matrix multiplication by concatenating every result
uint32_t matrix_concatenation(const uint8_t blockInByte[BLOCK_ELEMENT][BLOCK_ELEMENT], const int shift) {
    return  ((uint32_t)matrix_mul(blockInByte,0, shift) << 24) |
                ((uint32_t)matrix_mul(blockInByte,1, shift) << 16) |
                    ((uint32_t)matrix_mul(blockInByte,2, shift) << 8) |
                        matrix_mul(blockInByte,3, shift);

}

// function that get every byte in a block
void bloc_into_bytes(const uint32_t block[BLOCK_ELEMENT], uint8_t blockInByte[BLOCK_ELEMENT][BLOCK_ELEMENT]) {

    for (int i = 0; i < BLOCK_ELEMENT; i++) {
        blockInByte[0][i]   = block[i] >> 24;
        blockInByte[1][i]   = block[i] >> 16;
        blockInByte[2][i]   = block[i] >> 8;
        blockInByte[3][i]   = block[i];
    }
}

// function that finds the result of a mixing operation for a block
void bytes_into_block(uint32_t block [BLOCK_ELEMENT], const uint8_t blockInByte[BLOCK_ELEMENT][BLOCK_ELEMENT]) {
    block[0] = matrix_concatenation(blockInByte, 0);
    block[1] = matrix_concatenation(blockInByte, 1);
    block[2] = matrix_concatenation(blockInByte, 2);
    block[3] = matrix_concatenation(blockInByte,3);
}

// function doing the mixing operation
void mixing_block(uint32_t block [BLOCK_ELEMENT]) {
    uint8_t blockInByte[BLOCK_ELEMENT][BLOCK_ELEMENT];
    bloc_into_bytes(block, blockInByte);
    bytes_into_block(block, blockInByte);
}

// function that find the first word that are all the bytes in the original key
void initiate_starting_words(AES_KEY key, uint32_t words[NUMBER_WORDS]) {
    for (int i = 0; i < KEY_WORDS_RATIO; i++) {
        words[i] = (key.key[i * WORD_SIZE] << 24) | (key.key[i * WORD_SIZE + 1] << 16) |
                   (key.key[i * WORD_SIZE + 2] << 8) | key.key[i * WORD_SIZE + 3];
    }
}

// function that generates all the words needed for the round keys
void gen_words(AES_KEY key, uint32_t words[NUMBER_WORDS]) {
    initiate_starting_words(key, words);

    for (int i = KEY_WORDS_RATIO; i < NUMBER_WORDS; i++) {
        if ((i % KEY_WORDS_RATIO) == 0) {
            words[i] = words[i - KEY_WORDS_RATIO] ^ sub_word(rotate_left(words[i - 1])) ^ RCON[i/KEY_WORDS_RATIO];
        } else {
            words[i] = words[i - KEY_WORDS_RATIO] ^ words[i - 1];
        }
    }
}

// function that initiate the round keys by taking every words 4 by 4, for every round key
// the first round key is the initial key
void init_round_keys(const uint32_t words[NUMBER_WORDS], uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT]) {
    for (int i = 0; i < NUMBER_KEYS; i++) {
        for (int j = 0; j < BLOCK_ELEMENT; j++) {
            round_key[i][j] = words[i * WORD_SIZE + j];
        }
    }
}

// function that generates the word then initiate the keys
void gen_round_keys(AES_KEY key, uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT]) {
    uint32_t words[NUMBER_WORDS];
    gen_words(key, words);
    init_round_keys(words, round_key);
}


// function that do every step of the encryption of AES
void crypting(uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT], uint32_t block [BLOCK_ELEMENT]) {
    // Initial round key addition
    combine_block(round_key, block, 0);
    // Main rounds (1 to NUMBER_KEYS-2)
    for (int i = 1; i < NUMBER_KEYS - 1; i++) {
        sub_block(block);
        transpose_block(block);
        mixing_block(block);
        combine_block(round_key, block, i);
    }
    // Final round (no MixColumns)
    sub_block(block);
    transpose_block(block);
    combine_block(round_key, block, NUMBER_KEYS - 1);
}

// function that generates the round keys then encrypt the block
void aes_encrypt(AES_KEY key, uint32_t block [BLOCK_ELEMENT]) {
    uint32_t round_key[NUMBER_KEYS][BLOCK_ELEMENT];
    gen_round_keys(key, round_key);
    crypting(round_key, block);
}
