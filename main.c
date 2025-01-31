
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

    //aes(&aes_key, block);
    return 0;
}
