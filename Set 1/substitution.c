#include <stdio.h>

int main (int argc, char **argv) {
    if (argc < 2) {
        printf("\nUsage: %s [-d] mapping\n", argv[0]);
        printf("Where:\n");
        printf("  -d: decrypt");
        printf("  mapping: 26 letter char-mapping\n  or an int-value\n\n");
        printf("En/Decrypts stdin to stdout. Only letters are encrypted,\n");
        printf("all other characters are silently ignored.\n");
        printf("Use an int-value to do a letter shift (%% 26, 0: a = a)\n");
        printf("Shift 3 is the classical Caesar encryption.\n\n");
    }

    return 0;
}
