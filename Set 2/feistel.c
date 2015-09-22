#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char byte;

int getSubkey(int round);
void populate_subkeys();
int f(int block, int subkey);
void feistelEncrypt();
void feistelDecrypt();

char input_text[] = {97, 109, 111, 114, -69, -29, 32, 12, '\0'};
//char* input_text;
int input_text_size;
char *passphrase;
char expanded_passphrase_1[] = "184b4d16bbe3200c5a5f500cc09efa68cddd42cbda27c1e49fa7a0f2e2735007";
char expanded_passphrase_2[] = "bd11fd28eabd0b87f2ff4595a50041bfb882bbf8ae058ea5d677c7da07d43786";
byte *subkeys;
char* output_text;

int main (int argc, char **argv) {
    //input_text = argv[1];
    passphrase = argv[2];
    input_text_size = strlen(input_text);
    output_text = malloc(sizeof(char)*input_text_size);

    subkeys = malloc(sizeof(byte) * 4 * 16);
    populate_subkeys();

    feistelDecrypt();
    for (int i = 0; i < input_text_size; i++) {
        printf("%d ", (char)output_text[i]);
    }
    printf("\n");

    free(subkeys);
    return 1;
}

// The least significant byte goes first
void populate_subkeys() {
    // The variable j will keep track of the index in the subkeys array
    int i, j;
    // Loop through the first expanded passphrase
    for (i = 0; i < 8; i++) {
        j = i;
        char byte_1[3], byte_2[3], byte_3[3], byte_4[3];
        byte_1[0] = expanded_passphrase_1[(i*8) + 0];
        byte_1[1] = expanded_passphrase_1[(i*8) + 1];
        byte_1[2] = '\0';

        byte_2[0] = expanded_passphrase_1[(i*8) + 2];
        byte_2[1] = expanded_passphrase_1[(i*8) + 3];
        byte_2[2] = '\0';

        byte_3[0] = expanded_passphrase_1[(i*8) + 4];
        byte_3[1] = expanded_passphrase_1[(i*8) + 5];
        byte_3[2] = '\0';

        byte_4[0] = expanded_passphrase_1[(i*8) + 6];
        byte_4[1] = expanded_passphrase_1[(i*8) + 7];
        byte_4[2] = '\0';

        subkeys[(4*j) + 0] = (byte)strtol(byte_1, NULL, 16);
        subkeys[(4*j) + 1] = (byte)strtol(byte_2, NULL, 16);
        subkeys[(4*j) + 2] = (byte)strtol(byte_3, NULL, 16);
        subkeys[(4*j) + 3] = (byte)strtol(byte_4, NULL, 16);
    }

    // Loop through the second expanded passphrase
    for (i = 0; i < 8; i++) {
        j = 8+i;
        char byte_1[3], byte_2[3], byte_3[3], byte_4[3];
        byte_1[0] = expanded_passphrase_2[(i*8) + 0];
        byte_1[1] = expanded_passphrase_2[(i*8) + 1];
        byte_1[2] = '\0';

        byte_2[0] = expanded_passphrase_2[(i*8) + 2];
        byte_2[1] = expanded_passphrase_2[(i*8) + 3];
        byte_2[2] = '\0';

        byte_3[0] = expanded_passphrase_2[(i*8) + 4];
        byte_3[1] = expanded_passphrase_2[(i*8) + 5];
        byte_3[2] = '\0';

        byte_4[0] = expanded_passphrase_2[(i*8) + 6];
        byte_4[1] = expanded_passphrase_2[(i*8) + 7];
        byte_4[2] = '\0';

        subkeys[(4*j) + 0] = (byte)strtol(byte_1, NULL, 16);
        subkeys[(4*j) + 1] = (byte)strtol(byte_2, NULL, 16);
        subkeys[(4*j) + 2] = (byte)strtol(byte_3, NULL, 16);
        subkeys[(4*j) + 3] = (byte)strtol(byte_4, NULL, 16);
    }
}

int getSubkey(int round) {
    int *subkey = (int*)&subkeys[round*4];
    int return_value = *subkey;
    return return_value;
}

void feistelEncrypt() {
    int R[16];
    int L[16];

    // For each block in the plain text.
    for (int i = 0; i < input_text_size; i+=8){
        int* L_ptr = (int*)&input_text[i];
        int* R_ptr = (int*)&input_text[i+4];

        L[0] = *L_ptr;
        R[0] = *R_ptr;
        for (int current_round = 1; current_round < 16; current_round++){ //16
            L[current_round] = R[current_round-1];
            R[current_round] = L[current_round-1] ^ f(R[current_round-1], getSubkey(current_round));
        }
        L_ptr = (int*)&output_text[i];
        R_ptr = (int*)&output_text[i+4];
        *L_ptr = L[15];
        *R_ptr = R[15];
    }
}

void feistelDecrypt(){
    int R[16];
    int L[16];

    for (int i = 0; i < input_text_size; i++){
        int* L_ptr = (int*)&input_text[i];
        int* R_ptr = (int*)&input_text[i+4];

        L[15] = *L_ptr;
        R[15] = *R_ptr;
        for (int current_round = 15; current_round > 0; current_round--){
            R[current_round-1] = L[current_round];
            L[current_round-1] = R[current_round] ^ f(R[current_round-1], getSubkey(current_round));
        }
        L_ptr = (int*)&output_text[i];
        R_ptr = (int*)&output_text[i+4];
        *L_ptr = L[0];
        *R_ptr = R[0];
    }
}

// Assuming int = 4 bytes and block is a 4 bytes array
int f(int block, int subkey) {
    return block ^ subkey;
}
