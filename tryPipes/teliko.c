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

    for(int i=0; i<stSize/buf_size; i++){
        write(fd, str+(i*buf_size), buf_size);
    }
    if(stSize%buf_size!=0){
        write(fd, str+(stSize-stSize%buf_size), buf_size);
    }

    for(int i=0; i<bufLen/buf_size; i++){
        write(fd, buf+(i*buf_size), buf_size);
    }

    if(bufLen%buf_size!=0){
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

int main (int argc, char** argv) {
    int fd_Write = open(argv[1], O_WRONLY);
    int fd_Read = open(argv[4], O_RDONLY);

    // int pipeSize = fpathconf(fd_Write, _PC_PIPE_BUF);
    // write(fd_Write, "p", pipeSize);

    int countFiles = 8;
    printf("countFiles exits with %d\n", countFiles);


    // encodePipe(fd_Write, "5", 1);
    for(int i=0; i<5; i++){
        // int pipeSize = fpathconf(fd_Write, _PC_PIPE_BUF);
        // write(fd_Write, "kkk", pipeSize);
        encodePipe(fd_Write, "kkkkkkkkkkkkkkkkkkkk", 32);
    }

    // int pipeSize = fpathconf(fd_Read, _PC_PIPE_BUF);
    // char arr1[pipeSize];
    // read(fd_Read, arr1, pipeSize);
    // printf("Read from parent: %s\n", arr1);

    close(fd_Write);
    close(fd_Read);
    exit(0);
}