#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "algs.h"
#include <windows.h>
#include <random>


int ARR_SIZE = 10000;
int MAX_RAND = 0x7fffffff;

typedef struct {
    char code;
    void (*func)(int*, unsigned int);
    const char* name;
} Method;

void printArray(int *array, unsigned int n) {
    int i;
    for(i=0; i < n; i++) {
        printf("%d, ", array[i]);
    }
}

void peekArray(int *array, int nPrint, int n) {
    printf("array: ");
    printArray(array, nPrint/2);
    if (nPrint < n) printf("..., ");
    printArray(array + n - nPrint/2, nPrint/2);
}

long long measureET(void (*func)(int *, unsigned int), int* arg1, int arg2) {
    LARGE_INTEGER start, end, frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    func(arg1, arg2);
    QueryPerformanceCounter(&end);
    return (end.QuadPart - start.QuadPart) * 1000000 / frequency.QuadPart;
}

void randomFill(int *array, unsigned int n) {
    std::uniform_int_distribution<int> R(0, MAX_RAND);
    std::default_random_engine generator(time(NULL));
    while(n--) array[n] = R(generator);
}

void decFill(int *array, unsigned int n) {
    int i = 0;
    while(n--) array[i++] = n;
}

void incFill(int *array, unsigned int n) {
    while(n--) array[n] = n;
}

void constFill(int *array, unsigned int n) {
    while(n--) array[n] = 0;
}

void incDecFill(int *array, unsigned int n) {
    int i = 0;
    for (; i < n / 2; i++) array[i] = 2*i+1;
    for (; i < n; i++) array[i] = 2*(n-i);
}

void decIncFill(int *array, unsigned int n) {
    int i = 0;
    for (; i < n / 2; i++) array[i] = 2*(n/2-i)+1;
    for (; i < n; i++) array[i] = 2*(i-n/2);
}

bool isSorted(int *array, int n) {
    int i;
    for(i = 0; i < n-1; i++)
        if (array[i] > array[i+1])
            return false;
    return array[n-1] >= 0;
}

int* myAtoi(char* s) {
    int *params, i;
    char *c;
    if (s == NULL)
        return NULL;
    params = (int*)malloc(3*sizeof(int));
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    for (c = s; *c != 0; c++) {
        if (*c == ':') {
            i++;
        } else if (*c >= '0' && *c <= '9') {
            params[i] *= 10;
            params[i] += (int)(*c - '0');
        }
        else if (*c == 'k') {
            params[i] *= 1000;
        } else if (*c == 'M') {
            params[i] *= 1000000;
        } else if (*c == 'G') {
            params[i] *= 1000000000;
        } else {
            return NULL;
        }
    }
    if (i == 0) {
        params[1] = params[0];
    } else if (i == 1) {
        params[2] = (int)ceil((params[1]-params[0])/15.0);
    } else if (params[1] == 0 && params[2] != 0) {
        params[1] = params[0] + 15 * params[2];
    }
    if (params[2] == 0)
        params[2] = 1000;
    return params;
}

#endif