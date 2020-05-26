#include "../headers/generalFuncs.h"
#include "../headers/workerInfo.h"
#include "../headers/pipes.h"

int main(int argc, char** argv){
    // int pipeSize = fpathconf(workArray[i]->fdRead, _PC_PIPE_BUF);
    // for(int i = 0; i < argc; i++){ printf("for %d have %s\n", i, argv[i]); }
    // argv[0] ./diseaseAggregator
    // argv[1] -w
    // argv[2] numWorkers
    // argv[3] –b
    // argv[4] bufferSize
    // argv[5] –i
    // argv[6] input_dir

    if(argv[6]==NULL){
        fprintf(stderr, "Command must be in form: ./diseaseAggregator –w numWorkers -b bufferSize -i input_dir!\n");
        exit(1);
    }
    int bufferSize = 1;
    DIR* dir = opendir(argv[6]);
    int numWorkers = atoi(argv[2]);
    workerDataNode* workArray = malloc(numWorkers*sizeof(workerDataNode));
    int totalCountries = countDirFiles(argv[6]);    // total country Dirs
    printf("Directories are %d\n", totalCountries);
    
    if( totalCountries<numWorkers ){
        closedir(dir);
        fprintf(stderr, "Country Directories should be at least equal to numWorkers!\n");
        exit(1);
    }
    
    char** countriesDirs = getCountriesDirs(argv[6], totalCountries);
    
    for(int i=0; i<totalCountries; i++){ printf("%s\n", countriesDirs[i]); } printf("\n");
    
    char** countriesPerWorker =  WorkerCounts(countriesDirs, totalCountries, numWorkers);
    
    for(int i=0; i<numWorkers; i++){ printf("For worker %d %s\n", i, countriesPerWorker[i]); }


    for(int i=0; i<numWorkers; i++){
        workArray[i] = makeWorkerArCell(getpid(), countriesPerWorker[i], 5, 5, 5, i);
    }

    for(int i=0; i<numWorkers; i++){
        workArray[i]->pid = fork();

        if(workArray[i]->pid<0){
            perror("Fork");
            exit(1);
        }

        if (workArray[i]->pid==0) {    // i am child

            execl("./worker", "worker", workArray[i]->fifoRead, countriesPerWorker[i], argv[6], workArray[i]->fifoWrite, NULL);
        }
    }

    for(int i=0; i<numWorkers; i++){    // i am father
        workArray[i]->fdRead = open(workArray[i]->fifoRead, O_RDONLY);
        workArray[i]->fdWrite = open(workArray[i]->fifoWrite, O_WRONLY);
    }

    for(int i=0; i<numWorkers; i++){
    
        char arr1[5];
        char* ret=receiveMessage(workArray[i]->fdRead, arr1, bufferSize);
        printf("Stats send are %d\n", atoi(ret));
        for(int j=0; j<atoi(ret); j++){
            char arr3[5];
            char* ret3=receiveMessage(workArray[i]->fdRead, arr3, bufferSize);
            // printStatString(ret3);
            free(ret3);
        }
        free(ret);

        char arr2[5];
        char* ret2=receiveMessage(workArray[i]->fdRead, arr2, bufferSize);
        printf("RET is %s\n", ret2);
        free(ret2);

        // char *writer = malloc( (strlen("write_from_parent")+3)*sizeof(char) );
        // sprintf(writer, "write_from_parent%d", i);
        // sendMessage(workArray[i]->fdWrite, writer, bufferSize);
        // free(writer);
    }
    for(int i=0; i<numWorkers; i++){
        close(workArray[i]->fdRead);
        close(workArray[i]->fdWrite);
    }
    wait(0);

    for(int i=0; i<totalCountries; i++){
        free(countriesDirs[i]);
    }
    free(countriesDirs);
    for(int i=0; i<numWorkers; i++){
        free(countriesPerWorker[i]);
    }
    free(countriesPerWorker);
    closedir(dir);
    for(int i=0; i<numWorkers; i++){
        // printWorkerNode(workArray[i]);
        emptyworkerNode(&(workArray[i]));
    }
    free(workArray);
    return 0;
}