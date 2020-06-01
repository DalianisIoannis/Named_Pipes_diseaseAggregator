#pragma once

#include "./countryList.h"
#include "./general.h"
#include "./linkedList.h"
#include "./statistics.h"

typedef struct workerData{
    int totalCountries;

    pid_t pid;
    CountryList PIDcountries;
    char *fifoRead;
    char *fifoWrite;
    
    int fdRead;
    int fdWrite;
    struct workerData *next;
} workerData;
typedef workerData *workerDataNode;

workerDataNode* WorkersArr;

// int mySignalFlag=1;

void printWorkerNode(workerDataNode);
workerDataNode makeWorkerArCell(pid_t);
void emptyworkerNode(workerDataNode*);
int WorkerRun(char*, int, int, int, workerDataNode);
int inputPatientsToStructures(char*, Linked_List*, char* date, char* country, StatisticsList*, Linked_List *);