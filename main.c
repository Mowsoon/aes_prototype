
#include <string.h>

#include "aes.h"

void print_block(uint32_t block[BLOCK_ELEMENT]) {
    for (int i = 0; i < BLOCK_ELEMENT; i++) {
        printf("%08X ", block[i]);
    }
    printf("\n");
}


int main() {


    AES_KEY aes_key;
    generate_aes_key(&aes_key);
    uint32_t block[4] = {
        0x87F24D97,
        0x6E4C90EC,
        0x46E74AC3,
        0xA68CD895
    };
    print_block(block);
    aes_encrypt(aes_key, block);
    print_block(block);

    return 0;
}
