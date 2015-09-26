#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

int arg_c;
char **arg_v;
long int key_size;
int frequency[26];
double mean;
double std_dev;
double std_dev_sum;

void resetFrequency();
int frequencyAnalisis();
void calculateMean();
void calculateStdDev();
void printData();

// Usage:
// ./main [cipher] [key_size]
int main (int argc, char **argv) {
    if (argc < 2) {
        puts("Invalid number of arguments!");
        return 1;
    }
    arg_c = argc;
    arg_v = argv;
    key_size = strtol(argv[2], NULL, 10);

    /*
    std_dev_sum = 0;

    int i = 0;
    for (i = 0; i < key_size; i++) {
        resetFrequency();
        frequencyAnalisis(i);
        calculateMean();
        calculateStdDev();
        printData();
        std_dev_sum += std_dev;
    }

    printf("Std deviation sum = %f\n", std_dev_sum);
    */
    key_size = 12;
    int i = 0;
    for(i = 0; i < key_size; i++) {
        resetFrequency();
        frequencyAnalisis(i);
        printData();
        getchar();
    }

    return 0;
}

void resetFrequency() {
    int i = 0;
    for (i=0; i<26; i++) {
        frequency[i] = 0;
    }
}

int frequencyAnalisis(int increment) {
    int cipher_size = strlen(arg_v[1]);
    int i = 0;

    for (i = increment; i < cipher_size; i += key_size) {
        char currentChar = arg_v[1][i];
        currentChar -= 97;
        frequency[(int)currentChar]++;
    }


    return 0;
}

void calculateMean() {
    int i = 0;
    double partial_sum = 0;
    mean = 0;
    for (i = 0; i<26; i++) {
        partial_sum += frequency[i];
    }
    mean = partial_sum/26;
}

void calculateStdDev() {
    int i;
    std_dev = 0;
    for(i=0; i<26;++i) {
        std_dev+=(frequency[i]-mean)*(frequency[i]-mean);
    }
    std_dev = sqrt(std_dev/26);
}


void printData() {
    printf("Frequency analisis for key of size %li:\n", key_size);
    int i=0;
    for (i = 0; i < 26; i++) {
        printf("%c: %d\n", i+97, frequency[i]);
    }
    //printf("Mean value: %f\nStd deviation: %f\n", mean, std_dev);
}
