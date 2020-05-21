#include "../headers/generalFuncs.h"

int returnMaxInt(int a, int b){
    if(a>=b){ return a; }
    return b;
}

int countDirFiles(char* dirNam){
    int count=0;
    struct dirent *entry;

    DIR* dir = opendir(dirNam);
    if (dir) {
        printf("Directory %s exists!\n", dirNam);
    }
    else if (ENOENT == errno) {
        fprintf(stderr, "Directory does not exist!\n");
        return -1;
    }
    else {
        fprintf(stderr, "opendir() failed for some other reason!\n");
        return -1;
    }

    while( (entry=readdir(dir))!=NULL ){
        // if(entry->d_type==DT_REG){  // regular file
        if(entry->d_type==DT_DIR){  // dir
            count++;
        }
    }
    
    closedir(dir);
    return count-2;
}

char** getCountriesDirs(char* dirNam, int cntrs){
    struct dirent *entry;
    DIR* dir = opendir(dirNam);
    if (dir) {
    }
    else if (ENOENT == errno) {
        fprintf(stderr, "Directory does not exist!\n");
        return NULL;
    }
    else {
        fprintf(stderr, "opendir() failed for some other reason!\n");
        return NULL;
    }

    char** dirs = malloc(cntrs*sizeof(char*));
    if(dirs==NULL){
        return NULL;
    }

    int i=0;
    while( (entry=readdir(dir))!=NULL ){
        if( strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0 ){
            dirs[i++] = strdup(entry->d_name);
            if(dirs[i-1]==NULL){
                return NULL;
            }
        }
    }

    closedir(dir);
    return dirs;
}

char** strtokDirs(char *path, int numDirs){   
    char* temp  = strdup(path);
    char* tok   = strtok(temp, " ");
    char** dirs = malloc(numDirs*sizeof(char*));
    tok = strtok(NULL, " ");
    int k=0;
    while(tok!=NULL){
        dirs[k++] = strdup(tok);
        tok = strtok(NULL, " ");
    }
    free(temp);
    return dirs;
}