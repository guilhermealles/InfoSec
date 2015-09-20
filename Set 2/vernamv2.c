#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vernam(char *text, char *key, char *out_cipher);
int printableText(char *text, int size);

char *text;
char *key;
int text_size;

int main (int argc, char **argv) {
    arg_c = argc;
    arg_v = argv;

    text = argv[1];
    key = argv[2];
    //We'll use the length of the key to avoid problems with characters that represent '\0'
    text_size = strlen(key);

    char *cipher_text = malloc(sizeof(char) * (text_size+1));

    vernam(text, key, cipher_text);
    if (printableText(cipher_text, text_size)) {
        printf("Output is printable! Result: %s\n", cipher_text);
    }
    else {
        printf("Output is not printable! Result:\n");
        int i;
        for (i=0; i<text_size; i++) {
            printf("%d ", (int)cipher_text[i]);
        }
        printf("\n");
    }

    free(cipher_text);

    return 0;
}

void vernam(char *text, char *key, char *out_cipher) {
    int size = (int)strlen(key);
    out_cipher[size] = '\0';

    int i;
    for(i=0; i<size; i++) {
        int text_char = (int)text[i];
        int key_char = (int)key[i];
        int out_char = text_char ^ key_char;
        out_cipher[i] = (char)out_char;
    }
}

int printableText(char *text, int size) {
    int i;
    // Loops until size-1 (last character is non printable)
    for(i=0; i<size-1; i++) {
        if ((int)text[i] < 33 || (int)text[i] > 126) {
            return 0;
        }
    }
    return 1;
}
