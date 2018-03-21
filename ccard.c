#include <stdio.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include "highwayhash.c"
#include "farmhash.c"


//const uint8_t m = 16384;
uint8_t registers[16384];

float beta(float ez) {
    float zl = log(ez + 1);
    return -0.370393911*ez +
        0.070471823*zl +
        0.17393686*pow(zl, 2) +
        0.16339839*pow(zl, 3) +
        -0.09237745*pow(zl, 4) +
        0.03738027*pow(zl, 5) +
        -0.005384159*pow(zl, 6) +
        0.00042419*pow(zl, 7);
}

int main(void) {
    char str[512];
    int i;
    //uint64_t key[4] = {1, 2, 3, 4};
    const int precision = 14;
    uint64_t hash;
    uint64_t k;
    uint8_t val;
    uint64_t maxX = UINT64_MAX >> 50;
    float alpha = 0.7213 / (1 + 1.079/16384.0);
    long long line_num = 0;
    uint64_t seed = 46769;
    while(fgets(str, 512, stdin) != NULL) {
        if (line_num % 20000 == 0) {
            fprintf(stderr, "%6lldK%c", line_num/1000, 13);
            fflush(stderr);
        }
        line_num++;
        //i = 0;
        //while (str[i] >= ' ') {
        //    i++;
        //}
        //str[i] = '\0';
        //hash = HighwayHash64((const uint8_t *)str, strlen(str), key);
        hash = farmhash64_with_seed(str, strlen(str), seed);
        k = hash >> 50;
        val = (uint8_t)__builtin_clzl((hash<<precision)^maxX) + 1;
        //memset(str, 0, 512);
        if (registers[k] < val) {
            registers[k] = val;
        }
    }
    float sum = 0.0;
    float ez = 0.0;
    float m = 16384.0;
    for(i=0; i<16384; i++) {
        if(registers[i]==0) {
            ez++;
        }
        sum += 1.0 / pow(2.0, (float)registers[i]);
    }
    uint64_t ret = (uint64_t)(alpha * m * (m - ez)/(beta(ez) + sum));
    printf("estimated_cardinality= %llu\n", ret);
    return 0;
}
