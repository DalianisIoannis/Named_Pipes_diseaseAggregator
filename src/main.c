#include "../headers/generalFuncs.h"

int main(int argc, char** argv){
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

    DIR* dir = opendir(argv[6]);
    int totalCountries = countDirFiles(argv[6]);
    printf("Total directories %d\n", totalCountries);

    char** countriesDirs = getCountriesDirs(argv[6], totalCountries);
    for(int i=0; i<totalCountries; i++){
        printf("%s\n", countriesDirs[i]);
    }
    printf("\n");

    int numWorkers = atoi(argv[2]);
    pid_t Workers[numWorkers];
    int i;
    int fdRead[numWorkers], fdWrite[numWorkers];

    char **FIFOar = malloc(numWorkers*sizeof(char*));
    for(i=0; i<numWorkers; i++){
        FIFOar[i] = malloc( (sizeof("descriptor")+3)*sizeof(char*) );
        sprintf(FIFOar[i], "descriptor%d", i);
        mkfifo(FIFOar[i], 0666);
    }

    for(i=0; i<numWorkers; i++){
        Workers[i] = fork();
        srand(getpid());
        if(Workers[i]<0){
            perror("Fork");
            exit(1);
        }
        if (Workers[i]==0) {    // i am child
            int randomNumDirs = rand()%totalCountries;
            while(randomNumDirs==0){
                randomNumDirs = rand()%totalCountries;
            }

            int strSize=0;
            for(int i=0; i<randomNumDirs; i++){
                strSize=strSize+(strlen(countriesDirs[i])+1)*sizeof(char);  // for space
            }
            
            char numAsStr[5];
            sprintf(numAsStr, "%d", randomNumDirs);
            char *conStr = malloc((strSize+1+strlen(numAsStr)+1)*sizeof(char));
            strcpy(conStr, numAsStr);
            strcat(conStr, " ");
            for(int i=0; i<randomNumDirs; i++){
                strcat(conStr, countriesDirs[i]);
                strcat(conStr, " ");
            }
            printf("Concatenated %s\n", conStr);
            execl("./worker", "worker", FIFOar[i], conStr, argv[6], NULL);
            // free(conStr);
        }
        // } else {    // i am father 
        //     // fdRead[i] = open(FIFOar[i], O_RDONLY);
        //     int pipeSize = fpathconf(fdRead[i], _PC_PIPE_BUF);
        //     char arr1[pipeSize];
        //     read(fdRead[i], arr1, pipeSize);
        //     printf("User2: %s\n", arr1);
        //     // close(fdRead[i]);
        //     // wait(0);
        // }
    }
    for(int i=0; i<numWorkers; i++){
        fdRead[i] = open(FIFOar[i], O_RDONLY);
        int pipeSize = fpathconf(fdRead[i], _PC_PIPE_BUF);
        char arr1[pipeSize];
        read(fdRead[i], arr1, pipeSize);
        printf("User2: %s\n", arr1);
        close(fdRead[i]);
    }
    wait(0);
    
    for(i=0; i<numWorkers; i++){
        unlink(FIFOar[i]);
        free(FIFOar[i]);
    }
    for(int i=0; i<totalCountries; i++){
        free(countriesDirs[i]);
    }
    free(FIFOar);
    free(countriesDirs);
    closedir(dir);

    return 0;
}