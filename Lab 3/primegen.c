#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PRIME 10007

int* findGenerators(int *prime_factors_array, int prime_factors_size, int prime, int *generators_size);
long long recPowerMod(long long base, long long exponent, long long modulus);
int* getPrimefactors(int prime, int *size);
int* generators(int prime, int *generators_size_out);

int main (int argc, char **argv) {
    int generators_count;
    int *generators_set = generators(PRIME, &generators_count);

    printf("Found %d generators for the number %d:\n", generators_count, PRIME);
    int i;
    for (i=0; i<generators_count; i++) {
        printf("%d ", generators_set[i]);
    }
    printf("\n");

    return 0;
}

int* generators(int prime, int *generators_size_out) {
    int factors_count;
    int *prime_factors;

    prime_factors = getPrimefactors(prime-1, &factors_count);

    int generators_count;
    int *generators_array;
    generators_array = findGenerators(prime_factors, factors_count, prime, &generators_count);

    *generators_size_out = generators_count;
    return generators_array;
}

int* findGenerators(int *prime_factors_array, int prime_factors_size, int prime, int *generators_size) {
    int *big_array = malloc(sizeof(int) * prime);
    int generators_found = 0;

    int i = 0;
    for (i=2; i < prime; i++) {
        int j = 0;
        int can_be_generator = 1;
        while ((j<prime_factors_size) && (can_be_generator)) {
            if (recPowerMod(i, (prime-1)/prime_factors_array[j], prime) == 1) {
                can_be_generator = 0;
            }
            j++;
        }
        if (can_be_generator) {
            big_array[generators_found] = i;
            generators_found++;
        }
    }

    int *generators_array = malloc(sizeof(int) * generators_found);
    for (i = 0; i < generators_found; i++) {
        generators_array[i] = big_array[i];
    }

    free(big_array);
    *generators_size = generators_found;

    return generators_array;
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

int* getPrimefactors(int prime, int *size) {
    int *big_array = malloc (sizeof(int) * prime);
    int number = prime, factors_count=0;
    int div = 2;

    while (number > 1) {
        if (number%div != 0) {
            div++;
        }
        else {
            number = number/div;
            big_array[factors_count] = div;
            factors_count++;
        }
    }

    int *prime_factors = malloc(sizeof(int) * factors_count);
    int i;
    for (i = 0; i < factors_count; i++) {
        prime_factors[i] = big_array[i];
    }

    *size = factors_count;
    free(big_array);

    return prime_factors;
}
