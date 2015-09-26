#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long powerMod(long long base, long long exponent, long long modulus);
long long recPowerMod(long long base, long long exponent, long long modulus);

int main (int argc, char **argv) {
    int base, exponent, modulus;
    base = (int)strtol(argv[1], NULL, 10);
    exponent = (int)strtol(argv[2], NULL, 10);
    modulus = (int)strtol(argv[3], NULL, 10);

    long long answer = recPowerMod(base, exponent, modulus);
    printf("Ans: %lld.\n", answer);
    return 0;
}

long long recPowerMod(long long base, long long exponent, long long modulus) {
    if (exponent == 0) {
        return 1;
    }
    else if (exponent%2 == 0) {
        long long ans = recPowerMod(base, exponent/2, modulus);
        return (ans * ans) % modulus;
    }
    else {
        return (((base % modulus) * recPowerMod(base, exponent-1, modulus)) % modulus);
    }
}
