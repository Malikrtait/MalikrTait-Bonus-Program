#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz + sizeof(size_t);
    size_t* ret = (size_t*)malloc(sizeof(size_t) + sz);
    if (ret == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void* ptr)
{
    size_t* pSz = (size_t*)ptr - 1;
    extraMemoryAllocated -= *pSz + sizeof(size_t);
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
    return ((size_t*)ptr)[-1];
}

void selectionSort(int* pData, int n)
{
    int i, j, minIndex, tmp;
    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (pData[j] < pData[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            tmp = pData[i];
            pData[i] = pData[minIndex];
            pData[minIndex] = tmp;
        }
    }
}

void insertionSort(int* pData, int n)
{
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = pData[i];
        j = i - 1;
        while (j >= 0 && pData[j] > key) {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
}

void bubbleSort(int* pData, int n)
{
    int i, j, tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (pData[j] > pData[j + 1]) {
                tmp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = tmp;
            }
        }
    }
}

void merge(int* pData, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i];
            i++;
        }
        else {
            pData[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        pData[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        pData[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortHelper(int* pData, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSortHelper(pData, l, m);
        mergeSortHelper(pData, m + 1, r);

        merge(pData, l, m, r);
    }
}

void mergeSort(int* pData, int l, int r)
{
    mergeSortHelper(pData, l, r);
}

void heapify(int arr[], int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

int parseData(char* inputFileName, int** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int*)Alloc(sizeof(int) * dataSz);
        int i;
        for (i = 0; i < dataSz; i++) {
            fscanf(inFile, "%d", &(*ppData)[i]);
        }
        fclose(inFile);
    }

    return dataSz;
}

void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    for (i = sz; i < dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

    for (i = 0; i < 3; ++i) {
        int* pDataSrc, * pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int*)Alloc(sizeof(int) * dataSz);
        memcpy(pDataCopy, pDataSrc, sizeof(int) * dataSz); // Copy data for each sorting algorithm

        printf("---------------------------\n");
        printf("Dataset Size : %d\n", dataSz);
        printf("---------------------------\n");

        extraMemoryAllocated = 0; 
        start = clock();
        selectionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Selection Sort: Time: %f sec, Extra Memory Allocated: %d bytes\n", cpu_time_used, extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        memcpy(pDataCopy, pDataSrc, sizeof(int) * dataSz); 

        extraMemoryAllocated = 0; 

        start = clock();
        insertionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Insertion Sort: Time: %f sec, Extra Memory Allocated: %d bytes\n", cpu_time_used, extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        memcpy(pDataCopy, pDataSrc, sizeof(int) * dataSz); 
        extraMemoryAllocated = 0; 

        start = clock();
        bubbleSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Bubble Sort: Time: %f sec, Extra Memory Allocated: %d bytes\n", cpu_time_used, extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        memcpy(pDataCopy, pDataSrc, sizeof(int) * dataSz); 

        extraMemoryAllocated = 0; 

        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Merge Sort: Time: %f sec, Extra Memory Allocated: %d bytes\n", cpu_time_used, extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        memcpy(pDataCopy, pDataSrc, sizeof(int) * dataSz); 
        extraMemoryAllocated = 0; 
        start = clock();
        heapSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Heap Sort: Time: %f sec, Extra Memory Allocated: %d bytes\n", cpu_time_used, extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        DeAlloc(pDataSrc);
        DeAlloc(pDataCopy);
        }

        return 0;
        }
