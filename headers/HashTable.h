#pragma once

#include "./patients.h"
#include "./generalFuncs.h"
#include "./HashTable.h"
#include "./AVL.h"
#include "./MaxHeap.h"

#include <stdbool.h>

// bucket is made of an array of pointers
// [0]->pointer to string [1]->pointer to AVL [2]->pointer to string ...
typedef struct Bucket{
    int totalValues;
    void **arr;             // saves pairs of string, pointer
    struct Bucket *next;    // next if current gets Full
} Bucket;

typedef Bucket *hashBucket;

typedef struct HTable{
    int bucket_size;    // for all buckets
    int entries;        // hash indexes -> possible results of hashFunction
    hashBucket bucket_array;    // buckets of Table
} HTable;

typedef HTable *HashTable;


HashTable initHashTable(int, int);
void    deleteHT(HashTable);
void    printHashTable(HashTable);
void    printBucket(hashBucket);
void    emptyBucket(hashBucket*, int);
int     hashFunction(char*, int);
bool    addHT(HashTable, patientRecord, int);
void    addBucket(hashBucket, patientRecord, int);
hashBucket addNewBucket(patientRecord, int, int);
void    printNumOfDis(hashBucket, char*, char*);
void    globalDiseaseStats(HashTable, char*, char*);
void    countryOccurences(HashTable, char*, char*, char*, char*);
void    topk(HashTable, char*, char*, char*, char*, bool);
void    accesSpecificBucket(hashBucket, char*, int k, char*, char*, bool);
void    diseaseFrequencyNoCountry(HashTable, char*, char*, char*);
void    diseaseFrequencyCountry(HashTable, char*, char*, char*, char*);
void    numCurrentPatients(HashTable, char*);