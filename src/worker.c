#include "../headers/generalFuncs.h"

int main (int argc, char** argv) {
    // printf("Execed for FIFO %s \n", argv[1]);
    // printf("Gave string %s \n", argv[2]);
    // Open FIFO for write only
    struct dirent *entry;
    int fd = open(argv[1], O_WRONLY);
    char *arr2 = strdup(argv[1]);
    if(arr2==NULL){
        perror("strdup");
        exit(1);
    }
    
    char* temp = strdup(argv[2]);
    char* tok = strtok(temp, " ");
    int tot = atoi(tok);
    char** dirCat = strtokDirs(argv[2], tot);
    
    for(int i=0; i<tot; i++){
        char* tmpStr = strdup(dirCat[i]);
        free(dirCat[i]);
        dirCat[i] = malloc( (strlen(argv[3])+1+strlen(tmpStr))+sizeof(char) );
        strcpy(dirCat[i], argv[3]);
        strcat(dirCat[i], tmpStr);
        free(tmpStr);
        printf("For %d %s\n", i, dirCat[i]);

        DIR* dir = opendir(dirCat[i]);
        if (dir) {
            printf("opened %s\n", dirCat[i]);
        }
        else if (ENOENT == errno) {
            fprintf(stderr, "Directory does not exist!\n");
            exit(-1);
        }
        else {
            fprintf(stderr, "opendir() failed for some other reason!\n");
            exit(-1);
        }
        closedir(dir);
    }

    write(fd, arr2, strlen(arr2)+1);
    printf("Writing done\n\n");

    for(int i=0; i<tot; i++){
        free(dirCat[i]);
    }
    
    free(temp);
    free(dirCat);
    close(fd);
    free(arr2);
    exit(0);
}