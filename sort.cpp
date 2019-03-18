#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "algs.h"
#include "utils.h"

#define DEFAULT_SIZE 100000


static bool VERBOSE = false;
static unsigned int SEED;
static bool DISPLAY_HELP = false, STORE_RESULTS = true;

static FILE *outFile;

Method sortingMethods[] = {
        {'i', insertionSort, "Insertion Sort"}, 
        {'s', selectionSort, "Selection Sort"}, 
        {'b', bubbleSort, "Bubble Sort"},
        {'h', heapSort, "Heap Sort"},
        {'q', quickSort, "Quick Sort"},
        {'a', quickSortAnalysis, "Quick Sort Analysis"},
        {'Q', quickSort2, "Quick Sort 2"},
        {'c', countingSort, "Counting Sort"}, 
        {'S', shellSort, "Shell Sort"},
        {'2', shellSort2, "Shell Sort 2"},
        {'3', shellSort3, "Shell Sort 3"},
        {'m', mergeSort, "Merge Sort"}
};

Method fillingMethods[] = {
        {'I', incFill, "increasing"},
        {'D', decFill, "decreasing"},
        {'R', randomFill, "random"},
        {'C', constFill, "constant"},
        {'V', decIncFill, "decreasing and then increasing"},
        {'X', incDecFill, "increasing and then decreasing"}
};

void displayHelp() {
    int i;
    printf("Usage: sort [sorting methods] [filling method] min:[max]:[step] [other options]\n");
    printf("\nSorting methods:\n");
    for (i=0; i < sizeof(sortingMethods) / sizeof(Method); i++) {
        printf("-%c\t%s\n", sortingMethods[i].code, sortingMethods[i].name);
    }
    printf("\nArray filling methods:\n");
    for (i=0; i < sizeof(fillingMethods) / sizeof(Method); i++) {
        printf("-%c\t%s\n", fillingMethods[i].code, fillingMethods[i].name);
    }
    printf("\nOther options:\n \
--help     display help\n \
    -n     no file output\n \
    -v     verbose\n \
    -A n   average of n\n \
    -r m   max value that the RNG will output\n");
}

static int lastSize;



long long testAlgorithm(void (*func)(int *, unsigned int), Method fm, int* array, int n, const char* name) {
    long long et; bool success; 
    int nPrint;
    
    if (n > lastSize) {
        free(array);
        array = (int*) malloc(n * sizeof(int));
        lastSize = n;
    }

    nPrint = n < 10 ? n : 10;


    ARR_SIZE = n;
    if (VERBOSE)
        printf("Filling the array with %s numbers...\n", fm.name);

    fm.func(array, n);

    if (VERBOSE) {
        peekArray(array, nPrint, n);
        printf("\nArray sorted: %s\n", success ? "true" : "false");
        printf("Sorting %d elements with %s...\n", n, name);
    }
    
    
    et = measureET(func, array, n);
    success = isSorted(array, n);

    if (VERBOSE) {
        peekArray(array, nPrint, n);
        printf("\nArray sorted: %s\n", success ? "true" : "false");
    }
       
    
    if (VERBOSE) {
        if (success) {
            printf("Success. Execution time: %ldus\n\n", et); 
        }
        else {
            printf("AFTER %ldus SORTING FAILED\n\n", et);
        }
    } else {
        printf("%d, -%c, %s, %ldus\n", n, fm.code, name, et);
        if (!success)
            printf("[FAIL]\n");
    }
    return et;
}






int main(int argc, char** argv) {
    int tm, tf, *array, i, currSize;
    int loptId;
    int numMeasurements = 1;
    long long average = 0;
    struct option lopts[] = {{"help", no_argument, 0, 0}};
    Method sortings[32];
    Method fillFuncs[32];
    int* sizes[32] = {NULL};

    outFile = fopen("results.txt", "a");

    SEED = time(NULL);

    tm = 0; 
    tf = 0;


    char opt;
    lastSize = DEFAULT_SIZE;

    while ((opt = getopt_long(argc, argv, "isbhqQaHc23SmI::D::R::C::V::X::vnA:r:", lopts, &loptId)) != -1) {
        if (loptId == 0)
            DISPLAY_HELP = true;

        if (opt == 'v') {
            VERBOSE = true;
            continue;
        }
        if (opt == 'n') {
            STORE_RESULTS = false;
            continue;
        }
        if (opt == 'A') {
            numMeasurements = atoi(optarg);
            continue;
        }
        if (opt == 'r') {
            MAX_RAND = atoi(optarg);
            continue;
        }

        for (i=0; i < sizeof(sortingMethods) / sizeof(Method); i++) {
            if (opt == sortingMethods[i].code) {
                sortings[tm++] = sortingMethods[i];
                break;
            }
        }
        for (i=0; i < sizeof(fillingMethods) / sizeof(Method); i++) {
            if (opt == fillingMethods[i].code) {
                sizes[tf] = NULL;
                fillFuncs[tf] = fillingMethods[i];
                int* size = myAtoi(argv[optind]);
                if (size) {
                    for (i = tf; sizes[i] == NULL && i >= 0; i--)
                        sizes[i] = size;
                    lastSize = size[1];
                }
                tf++;
                break;
            }
        }
    }

    if (DISPLAY_HELP)
        displayHelp();


    for (i=0; i < tm; i++) {
        lastSize = sizes[i][1];
        array = (int*) malloc(lastSize * sizeof(int));
        if(STORE_RESULTS) {
            fprintf(outFile, "\n%s (%s)\n", sortings[i].name, fillFuncs[i].name);
            fprintf(outFile, "Array size\tTime(us)\n");
        }
        
        for(currSize = sizes[i][0]; currSize <= sizes[i][1]; currSize+= sizes[i][2]) {
            average = 0;
            for (int k = 0; k < numMeasurements; k++) {
                average  += testAlgorithm(sortings[i].func, fillFuncs[i], array, 
                currSize, sortings[i].name) / numMeasurements;
            }
            printf("Average of %d: %ldus\n", numMeasurements, average);
            if (STORE_RESULTS)
                fprintf(outFile, "%d\t%ld\n", currSize, average);
            
        }
        
    }

    free(array);
    fclose(outFile);
    return 0;
}

