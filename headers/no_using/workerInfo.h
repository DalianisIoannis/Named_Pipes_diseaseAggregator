#include "./generalFuncs.h"

typedef struct cNode{
    char* country;
    struct cNode *next;
} cNode;
typedef cNode *countrylistNode;

typedef struct{
    countrylistNode front;
    countrylistNode rear;
} countryList;
typedef countryList *CountryList;


CountryList initcountryList();
void emptycountryList(CountryList*);
void addCountryListNode(CountryList*, char*);
CountryList makeCountryList(char*);
void printCountryList(CountryList);


typedef struct workerData{
    pid_t pid;
    CountryList PIDcountries;
    int totalCountries;
    char *fifoRead;
    char *fifoWrite;
    int fdRead;
    int fdWrite;
    struct workerData *next;
} workerData;
typedef workerData *workerDataNode;

// typedef struct{
//     workerDataNode front;
//     workerDataNode rear;
// } workerl;
// typedef workerl *workerList;


// workerList initworkerList();
// void emptyworkerList(workerList*);
// bool addWorkerNode(workerList*, pid_t, char*, int, int, int, int);
// void printWorkerList(workerList);
void printWorkerNode(workerDataNode);
workerDataNode makeWorkerArCell(pid_t, char*, int, int, int, int);
void emptyworkerNode(workerDataNode*);