
#include "aes.h"

int main() {

    AES_KEY key;
    generate_aes_key_struct(&key, 128);

    printf("Key size (bytes): %zu\n", key.key_size);
}
