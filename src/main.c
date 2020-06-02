#include "../headers/general.h"
#include "../headers/workers.h"
#include "../headers/countryList.h"
#include "../headers/pipes.h"
#include "../headers/fatherFunctions.h"
#include "../headers/statistics.h"
#include "../headers/signals.h"

int main(int argc, char **argv)
{
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

    int bufferSize = atoi(argv[4]);
    int numWorkers = atoi(argv[2]);
    workerDataNode* WorkersArr;
    
    char *input_dir = strdup(argv[6]);
    DIR* dir = opendir(input_dir);
    closedir(dir);

    WorkersArr = NULL;
    if ( (WorkersArr = malloc(numWorkers*sizeof(workerDataNode*))) == NULL ) {
        perror("WorkersArr malloc");
        exit(1);
    }

    termination = 0;
    sigkill = 0;
    struct sigaction *act = malloc(sizeof(struct sigaction));
    HandlerInit(act, handler);

    pid_t pid = 0;
    for (int i=0; i < numWorkers; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Fork");
            exit(1);
        }

        else if (pid == 0) {    // child

            int rfd, wfd;

            char* fiforead = malloc( (strlen("./pipeFiles/reader_des")+12) );
            sprintf(fiforead, "./pipeFiles/reader_des%d", getpid());

            char *fifowrite = malloc((strlen("./pipeFiles/writer_des")+12));
            sprintf(fifowrite, "./pipeFiles/writer_des%d", getpid());

            if ((wfd = open(fiforead, O_WRONLY)) == -1) {
                perror("open wfd");

            }
            if ((rfd = open(fifowrite, O_RDONLY)) == -1) {
                perror("open rfd");
            }

            WorkerRun(input_dir, bufferSize, rfd, wfd, WorkersArr[i]);

            for(int j=0; j < i; j++) {

                emptyworkerNode(&WorkersArr[j]);

            }

            free(WorkersArr);

            free(fiforead);
            free(fifowrite);
            free(input_dir);

            free(act);
            
            exit(0);
        }

        else {
            if ((WorkersArr[i] = makeWorkerArCell(pid)) == NULL)
            {
                perror("makeWorkerArCell failed\n");
            }
        }
    }

    // countries for every pid
    CountryList* countriesListArray = malloc(numWorkers*sizeof(CountryList));
    for (int i=0; i < numWorkers; i++) {
        countriesListArray[i]=initcountryList();
    }

    sendCountriesToWorkers(WorkersArr, input_dir, numWorkers, bufferSize, countriesListArray);

    // get statistics
    for (int i=0; i < numWorkers; i++) {
        for( ; ; ) {

            char arr[bufferSize];
            char* readed = receiveMessage(WorkersArr[i]->fdRead, arr, bufferSize);

            if(strcmp(readed, "OK")==0){
                free(readed);
                break;
            }
            
            // printStatsFromConcat(readed);
            
            free(readed);

        }
    }

    FatherQuerries(WorkersArr, numWorkers, bufferSize, countriesListArray);

    for(int i=0; i < numWorkers; i++) {
        wait(NULL);
    }
    
    for (int i=0; i < numWorkers; i++) {
        emptycountryList( &(countriesListArray[i]) );
        free(countriesListArray[i]);
    }
    free(countriesListArray);

    for(int i=0; i < numWorkers; i++) {

        unlink(WorkersArr[i]->fifoRead);
        unlink(WorkersArr[i]->fifoWrite);

        close( WorkersArr[i]->fdRead );
        close( WorkersArr[i]->fdWrite );

        emptyworkerNode(&(WorkersArr[i]));
    
    }
    free(WorkersArr);

    free(act);

    free(input_dir);

    return 0;
}