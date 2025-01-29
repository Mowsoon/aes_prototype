
#include "aes.h"

int main() {

    AES_KEY aes_key;
    generate_aes_key_struct(&aes_key, 128);

    printf("Key size (bytes): %zu\n", aes_key.key_size);

    for (int i = 0; i < aes_key.key_size; i++) {
        printf("%04x ", aes_key.key[i]);
    }

    return 0;
}
