#include "../headers/generalFuncs.h"
#include "../headers/patients.h"
#include "../headers/AVL.h"
#include "../headers/linkedList.h"
#include "../headers/HashTable.h"
#include "../headers/statistics.h"
#include "../headers/pipes.h"

int main (int argc, char** argv) {
    // int pipeSize = fpathconf(fd_Write, _PC_PIPE_BUF);
    struct dirent *entry;
    int bufferSize = 1;
    int fd_Write = open(argv[1], O_WRONLY);
    int fd_Read = open(argv[4], O_RDONLY);

    char *arr2 = strdup(argv[1]);
    if(arr2==NULL){
        perror("strdup");
        exit(1);
    }
    char* temp = strdup(argv[2]);
    char* tok = strtok(temp, " ");
    int tot = atoi(tok);    // num of country files
    char** dirCat = strtokDirs(argv[2], tot);
    
    AVLTreePtr WorkerAVL = initAVLTree();
    Linked_List ll = initlinkedList();
    HashTable WorkerHT = initHashTable(24, 5);

    StatisticsList statList = initStatisticsList();

    for(int i=0; i<tot; i++){   // for every dir
        char* tmpStr = strdup(dirCat[i]);
        free(dirCat[i]);
        dirCat[i] = malloc( (strlen(argv[3])+1+strlen(tmpStr))*sizeof(char) );
        strcpy(dirCat[i], argv[3]);
        // printf("Path country %s\n", tmpStr);
        strcat(dirCat[i], tmpStr);
        
        DIR* dir = opendir(dirCat[i]);
        if (dir) {
            int numFiles = countcountryFiles(dirCat[i]);
            printf("Oppened dir %s and has %d files.\n", dirCat[i], numFiles);
            

            int countFiles=0;
            while( (entry=readdir(dir))!=NULL ){
                if( strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0 ){
                                        
                    char* date = strdup(entry->d_name);
                    char* my_file = makeFullPathName(dirCat[i], entry->d_name);
                    // printf("File %s\n", my_file);

                    FILE *fp = fopen(my_file, "r");
                    if (fp == NULL){
                        fprintf(stderr, "Cannot open file!\n");
                        exit(-1);
                    }
                    else{
                        char *buffer = NULL;
                        size_t size = 0;
                        while( getline(&buffer, &size, fp)>=0 ){
                            // printf("Buffer is %s\n", buffer);
                            // printf("Path country %s\n", dirCat[i]);
                            patientRecord pR = initRecord(buffer, date, tmpStr);
                            informCharList(&statList, pR);
                            // printRecord(pR);

                            // if(addAVLNode(WorkerAVL, pR, NULL)==false){
                            //     // printf("Such recordId already exists!\n");
                            //     deleteRecord(&pR);
                            // }
                            // // else{ printRecord(pR); }
                            // // deleteRecord(&pR);
                            // 0 for disease
                            if(strcmp(pR->situation, "EXIT")==0){
                                addNode(&ll, pR);
                            }
                            else{
                                if(addHT(WorkerHT, pR, 0)==false){
                                    printf("addHT false!\n");
                                    deleteRecord(&pR);
                                }
                            }

                            free(buffer);
                            buffer = NULL;
                        }
                        free(buffer);
                    }
                    free(date);
                    fclose(fp);
                    free(my_file);
                    countFiles++;
                }
            }
            // printf("countFiles exits with %d\n", countFiles);

        }
        else if (ENOENT == errno) {
            fprintf(stderr, "Directory does not exist!\n");
            exit(-1);
        }
        else {
            fprintf(stderr, "opendir() failed for some other reason!\n");
            exit(-1);
        }
        free(tmpStr);
        closedir(dir);
    }
    int counting=0;
    StatsNode tmpNode = statList->front;
    while(tmpNode!=NULL){
        tmpNode=tmpNode->next;
        counting++;
    }
    // printf("Counted %d\n", counting);
    char* numRet = malloc(sizeof(int));
    sprintf(numRet, "%d", counting);
    // printf("EEEEEEEEE %s\n", numRet);
    sendMessage(fd_Write, numRet, bufferSize);
    free(numRet);

    // concat stats
    tmpNode = statList->front;
    while(tmpNode!=NULL){
        char* returner = (concatStats(tmpNode->item));
        tmpNode=tmpNode->next;
        sendMessage(fd_Write, returner, bufferSize);
        free(returner);
    }
    // printStatList(statList);
    emptyStatisticsList(&statList);
    sendMessage(fd_Write, "kkkkkkkkkkkkkkkkkkkk", bufferSize);

    // char arr1[5];
    // char* ret=receiveMessage(fd_Read, arr1, bufferSize);
    // printf("Read from parent: %s\n", ret);
    // free(ret);

    for(int i=0; i<tot; i++){
        free(dirCat[i]);
    }

    // printAVLTree(WorkerAVL);
    // printHashTable(WorkerHT);

    emptyLinkedList(&ll);
    free(ll);

    emptyAVLTree(WorkerAVL);
    deleteHT(WorkerHT);

    free(temp);
    free(dirCat);
    close(fd_Write);
    close(fd_Read);    
    free(arr2);
    exit(0);
}