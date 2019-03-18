#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>


#define swap(a,b,t) ((t)=(a),(a)=(b),(b)=(t))



void insertionSort(int *array, unsigned int n) {
    int i, j, k;
    for (i=1; i < n; i++) {
        k = array[i];
        for (j = i-1; array[j] > k && j >= 0; j--) /* I'm checking only those elements, whose indices are lower than that of k */
            array[j + 1] = array[j]; /* move elements greater than k one place to the right */
        array[j+1] = k; /* put k back into the array, in its appropriate place */
    }
}

void selectionSort(int *array, unsigned int n) {
    int minIndex, t, i, j;
    for (i = 0; i < n; i++) {
        minIndex = i; /* find the minimal value in array[i:]  */
        for (j = i+1; j < n; j++)
            if (array[j] < array[minIndex])
                minIndex = j;
        swap(array[i], array[minIndex], t); /* swap a[i] with the minimal element */
    } /* increment i, repeat */
}

void bubbleSort(int *array, unsigned int n) {
    int t, i;
    bool sorted = false;

    while (n > 1 && !sorted) {
        sorted = true;
        /* swap each 2 adjacent elements if not in order */
        for (i = 0; i < n-1; i++) {
            if (array[i] > array[i+1])
                swap(array[i], array[i+1], t), sorted = false;
        }
        n--; /* because the last element is maximal */
    }
    
}

void heapSort(int *array, unsigned int n) {
    unsigned int t, i, j, k;
    array--; /* so that the array can be indexed from 1 for easier arithmetic */
    /* create heap */
    for (i = 1; i <= n; i++) {
        k = i;
        for (j = i / 2; array[k] > array[j] && k > 1; k = j, j /= 2)
            swap(array[k], array[j], t);
    }
    /* sort */
    for (i = n; i > 1; i--) {
        swap(array[i], array[1], t);
        j = 1;
        k = 2;
        while (k < i) {
            if (array[k] < array[k+1] && k+1 < i)
                k++;
            if (array[k] > array[j])
                swap(array[k], array[j], t);
            j = k;
            k *= 2;
        }
    }
    array++; /* return back to normal */
}

static int recursionDepth = 0, maxRecursionDepth = 0, numSwaps = 0, numComparisons = 0;
static std::vector<float> qsDeviations;


void qsa(int *array, int beg, int end) {
    int a;
    int i, j, t;
    if (recursionDepth > maxRecursionDepth) maxRecursionDepth = recursionDepth;
    a = array[(end+beg)/2]; /* choose the middle element */
    i = beg - 1;
    j = end + 1;
    while(1) {
        do numComparisons++, i++; while(array[i] < a); /* find leftmost to swap */
        do numComparisons++, j--; while(array[j] > a); /* find rightmost to swap */
        if (j < i) /* finish if pointers cross */
            break;
        swap(array[i], array[j], t);
        numSwaps++;
    }
    
    /* repeat recursively for both parts: "<a" and ">a" */
    if (j - beg >= 1)
        recursionDepth++, qsa(array, beg, j), recursionDepth--;
    
    if (end - i >= 1)
        recursionDepth++, qsa(array, i, end), recursionDepth--;

    int middle = (beg+end)/2;
    float median = (end - beg + 1) % 2 ? *(array + middle) : (*(array+middle)+*(array+middle+1))/2.f;
    qsDeviations.push_back( (float)((a-median)*(a-median)) );
}

void quickSortAnalysis(int *array, unsigned int n) {
    FILE *file;
    qsDeviations.clear();
    recursionDepth = 0;
    maxRecursionDepth = 0;
    numSwaps = 0;
    numComparisons = 0;
    file = fopen("qs_data.txt", "a");
    qsa(array, 0, n-1);
    float sum = 0;
    for (float i : qsDeviations) {
        sum += i;
    }
    printf("SUM: %f\n", sum);
    float sd = sqrt(sum / n);
    printf("Standard deviation: %f\n", sd);
    printf("Max recursion depth: %d\n", maxRecursionDepth);
    printf("Number of swaps: %d\n", numSwaps);
    fprintf(file, "%d\t%d\t%f\t%d\t%d\n", n, maxRecursionDepth, sd, numSwaps, numComparisons);

    fclose(file);
}

void qs(int *array, int beg, int end) {
    int a;
    int i, j, t;
    a = array[(end+beg)/2]; /* choose the middle element */
    i = beg - 1;
    j = end + 1;
    while(1) {
        do i++; while(array[i] < a); /* find leftmost to swap */
        do j--; while(array[j] > a); /* find rightmost to swap */
        if (j < i) /* finish if pointers cross */
            break;
        swap(array[i], array[j], t);
    }
     /* repeat recursively for both parts: "<a" and ">a" */
    if (j - beg >= 1)
        qs(array, beg, j);
    if (end - i >= 1)
        qs(array, i, end);
}

void quickSort(int *array, unsigned int n) {
    qs(array, 0, n-1);
}

#define CH(func) ;

void qs2(int *array, int beg, int end) {
    int a, b, c;
    int i, j, t;
    a = array[(end+beg)/2]; 
    b = array[beg];
    c = array[end];

    a = a < b ? (b < c ? b : (a < c ? c : a)) : (b > c ? b : (a > c ? c : a)); /* choose the median of first, middle and last */

    i = beg - 1;
    j = end + 1;
    while(1) {
        do i++; while(array[i] < a); /* find leftmost to swap */
        do j--; while(array[j] > a); /* find rightmost to swap */
        if (j < i) /* finish if pointers cross */
            break;
        swap(array[i], array[j], t);
    }
     /* repeat recursively for both parts: "<a" and ">a" */
    if (j - beg >= 1)
        qs(array, beg, j);
    if (end - i >= 1)
        qs(array, i, end);
}

void quickSort2(int *array, unsigned int n) {
    qs2(array, 0, n-1);
}




void countingSortOnlyInt(int *array, unsigned int n) { /* ONLY FOR SORTING INTEGERS */
    int min, max, range, *ar, *end, *luTable;

    /* find the minimal and maximal values */
    min = max = array[0];
    end = array+n;
    for (ar = array; ar < end; ar++) {
        if (*ar < min)
            min = *ar;
        else if (*ar > max)
            max = *ar;
    }

    /* allocate space needed for the lookup table and init to zeros */
    range = max - min + 1;
    luTable = (int*)malloc(range * sizeof(int));
    memset(luTable, 0, range * sizeof(int));

    /* fill the table with the number of occurances for each value in the array */
    for (ar = array; ar < end; ar++) {
        luTable[*ar-min]++;
    }

    /* overwrite the array with sorted values, utilising luTable */
    end = luTable + range;
    for (ar = luTable; ar < end; ar++) {
        while ((*ar)--)
            *(array++) = min;
        min++;
    }
    free(luTable);
    /* here 'array' points to the end of the array */
}

void countingSort(int *array, unsigned int n) {
    int min, max, range, *ar, *end, *luTable, *aux;

    aux = (int*) malloc(n * sizeof(int));

    /* find the minimal and maximal values */
    min = max = array[0];
    end = array+n;
    for (ar = array; ar < end; ar++, aux++) {
        /* at the same time copy the array's elements to the auxiliary array */
        *aux = *ar;
        if (*ar < min)
            min = *ar;
        else if (*ar > max)
            max = *ar;
    }

    /* allocate space needed for the lookup table and init to zeros */
    range = max - min + 1;
    luTable = (int*)malloc(range * sizeof(int));
    memset(luTable, 0, range * sizeof(int));
    
    /* fill the table with the number of occurances for each value in the array */
    for (ar = array; ar < end; ar++) {
        luTable[*ar-min]++;
    }

    /* accumulate */
    end = luTable + range;
    for (ar = luTable + 1; ar < end; ar++) {
        *ar += *(ar-1);
    }

    /* overwrite the array with sorted values, iterating from the end  */
    end = aux - n;
    for(aux--; aux >= end; aux--) {
        array[--luTable[*aux - min]] = *aux;
    }

    free(luTable);
    free(aux);
}

void shellSort(int *array, unsigned int n) { /* same as Insertion Sort, but applied every n/2^k (k=0,1,...) elements */
    int i, j, t, g;
    g = n;
    while ((g >>= 1) > 0) { /* g = gap */
        for (i=g; i < n; i++) {
            t = array[i];
            for (j = i; j >= g && array[j-g] > t; j-= g)
                array[j] = array[j - g];
            array[j] = t;
        }
    }
}

void shellSort2(int *array, unsigned int n) { /* same as Insertion Sort, but applied every 2*i^2+1 (i=...,1,0) elements */
    int i, j, t, g;
    int k = (int)floor(sqrt((n-1)/2));
    do { /* g = gap */
        g = 2*k*k+1;
        for (i=g; i < n; i++) {
            t = array[i];
            for (j = i; j >= g && array[j-g] > t; j-= g)
                array[j] = array[j - g];
            array[j] = t;
        }
    } while (k--);
}
  
void shellSort3(int *array, unsigned int n) { /* same as Insertion Sort, but applied every 2^i+1 (i=...,1,0) elements */
    int i, j, t, g;
    int k = 0, l = n;
    while (l /= 2) k++;
    do { /* g = gap */
        g = (2 << k) - 1;
        for (i=g; i < n; i++) {
            t = array[i];
            for (j = i; j >= g && array[j-g] > t; j-= g)
                array[j] = array[j - g];
            array[j] = t;
        }
    } while (k--);
}

void ms(int *array, int i, int j, int *aux) {
    int k, m, l, r;
    if (j <= i) {
        return;
    }
    m = (i + j) / 2; /* middle */
    ms(array, i, m, aux); /* repeat for lower */
    ms(array, m + 1, j, aux); /* and higher */

    l = i;
    r = m + 1;
    k;
    for (k = i; k <= j; k++) {

        if (l == m + 1)
            aux[k] = array[r++];
        else if (r == j + 1)
            aux[k] = array[l++];
        else if (array[l] < array[r])
            aux[k] = array[l++];
        else    
            aux[k] = array[r++];

    }
    for (k = i; k <= j; k++) { /* copy the elements from aux[] to array[] */
        array[k] = aux[k];
    }
}

void mergeSort(int *array, unsigned int n) {
    int *aux;
    aux = (int*)malloc(n * sizeof(int));
    ms(array, 0, n-1, aux);

}

#undef swap