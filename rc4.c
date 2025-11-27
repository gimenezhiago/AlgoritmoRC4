#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void ksa(unsigned char *key, int keylen, unsigned char S[256]) {
    int i, j = 0;
    for (i = 0; i < 256; i++) S[i] = i;

    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keylen]) & 0xFF;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

void prga(unsigned char S[256], unsigned char *data, int len, unsigned char *out) {
    int i = 0, j = 0, k;
    for (k = 0; k < len; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        int t = (S[i] + S[j]) & 0xFF;
        unsigned char K = S[t];
        out[k] = data[k] ^ K;
    }
}

int hex_to_byte(char a, char b) {
    char s[3] = {a, b, 0};
    return (int)strtol(s, NULL, 16);
}

int parse_hex_format1(const char *input, unsigned char *out) {
    // formato \xAA\xBB\xCC
    if (strncmp(input, "\\x", 2) != 0) return -1;

    int count = 0;
    const char *p = input;

    while (*p) {
        if (*p == '\\' && *(p+1) == 'x') {
            if (!isxdigit(*(p+2)) || !isxdigit(*(p+3)))
                return -1;
            out[count++] = hex_to_byte(*(p+2), *(p+3));
            p += 4;
        } else return -1;
    }

    return count;
}

int parse_hex_format2(const char *input, unsigned char *out) {
    // formato "AA BB CC"
    int len = 0;
    char copy[1024];
    strcpy(copy, input);

    char *tok = strtok(copy, " ");
    while (tok) {
        if (strlen(tok) != 2) return -1;
        if (!isxdigit(tok[0]) || !isxdigit(tok[1])) return -1;

        out[len++] = hex_to_byte(tok[0], tok[1]);
        tok = strtok(NULL, " ");
    }
    return len;
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

    unsigned char data[1024];
    int data_len = 0;
    int is_hex_input = 0;

    // Tenta formato \xAA\xBB\xCC
    data_len = parse_hex_format1(input, data);
    if (data_len > 0) {
        is_hex_input = 1;
    } else {
        // Tenta formato AA BB CC
        data_len = parse_hex_format2(input, data);
        if (data_len > 0) {
            is_hex_input = 1;
        } else {
            // Texto normal
            strcpy((char *)data, input);
            data_len = strlen(input);
        }
    }

    unsigned char S[256];
    unsigned char output[1024];
    ksa(key, keylen, S);
    prga(S, data, data_len, output);

    if (is_hex_input) {
        printf("\nTexto decifrado: ");
        fwrite(output, 1, data_len, stdout);
        printf("\n");
    }

    printf("\nResultado (HEX): ");
    for (int i = 0; i < data_len; i++)
        printf("%02X ", output[i]);

    printf("\nResultado (bytes): ");
    for (int i = 0; i < data_len; i++)
        printf("\\x%02X", output[i]);

    printf("\n");
    return 0;
}
