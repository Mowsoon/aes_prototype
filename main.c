
#include "aes.h"

int main() {


    AES_KEY aes_key;
    generate_aes_key(&aes_key, 128);

    for (int i = 0; i < aes_key.key_size; i++) {
        printf("%02x ", aes_key.key[i]);
    }
    printf("\n");

    aes(&aes_key);

    return 0;
}
