#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "metrohash.h"

#define MAX_STRING 100
#define NUM_REGISTERS 16384
#define ALPHA (0.7213 / (1 + 1.079 / 16384.0))
#define MAX_X (UINT64_MAX >> 50)
#define SEED 46769

uint8_t registers[NUM_REGISTERS];

float beta(float ez) {
    float zl = log(ez + 1);
    return -0.370393911 * ez + 0.070471823 * zl + 0.17393686 * pow(zl, 2) +
           0.16339839 * pow(zl, 3) + -0.09237745 * pow(zl, 4) +
           0.03738027 * pow(zl, 5) + -0.005384159 * pow(zl, 6) +
           0.00042419 * pow(zl, 7);
}

int main(void) {
    char str[MAX_STRING];
    int i;
    const int precision = 14;
    uint64_t hash = 0;
    uint64_t k;
    uint8_t val;
    long long word_num = 0;
    char eof = 0;
    while (!feof(stdin)) {
        if ((word_num >= 100000) && (word_num % 100000 == 0)) {
            fprintf(stderr, "Num Words: %9lldK%c", word_num / 1000, 13);
            fflush(stderr);
        }
        scanf("%s", str);
        if (eof) break;
        word_num++;
        metrohash64_1((const uint8_t *)str, strlen(str), SEED,
                      (uint8_t *)&hash);
        k = hash >> 50;
        val = (uint8_t)__builtin_clzl((hash << precision) ^ MAX_X) + 1;
        if (registers[k] < val) {
            registers[k] = val;
        }
    }
    float sum = 0.0;
    float ez = 0.0;
    float m = 16384.0;
    for (i = 0; i < NUM_REGISTERS; i++) {
        if (registers[i] == 0) {
            ez++;
        }
        sum += 1.0 / pow(2.0, (float)registers[i]);
    }
    uint64_t ret = (uint64_t)(ALPHA * m * (m - ez) / (beta(ez) + sum));
    printf("%llu\n", ret);
    return 0;
}
