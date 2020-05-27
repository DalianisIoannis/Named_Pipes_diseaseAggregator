#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

int encodePipe(int fd, char* buf, int buf_size){
    char str[12] = {0};
    // printf("Want to write %s\n", buf);
    // printf("XIXI %s\n", str);
    int stSize = sizeof(str);
    int bufLen = strlen(buf)+1;

    sprintf(str, "%u", bufLen);
    // sprintf(str, "%d", bufLen);
    // printf("to str %s\n", str);

    for(int i=0; i<stSize/buf_size; i++){
        write(fd, str+(i*buf_size), buf_size);
    }
    if(stSize%buf_size){
        write(fd, str+(stSize-stSize%buf_size), buf_size);
    }

    for(int i=0; i<bufLen/buf_size; i++){
        write(fd, buf+(i*buf_size), buf_size);
    }

    if(bufLen%buf_size){
        write(fd, buf+(bufLen-bufLen%buf_size), buf_size);
    }
    return 0;    
}

char* decodePipe(int fd, char* buf, int buf_size){
    char* return_buf = NULL;
    char str[12] = {0};
    int stSize = sizeof(str);
    int return_bufSize = 0;
    
    for(int i=0; i<stSize/buf_size; i++){
        read(fd, buf, buf_size);
        memcpy(str+(i*buf_size), buf, buf_size);
    }
    if(stSize%buf_size!=0){
        read(fd, buf, buf_size);
        memcpy(str+(stSize/buf_size)*buf_size, buf, stSize-(stSize/buf_size)*buf_size);
    }
    return_bufSize = (int)strtol(str, NULL, 10);
    if( (return_buf=malloc(return_bufSize))==NULL ){
        printf("malloc ERROR\n");
        return NULL;
    }
    for(int i=0; i<return_bufSize/buf_size; i++){
        read(fd, buf, buf_size);
        memcpy(return_buf+(i*buf_size), buf, buf_size);
    }

    if(return_bufSize%buf_size!=0){
        read(fd, buf, buf_size);
        memcpy(return_buf+(return_bufSize/buf_size)*buf_size, buf, return_bufSize-(return_bufSize/buf_size)*buf_size);
    }

    return return_buf;
}


int main(int argc, char** argv){
    int numWorkers=2;
    pid_t Workers[numWorkers];
    int i, fdRead[numWorkers], fdWrite[numWorkers];

    char **fifoRead = malloc(numWorkers*sizeof(char*));
    char **fifoWrite = malloc(numWorkers*sizeof(char*));
    for(i=0; i<numWorkers; i++){
        fifoRead[i] = malloc( (sizeof("reader_des")+3)*sizeof(char*) );
        sprintf(fifoRead[i], "reader_des%d", i);
        mkfifo(fifoRead[i], 0666);
        
        fifoWrite[i] = malloc( (sizeof("writer_des")+3)*sizeof(char*) );
        sprintf(fifoWrite[i], "writer_des%d", i);
        mkfifo(fifoWrite[i], 0666);
    }
    for(i=0; i<numWorkers; i++){
        Workers[i] = fork();
        srand(getpid());
        if(Workers[i]<0){
            perror("Fork");
            exit(1);
        }
        if (Workers[i]==0) {    // i am child
            execl("./teliko", "teliko", fifoRead[i], "10", "10", fifoWrite[i], NULL);
        }
    }
    for(int i=0; i<numWorkers; i++){    // i am father
        fdRead[i] = open(fifoRead[i], O_RDONLY);
        fdWrite[i] = open(fifoWrite[i], O_WRONLY);
    }
    for(int i=0; i<numWorkers; i++){
        // int pipeSize = fpathconf(fdRead[i], _PC_PIPE_BUF);
        // char arr1[pipeSize];
        // read(fdRead[i], arr1, pipeSize);
        // printf("NUM RET: %s\n", arr1);

        int pipeSize = fpathconf(fdRead[i], _PC_PIPE_BUF);
        char arr2[pipeSize];
        // char* ret=decodePipe(fdRead[i], arr2, 1);
        // printf("Will happen %s times\n", ret);
        // int numRet = atoi(ret);
        // printf("Will happen %d times\n", numRet);
        // free(ret);
        // while(read(fdRead[i], arr2, pipeSize)>0){
            // read(fdRead[i], arr2, pipeSize);
            // printf("Read from child: %s\n", arr2);
        // }
        for(int j=0; j<5; j++){
            char *ret=decodePipe(fdRead[i], arr2, 32);
            printf("RET is %s\n", ret);
            free(ret);
        }

        // char *writer = malloc( (strlen("write_from_parent")+3)*sizeof(char) );
        // sprintf(writer, "write_from_parent%d", i);
        // write(fdWrite[i], writer, strlen(writer)+1);
        // free(writer);
    }
    for(int i=0; i<numWorkers; i++){
        close(fdWrite[i]);
        close(fdRead[i]);
    }
    wait(0);

    for(i=0; i<numWorkers; i++){
        unlink(fifoRead[i]);
        free(fifoRead[i]);

        unlink(fifoWrite[i]);
        free(fifoWrite[i]);
    }
    free(fifoRead);
    free(fifoWrite);


    return 0;
}