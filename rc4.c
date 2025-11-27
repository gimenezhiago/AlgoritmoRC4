#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ksa(unsigned char *key, int keylen, unsigned char S[256]) {
    int i, j = 0;
    for (i = 0; i < 256; i++) {
        S[i] = i;
    }
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keylen]) % 256;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

void prga(unsigned char S[256], unsigned char *data, int data_len, unsigned char *output) {
    int i = 0, j = 0, k, idx;

    for (k = 0; k < data_len; k++) {
        i = (i + 1) % 0xFF;
        j = (j + S[i]) % 0xFF;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        idx = (S[i] + S[j]) % 0xFF;
        output[k] = data[k] ^ S[idx];
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <chave>\n", argv[0]);
        return 1;
    }

    unsigned char *key = (unsigned char *)argv[1];
    int keylen = strlen((char *)key);
    
    char input[1024]; 
    printf("Digite o texto para cifrar/decifrar: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = 0;

    int len = strlen(input);
    unsigned char S[256];
    unsigned char output[1024];

    ksa(key, keylen, S);
    prga(S, (unsigned char *)input, len, output);

    printf("Resultado (HEX): ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", output[i]); 
    }

    printf("\nResultado (bytes): ");
    for (int i = 0; i < len; i++) {
        printf("\\x%02X", output[i]); 
    }

    printf("\n");
    return 0;

}