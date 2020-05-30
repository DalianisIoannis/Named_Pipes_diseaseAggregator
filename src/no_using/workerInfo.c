#include "../headers/generalFuncs.h"
#include "../headers/workerInfo.h"


CountryList initcountryList(){
    CountryList ll = malloc(sizeof(countryList));
    if( ll==NULL ){
        fprintf(stderr, "Couldn't allocate country List. Abort...\n");
        return NULL;
    }
    (ll)->front     = NULL;
    (ll)->rear      = NULL;
    return ll;
}

void emptycountryList(CountryList* cL){
    countrylistNode tmp;
    while ( (*cL)->front!=NULL ){
        // deleteRecord( &((*ll)->front->item) );
        free( (*cL)->front->country );

        tmp = (*cL)->front->next;
        
        (*cL)->front->next = NULL;
        free((*cL)->front);
        
        (*cL)->front = tmp;
    }
}

// workerList initworkerList(){
//     workerList ll = malloc(sizeof(workerl));
//     if( ll==NULL ){
//         fprintf(stderr, "Couldn't allocate worker List. Abort...\n");
//         return NULL;
//     }
//     (ll)->front     = NULL;
//     (ll)->rear      = NULL;
//     return ll;
// }

// void emptyworkerList(workerList* wL){
//     workerDataNode tmp;
//     while ( (*wL)->front!=NULL ){
//         // deleteRecord( &((*ll)->front->item) );
//         emptycountryList( &((*wL)->front->PIDcountries) );
//         free((*wL)->front->PIDcountries);
//         unlink((*wL)->front->fifoRead);
//         free((*wL)->front->fifoRead);


//         tmp = (*wL)->front->next;
//         (*wL)->front->next = NULL;
//         free((*wL)->front);
//         (*wL)->front = tmp;
//     }
//     free(*wL);
// }

// bool addWorkerNode(workerList* wL, pid_t pid, char* countries, int totalCountries, int pidRead, int pidWrite, int workerNum){
//     printf("Countries gave are %s\n", countries);
//     workerDataNode llNode = malloc(sizeof(workerData));
//     if(llNode==NULL){ return false; }
    
//     llNode->pid = pid;
//     llNode->PIDcountries = makeCountryList(countries);
    
//     llNode->fifoRead = malloc( (sizeof("reader_des")+3)*sizeof(char*) );
//     sprintf(llNode->fifoRead, "reader_des%d", workerNum);
//     mkfifo(llNode->fifoRead, 0666);

//     if( (*wL)->front==NULL ){   // empty list
//         (*wL)->front = llNode;
//         (*wL)->front->next = NULL;
        
//         (*wL)->rear = llNode;
//         (*wL)->rear->next = NULL;
//     }
//     else{
//         (*wL)->rear->next = llNode;
//         (*wL)->rear = llNode;
//         (*wL)->rear->next = NULL;
//     }
//     return true;
// }

CountryList makeCountryList(char* countries){
    CountryList cL = initcountryList();
    // printf("Mesa einai %s\n", countries);
    char* tmp = strdup(countries);
    char* tok = strtok(tmp, " ");
    tok = strtok(NULL, " ");
    while(tok!=NULL){
        // printf("tok %s\n", tok);
        addCountryListNode(&cL, tok);
        tok = strtok(NULL, " ");
    }
    free(tmp);
    return cL;
}

void addCountryListNode(CountryList* cL, char* country){
    countrylistNode node = malloc(sizeof(cNode));
    node->country = strdup(country);
    if( (*cL)->front==NULL ){   // empty list
        (*cL)->front = node;
        (*cL)->front->next = NULL;
        
        (*cL)->rear = node;
        (*cL)->rear->next = NULL;
    }
    else{
        (*cL)->rear->next = node;
        (*cL)->rear = node;
        (*cL)->rear->next = NULL;
    }
}

void printCountryList(CountryList ll){
    countrylistNode tmp = ll->front;
    while(tmp!=NULL){
        printf("\tCountry %s", tmp->country);
        tmp = tmp->next;
        printf("\n");
    }
}

void printWorkerNode(workerDataNode node){
    printf("Worker Node:\n");
    printCountryList(node->PIDcountries);
    printf("\tpid %d\n", node->pid);
    printf("\tfifoRead %s\n", node->fifoRead);
    printf("\tfifoWrite %s\n", node->fifoWrite);
    printf("\ttotalCountries %d\n", node->totalCountries);
    printf("\tfdRead %d\n", node->fdRead);
    printf("\tfdWrite %d\n", node->fdWrite);
}

// void printWorkerList(workerList wL){
//     printf("Printing Worker Data.\n");
//     workerDataNode tmp = wL->front;
//     while(tmp!=NULL){
//         printWorkerNode(tmp);
//         tmp=tmp->next;
//     }
// }

workerDataNode makeWorkerArCell(pid_t pid, char* countries, int totalCountries, int pidRead, int pidWrite, int workerNum){
    workerDataNode cell = malloc(sizeof(workerData));
    if(cell==NULL){ return false; }
    
    cell->pid = pid;
    cell->totalCountries = totalCountries;
    cell->PIDcountries = makeCountryList(countries);
    
    cell->fifoRead = malloc( (sizeof("reader_des")+3)*sizeof(char*) );
    sprintf(cell->fifoRead, "reader_des%d", workerNum);
    mkfifo(cell->fifoRead, 0666);

    cell->fifoWrite = malloc( (sizeof("writer_des")+3)*sizeof(char*) );
    sprintf(cell->fifoWrite, "writer_des%d", workerNum);
    mkfifo(cell->fifoWrite, 0666);

    // cell->fdRead = open(cell->fifoRead, O_RDONLY);
    // close( cell->fdRead );
    
    // cell->fdWrite = open(cell->fifoWrite, O_WRONLY);
    // close( cell->fdWrite );

    return cell;
}

void emptyworkerNode(workerDataNode* wL){
    emptycountryList( &((*wL)->PIDcountries) );
    free((*wL)->PIDcountries);
    unlink((*wL)->fifoRead);
    free((*wL)->fifoRead);
    unlink((*wL)->fifoWrite);
    free((*wL)->fifoWrite);

    // close( (*wL)->fdRead );
    // close( (*wL)->fdWrite );
    
    free(*wL);

}