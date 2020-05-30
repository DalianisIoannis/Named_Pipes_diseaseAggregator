#include "../headers/general.h"
#include "../headers/workers.h"
#include "../headers/countryList.h"
#include "../headers/pipes.h"
#include "../headers/fatherFunctions.h"
#include "../headers/statistics.h"

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
    printf("Number of Workers is %d.\n", numWorkers);
    printf("Buffersize is %d.\n", bufferSize);
    
    char *input_dir = strdup(argv[6]);
    printf("Input Dir is %s.\n", input_dir);
    DIR* dir = opendir(input_dir);
    closedir(dir);


    workerDataNode* WorkersArr = NULL;
    if ( (WorkersArr = malloc(numWorkers*sizeof(workerDataNode*))) == NULL ) {
        perror("WorkersArr malloc");
        exit(1);
    }

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
            // if ((wfd = open(WorkersArr[i]->fifoRead, O_WRONLY)) == -1) {
                perror("open wfd");

            }
            if ((rfd = open(fifowrite, O_RDONLY)) == -1) {
                perror("open rfd");
            }

            WorkerRun(input_dir, bufferSize, rfd, wfd, WorkersArr[i]);

            // char arr[bufferSize];
            // char* readed = receiveMessage(rfd, arr, bufferSize);
            // printf("%s\n", readed);
            // free(readed);

            for(int j=0; j < i; j++) {

                emptyworkerNode(&WorkersArr[j]);

            }

            free(WorkersArr);

            free(fiforead);
            free(fifowrite);
            free(input_dir);
            
            exit(0);
        }

        else {
            if ((WorkersArr[i] = makeWorkerArCell(pid)) == NULL)
            {
                perror("makeWorkerArCell failed\n");
            }
            sendMessage(WorkersArr[i]->fdWrite, "k", bufferSize);
        }
    }

    sendCountriesToWorkers(WorkersArr, input_dir, numWorkers, bufferSize);

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

    // int maxfd;
    // fd_set read_fds;

    // for ( ;; ) {

    //     FD_ZERO(&read_fds)

    //     for (int i =0; i < numWorkers; i++) {
    //         FD_SET(workers_arr[i].rfd, &read_fds);
            
    //     }

    //     // find max rfd

    //     select(maxfd + 1, &read_fds, null, null, null)

    //     int fd
    //     for (int i =0; i < numWorkers; i++) {
    //         FD_ISSET(workers_arr[i].rfd, &read_fds) {
    //             fd = workers_arr[i].rfd
    //         }
    //     }

    //     statisticks = read(fd)
    //     if (statistics == "OK") {
    //         count ++;
    //         if (count == numWorkers) {
    //             break;
    //         }
    //         else {
    //             //store statictis 
    //         }
    //     }
    // }


    // Querries
    FatherQuerries(WorkersArr, numWorkers, bufferSize);
    // while(getline(stdin)) {
    //     send wy
    // }


 
    for(int i=0; i < numWorkers; i++) {
        wait(NULL);
    }

    for(int i=0; i < numWorkers; i++) {

        // emptycountryList( &(WorkersArr[i]->PIDcountries) );
        // free(WorkersArr[i]->PIDcountries);

        unlink(WorkersArr[i]->fifoRead);
        unlink(WorkersArr[i]->fifoWrite);

        emptyworkerNode(&(WorkersArr[i]));
    
    }
    free(WorkersArr);

    free(input_dir);
    return 0;
}