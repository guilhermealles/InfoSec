#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int parseParameters(int argc, char **argv, int *decrypt_option, int *shift_option);
int encryptShift(char *inText, int shift, char *outText);
int decryptShift(char *inText, int shift, char *outText);
int encryptMapping(char *inText, char *mapping, char *outText);
int decryptMapping(char *inText, char *mapping, char *outText);
void showAmbiguousMappingMessage();

int main (int argc, char **argv) {
    int decrypt_option = 0;
    // If use_shift != 0, then a mapping is provided. Otherwise, this is the shift to be used.
    int use_shift = 0;
    int message_index;
        if (!parseParameters(argc, argv, &decrypt_option, &use_shift)) {
        puts("Error parsing parameters!");
        return 0;
    }
    else {
        message_index = (decrypt_option == 0) ? 2 : 3;
    }

    char *output_message = malloc(sizeof(char) * ((int)strlen(argv[message_index]) + 1));
    if (use_shift) {
        if (decrypt_option) {
            printf("Decrypting \"%s\" with shift %d.\n", argv[message_index], use_shift);
            decryptShift(argv[message_index], use_shift, output_message);
        }
        else {
            printf("Encrypting \"%s\" with shift %d.\n", argv[message_index], use_shift);
            encryptShift(argv[message_index], use_shift, output_message);
        }
    }
    else {
        if (decrypt_option) {
            printf("Decrypting \"%s\" with mapping \"%s\".\n", argv[message_index], argv[2]);
            decryptMapping(argv[message_index], argv[2], output_message);
        }
        else {
            printf("Encrypting \"%s\" with mapping \"%s\".\n", argv[message_index], argv[1]);
            encryptMapping(argv[message_index], argv[1], output_message);
        }
    }
    printf("Output: %s\n", output_message);

    free(output_message);
    return 0;
}

int parseParameters(int argc, char **argv, int *decrypt_option, int *shift_option) {
    if (argc < 3 || argc > 4) {
        printf("\nUsage: %s [-d] mapping\n", argv[0]);
        printf("Where:\n");
        printf("  -d: decrypt");
        printf("  mapping: 26 letter char-mapping\n  or an int-value\n\n");
        printf("En/Decrypts stdin to stdout. Only letters are encrypted,\n");
        printf("all other characters are silently ignored.\n");
        printf("Use an int-value to do a letter shift (%% 26, 0: a = a)\n");
        printf("Shift 3 is the classical Caesar encryption.\n\n");
    }
    else {
        // Check -d option
        if (strcmp(argv[1], "-d") == 0) {
            *decrypt_option = 1;
        }
        else {
            *decrypt_option = 0;
        }

        // Check for mapping or shift
        if (*decrypt_option) {
            if (strlen(argv[2]) == (int)26) {
                *shift_option = 0;
            }
            else {
                long int shift = strtol(argv[2], NULL, 10);
                if (shift == 0) {
                    return 0;
                }
                else {
                    *shift_option = (int)shift;
                }
            }
        }
        else {
            if (strlen(argv[1]) == (int)26) {
                *shift_option = 0;
            }
            else {
                long int shift = strtol(argv[1], NULL, 10);
                if (shift == 0) {
                    return 0;
                }
                else {
                    *shift_option = (int)shift;
                }
            }
        }
    }
    return 1;
}

// Encrypts a message using the shift provided as argument. Before encrypting, the message
// is converted to lower case.
int encryptShift(char *inText, int shift, char *outText) {
    int i = 0;
    char currentChar;
    while (inText[i] != '\0') {
        // Converts to lowercase
        currentChar = tolower(inText[i]);

        // Shift from the ASCII code ('a' becomes 0, 'b' becomes 1, ...)
        currentChar -= 97;

        // Apply the shift
        currentChar = (currentChar + shift) % 26;

        // Reshift char to ASCII code
        currentChar += 97;

        // Puts the result in the output stream
        outText[i] = currentChar;

        i++;
    }
    outText[i+1] = '\0';
    return 0;
}

// Decrypts a message using the shift provided as argument. Before decrypting, the message
// is converted to lower case.
int decryptShift(char *inText, int shift, char *outText) {
    int i = 0;
    char currentChar;
    while (inText[i] != '\0') {
        // Converts to lowercase
        currentChar = tolower(inText[i]);

        // Shift from the ASCII code ('a' becomes 0, 'b' becomes 1, ...)
        currentChar -= 97;

        // Apply the shift
        currentChar = ((currentChar - shift) + 26) % 26;

        // Reshift char to ASCII code
        currentChar += 97;

        // Puts the result in the output stream
        outText[i] = currentChar;

        i++;
    }
    //Finish output string
    outText[i+1] = '\0';
    return 0;
}

int encryptMapping(char *inText, char *mapping, char *outText) {
    //Let's consider each character in the inText as an index to the mapping array
    int i = 0;
    char currentChar;
    while (inText[i] != '\0') {
        // Converts to lowercase
        currentChar = tolower(inText[i]);

        // Shift from the ASCII code
        currentChar -= 97;

        // Encrypt text with the mapping
        outText[i] = mapping[(int)currentChar];

        i++;
    }
    // Finish output string
    outText[i+1] = '\0';
    return 0;
}

int decryptMapping(char *inText, char *mapping, char *outText) {
    // i will be used to loop through the inText, j will be used to loop through the mapping
    int i=0;
    char currentChar;
    while (inText[i] != '\0') {
        currentChar = tolower(inText[i]);
        // Search the mapping to find the index of the character
        int j = 0, found = 0, mappingIndex = -1;
        while (mapping[j] != '\0') {
            char currentMappingChar = tolower(mapping[j]);
            if (currentMappingChar == currentChar) {
                if (found == 1) {
                    showAmbiguousMappingMessage();
                    return 1;
                }
                else {
                    mappingIndex = j;
                    found = 1;
                }
            }
            j++;
        }
        if (mappingIndex < 0) {
            showAmbiguousMappingMessage();
            return 1;
        }
        else {
            outText[i] = mappingIndex + 97;
        }
        i++;
    }
    return 0;
}

void showAmbiguousMappingMessage() {
    puts("Impossible to decrypt! Mapping is ambiguous.");
}
