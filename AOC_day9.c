#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int block;
    int freespace;
} FileInfo;

typedef struct {
    FileInfo* files;
    int size;
    int capacity;
} Dictionary;

void initDictionary(Dictionary* dict) {
    dict->size = 0;
    dict->capacity = 10;
    dict->files = malloc(dict->capacity * sizeof(FileInfo));
}

void addToDictionary(Dictionary* dict, int block, int freespace) {
    if (dict->size == dict->capacity) {
        dict->capacity *= 2;
        dict->files = realloc(dict->files, dict->capacity * sizeof(FileInfo));
    }
    dict->files[dict->size].block = block;
    dict->files[dict->size].freespace = freespace;
    dict->size++;
}

void createDict(int* inputList, int inputSize, Dictionary* filesDict, int** dataArray, int* dataSize) {
    initDictionary(filesDict);
    *dataSize = 0;

    for (int i = 0; i < inputSize; i += 2) {
        int block = inputList[i];
        int freespace = (i + 1 < inputSize) ? inputList[i + 1] : 0;
        addToDictionary(filesDict, block, freespace);
        *dataSize += block + freespace;
    }

    *dataArray = malloc(*dataSize * sizeof(int));
    int index = 0;

    for (int id = 0; id < filesDict->size; id++) {
        for (int j = 0; j < filesDict->files[id].block; j++) {
            (*dataArray)[index++] = id;
        }
        for (int j = 0; j < filesDict->files[id].freespace; j++) {
            (*dataArray)[index++] = -1;  // Using -1 to represent '.'
        }
    }
}

int checkIfDefagDone(int index, int* dataArray, int dataSize) {
    while (index < dataSize) {
        if (dataArray[index] != -1) {
            return 0;
        }
        index++;
    }
    return 1;
}

long long checksum(int* dataArray, int dataSize) {
    long long checksumTotal = 0;
    for (int index = 0; index < dataSize; index++) {
        if (dataArray[index] == -1) continue;
        checksumTotal += (long long)index * dataArray[index];
    }
    return checksumTotal;
}

void defragPartOne(int* origArray, int* dataArray, int dataSize) {
    memcpy(dataArray, origArray, dataSize * sizeof(int));
    int pointer1 = 0;
    int pointer2 = dataSize - 1;

    while (pointer1 < dataSize) {
        if (checkIfDefagDone(pointer1, dataArray, dataSize)) break;

        if (dataArray[pointer1] != -1) {
            pointer1++;
            continue;
        }
        if (dataArray[pointer2] == -1) {
            pointer1--;
            pointer2--;
            continue;
        }

        int temp = dataArray[pointer1];
        dataArray[pointer1] = dataArray[pointer2];
        dataArray[pointer2] = temp;

        pointer1++;
    }
}

int find_slice_index(int* array, int* subarray, int arraySize, int subarraySize) {
    for (int i = 0; i <= arraySize - subarraySize; i++) {
        if (memcmp(array + i, subarray, subarraySize * sizeof(int)) == 0) {
            return i;
        }
    }
    return -1;
}

void defragPartTwo(Dictionary* filesDict, int* origArray, int* dataArray, int dataSize) {
    memcpy(dataArray, origArray, dataSize * sizeof(int));

    int pointer1 = 0;
    int pointer2 = filesDict->size - 1;

    while (pointer1 < filesDict->size) {
        if (pointer2 == 0) break;

        if (pointer1 >= pointer2) {
            pointer1 = 0;
            pointer2--;
            continue;
        }

        if (filesDict->files[pointer2].block <= filesDict->files[pointer1].freespace) {
            int* subarray = malloc(filesDict->files[pointer2].block * sizeof(int));
            for (int i = 0; i < filesDict->files[pointer2].block; i++) {
                subarray[i] = -1;
            }
            int arrayPointer1 = find_slice_index(dataArray, subarray, dataSize, filesDict->files[pointer2].block);
            free(subarray);

            if (arrayPointer1 == -1) {
                printf("Actually not enough space somehow\n");
                exit(1);
            }

            int arrayPointer2 = -1;
            for (int i = 0; i < dataSize; i++) {
                if (dataArray[i] == pointer2) {
                    arrayPointer2 = i + filesDict->files[pointer2].block;
                    break;
                }
            }

            int* temp = malloc(filesDict->files[pointer2].block * sizeof(int));
            memcpy(temp, dataArray + arrayPointer1, filesDict->files[pointer2].block * sizeof(int));
            memcpy(dataArray + arrayPointer1, dataArray + arrayPointer2 - filesDict->files[pointer2].block, filesDict->files[pointer2].block * sizeof(int));
            memcpy(dataArray + arrayPointer2 - filesDict->files[pointer2].block, temp, filesDict->files[pointer2].block * sizeof(int));
            free(temp);

            filesDict->files[pointer1].freespace -= filesDict->files[pointer2].block;

            pointer1 = 0;
            pointer2--;
            continue;
        }

        pointer1++;
    }
}

int main() {
    FILE* file = fopen("input9.txt", "r");
    

    int* inputList = NULL;
    int inputSize = 0;
    int capacity = 10;
    inputList = malloc(capacity * sizeof(int));

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c >= '0' && c <= '9') {
            if (inputSize == capacity) {
                capacity *= 2;
                inputList = realloc(inputList, capacity * sizeof(int));
            }
            inputList[inputSize++] = c - '0';
        }
    }

    fclose(file);

    Dictionary filesDict;
    int* dataArray;
    int dataSize;

    createDict(inputList, inputSize, &filesDict, &dataArray, &dataSize);

    int* defraggedArray = malloc(dataSize * sizeof(int));
    defragPartOne(dataArray, defraggedArray, dataSize);
    long long part1 = checksum(defraggedArray, dataSize);

    printf("\n\nPart 1: %lld\n", part1);

    defragPartTwo(&filesDict, dataArray, defraggedArray, dataSize);
    long long part2 = checksum(defraggedArray, dataSize);

    printf("\n\nPart 2: %lld\n", part2);

    free(inputList);
    free(dataArray);
    free(defraggedArray);
    free(filesDict.files);

    return 0;
}

