#include "../headers/workers.h"
#include "../headers/countryList.h"
#include "../headers/general.h"
#include "../headers/pipes.h"
#include "../headers/statistics.h"
#include "../headers/linkedList.h"
#include "../headers/patients.h"
#include "../headers/statistics.h"
#include "../headers/HashTable.h"

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

workerDataNode makeWorkerArCell(pid_t pid){
    
    workerDataNode cell = malloc(sizeof(workerData));
    
    if(cell==NULL){ return NULL; }
    
    cell->pid = pid;
    
    // cell->PIDcountries = initcountryList();
    
    cell->fifoRead = malloc( (strlen("./pipeFiles/reader_des")+12) );
    sprintf((cell)->fifoRead, "./pipeFiles/reader_des%d", pid);
    if (mkfifo(cell->fifoRead, 0666) == -1) {
        perror("mkfifor");
        return NULL;
    }

    (cell)->fifoWrite = malloc( (strlen("./pipeFiles/writer_des")+12) );
    sprintf((cell)->fifoWrite, "./pipeFiles/writer_des%d", pid);
    if (mkfifo(cell->fifoWrite, 0666) == -1) {
        perror("mkfifo");
        return NULL;
    }

    if ((cell->fdRead = open(cell->fifoRead, O_RDONLY)) == -1) {
        perror("open");
    }
    // close( cell->fdRead );
    
    if ((cell->fdWrite = open(cell->fifoWrite, O_WRONLY)) == -1) {
        perror("open");

    }
    // close( cell->fdWrite );

    return cell;
}

void emptyworkerNode(workerDataNode *wL){
    
    // emptycountryList( &((*wL)->PIDcountries) );
    // free((*wL)->PIDcountries);

    // unlink((*wL)->fifoRead);
    free((*wL)->fifoRead);

    // unlink((*wL)->fifoWrite);
    free((*wL)->fifoWrite);

    // close( wL->fdRead );
    // close( wL->fdWrite );
    
    free(*wL);

}

int WorkerRun(char* inDir, int bufferSize, int rfd, int wfd, workerDataNode node){

    char arr[bufferSize];
    char* readed = receiveMessage(rfd, arr, bufferSize);
    struct dirent *entry;
    printf("%s\n", readed);
    free(readed);

    // node->PIDcountries = initcountryList();
    CountryList cL = initcountryList();
    characteristic charsList = initChar();
    Linked_List ListOfPatients = initlinkedList();

    for( ; ; ){ // read countries
        
        readed = receiveMessage(rfd, arr, bufferSize);

        if(strcmp(readed, "OK")==0){
            free(readed);
            break;
        }
        
        // addCountryListNode(&(node->PIDcountries), readed);
        addCountryListNode(&(cL), readed);

        free(readed);
    }

    // for countries take patients
    countrylistNode tmpNode = cL->front;

    while(tmpNode!=NULL) {
        
        char* CountryDir = malloc( (strlen(inDir)+strlen(tmpNode->country)+1)*sizeof(char) );
        strcpy(CountryDir, inDir);
        strcat(CountryDir, tmpNode->country);
        // // printf("Final file is %s\n", CountryDir);
        
        DIR* dirr = opendir(CountryDir);
        
        if (ENOENT == errno) {
            fprintf(stderr, "Directory does not exist!\n");
            return -1;
        }
        else if (!dirr) {
            fprintf(stderr, "opendir() failed for some other reason!\n");
            return -1;
        }

        while( (entry=readdir(dirr))!=NULL ) {
            if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0) {
            
                StatisticsList charsForDateFile = initStatisticsList();

                // printf("Date file %s\n", entry->d_name);

                char* countryDateFile = malloc( (strlen(CountryDir)+strlen(entry->d_name)+1+1)*sizeof(char) );
                strcpy(countryDateFile, CountryDir);
                strcat(countryDateFile, "/");
                strcat(countryDateFile, entry->d_name);

                FILE *fp = fopen(countryDateFile, "r");
                // 
                if(fp==NULL) {
                    perror("FILE.");
                }
                // else { printf("Opened file %s\n", countryDateFile); }
                
                char* buffer = NULL;
                size_t size = 0;
                while( getline(&buffer, &size, fp)>=0 ) {

                    inputPatientsToStructures(buffer, &(ListOfPatients), entry->d_name, tmpNode->country, &charsForDateFile);  // every line ends with /n
                    
                    free(buffer);
                    buffer = NULL;
                }

                // send statistics
                // printStatList(charsForDateFile);
                StatsNode tmpStatsNode = charsForDateFile->front;
                while(tmpStatsNode!=NULL) {

                    char* stringStats = concatStats(tmpStatsNode->item);
                    // send stats
                    sendMessage(wfd, stringStats, bufferSize);
                    free(stringStats);

                    tmpStatsNode = tmpStatsNode->next;

                }
                
                emptyStatisticsList(&charsForDateFile);

                free(countryDateFile);
                free(buffer);
                fclose(fp);
            
            }
        }

        free(CountryDir);
        closedir(dirr);
        tmpNode = tmpNode->next;
    }

    sendMessage(wfd, "OK", bufferSize);


    // printCountryList(cL);

    // input to Data Structures
    int diseaseHashtableNumOfEntries = 8;
    int countryHashtableNumOfEntries = 3;
    int bucketSize = 24;

    HashTable   HT_disease, HT_country;
    if( (HT_disease = initHashTable(bucketSize, diseaseHashtableNumOfEntries))==NULL ){
        fprintf(stderr, "Couldn't allocate Hash Table. Abort...\n");
        return false;
    }

    inputLLtoHT(ListOfPatients, HT_disease, 0);    // 0 for disease
    
    // printf("Hash Table of Diseases is:\n"); printHashTable(HT_disease);

    if( (HT_country = initHashTable(bucketSize, countryHashtableNumOfEntries))==NULL ){
        fprintf(stderr, "Couldn't allocate Hash Table. Abort...\n");
        return false;
    }

    inputLLtoHT(ListOfPatients, HT_country, 1);    // 1 for country

    // printf("Hash Table of Countries is:\n"); printHashTable(HT_country);
    
    // read from Father Querries
    for( ; ; ) { // read querries
        
        readed = receiveMessage(rfd, arr, bufferSize);

        if(strcmp(readed, "OK")==0){
            free(readed);
            break;
        }
        
        printf("Querry %s\n", readed);
        
        if(strcmp(readed, "/listCountries")==0) {
            // print countries with pid

            countrylistNode node = cL->front;
            while(node!=NULL) {
                
                char* tmp = malloc( (strlen(node->country)+12+1) );
                strcpy(tmp, node->country);
                strcat(tmp, " ");
                char* tmpPIDstring = malloc( 12 );
                sprintf(tmpPIDstring, "%d", getpid());
                strcat(tmp, tmpPIDstring);

                sendMessage(wfd, tmp, bufferSize);

                free(tmp);
                free(tmpPIDstring);
                node = node->next;

            }

            sendMessage(wfd, "OK", bufferSize);

            // printCountryList(cL);

        }
        else {
         
            char* instruct = strtok(readed," ");
            char* ind1 = strtok(NULL," ");
            char* ind2 = strtok(NULL," ");
            char* ind3 = strtok(NULL," ");
            char* ind4 = strtok(NULL," ");
            char* ind5 = strtok(NULL," ");

            if( strcmp(instruct, "/diseaseFrequency")==0 ){ // diseaseFrequency virusName date1 date2 [country]
                // instruct diseaseFrequency
                // ind1     virusName has to be not NULL
                // ind2     date1 has to be not NULL
                // ind3     date2 has to be not NULL
                // ind4     country can be NULL
                if( ind1==NULL || ind2==NULL || ind3==NULL ){
                    printf("Need to provide proper variables.\n\n");
                }
                else{
                    int compDat = compareDates(ind2, ind3);
                    if(compDat!=0 && compDat!=2){
                        printf("Give correct dates.\n\n");
                    }
                    else{
                        if( ind4==NULL ){ // didn't give country
                            
                            int res = diseaseFrequencyNoCountry(HT_disease, ind1, ind2, ind3);
                            char* newInt = malloc(12);
                            sprintf(newInt, "%d", res);
                            
                            sendMessage(wfd, newInt, bufferSize);
                            free(newInt);

                        }
                        else{
                            
                            int res = diseaseFrequencyCountry(HT_country, ind1, ind4, ind2, ind3);
                            if(res!=0) {
                                printf("%d\n", res);
                            }

                        }
                    }
                }
            }
            else if(strcmp(instruct, "/topk-AgeRanges")==0) {
                if( ind5==NULL ){
                    printf("Need to provide proper variables.\n\n");
                }
                else {
                    
                    topkAgeRanges(HT_country, ind1, ind2, ind3, ind4, ind5);
                    
                }
            }
            else if(strcmp(instruct, "/searchPatientRecord")==0) {

                // printf("search for id %s\n", ind1);
                // printLinkedList(ListOfPatients);
                char* receive = returnPatientifExists(ListOfPatients, ind1);
                // printf("Got %s\n", receive);
                if(receive!=NULL) {
                    sendMessage(wfd, receive, bufferSize);
                }
                else {
                    sendMessage(wfd, "WRONG", bufferSize);
                }
                free(receive);

            }
        
        }

        free(readed);
    }

    deleteHT(HT_disease);
    deleteHT(HT_country);
    
    freeStats(charsList);

    // printLinkedList(ListOfPatients);
    emptyLinkedList(&ListOfPatients);
    free(ListOfPatients);

    emptycountryList(&cL);
    free(cL);
    return 0;
}

int inputPatientsToStructures(char* line, Linked_List *ListOfPatients, char* date, char* country, StatisticsList* charsForDateFile) {

    Linked_List ListOfEXITPatients = initlinkedList();

    char* tmp = strdup(line);
    char* tok = strtok(tmp, " ");
    tok = strtok(NULL, " ");

    patientRecord pR = initRecord(line, date, country);

    if(strcmp(tok, "ENTER")==0) {
        
        addNode(ListOfPatients, pR);

        informCharList(charsForDateFile, pR);

    }
    else {

        addNode(&ListOfEXITPatients, pR);

    }

    // check if EXITS exist in ENTRIES
    listNode looker = ListOfEXITPatients->front;
    while(looker!=NULL) {

        updateExitDate(ListOfPatients, looker->item);

        looker = looker->next;
    }

    emptyLinkedList(&ListOfEXITPatients);
    free(ListOfEXITPatients);

    free(tmp);
    return 0;
}