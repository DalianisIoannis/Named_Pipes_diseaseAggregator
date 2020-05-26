#include "../headers/generalFuncs.h"

int returnMaxInt(int a, int b){
    if(a>=b){ return a; }
    return b;
}

// counts dirs in a dir
int countDirFiles(char* dirNam){
    int count=0;
    struct dirent *entry;
    DIR* dir = opendir(dirNam);
    if (dir) { }//printf("Directory %s exists!\n", dirNam); }
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

int countcountryFiles(char* dirNam){
    int count=0;
    struct dirent *entry;
    DIR* dir = opendir(dirNam);
    if (dir) { printf("Directory %s exists!\n", dirNam); }
    else if (ENOENT == errno) {
        fprintf(stderr, "Directory does not exist!\n");
        return -1;
    }
    else {
        fprintf(stderr, "opendir() failed for some other reason!\n");
        return -1;
    }
    while( (entry=readdir(dir))!=NULL ){
        if(entry->d_type==DT_REG){  // regular file
            count++;
        }
    }
    closedir(dir);
    return count;
}

char** getCountriesDirs(char* dirNam, int cntrs){
    struct dirent *entry;
    DIR* dir = opendir(dirNam);
    if (dir) {}
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

// rerurns array of dirNames
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

int compareDates(char *d1, char *d2){
    if(strcmp(d2,"-")==0){ return -2; }
    if(strcmp(d1,"-")==0){ return -1; }
    char *token, *temp;
    int day1, day2, month1, month2, year1, year2;
    
    temp = strdup(d1);
    if(temp==NULL){
        fprintf(stderr, "Couldn't allocate temp string. Abort...\n");
        exit(1);
    }

    token = strtok(temp,"-");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    day1 = atoi(token);

    token = strtok(NULL,"-");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    month1 = atoi(token);

    token = strtok(NULL,"-\n \t");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    year1 = atoi(token);

    free(temp);
    temp = strdup(d2);
    if(temp==NULL){
        fprintf(stderr, "Couldn't allocate temp string. Abort...\n");
        exit(1);
    }

    token = strtok(temp,"-");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    day2 = atoi(token);
    
    token = strtok(NULL,"-");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    month2 = atoi(token);
    
    token = strtok(NULL,"-\n \t");
    if( isNumber(token)==false ){
        fprintf(stderr, "Date given wrong. Abort...\n");
        exit(1);
    }
    year2 = atoi(token);

    free(temp);

    if(year2>year1){
        return 2;
    }
    else if(year2==year1){
        if(month2>month1){
            return 2;
        }
        else if(month2==month1){
            if(day2>day1){ return 2; }
            else if(day2==day1){ return 0; }
            else{ return 1; }
        }
        else{ return 1; }
    }
    else{ return 1; }
}

bool isNumber(const char *str){
    if( str==NULL || *str=='\0' ){ return false; }
    int minusCount  = 0,
        plusCount   = 0,
        dotCount    = 0;

    while(*str){
        char c = *str;
        switch(c){
            case '.':
                if(++dotCount>1){ return false; }
                break;
            case '-':
                if(++minusCount>1){ return false; }
                break;
            case '+':
                if(++plusCount>1){ return false; }
                break;
            default:
                if( c<'0' || c>'9'){ return false; }
        }
        str++;
    }
    return true;
}

char** WorkerCounts(char** countries, int countriesNum, int workers){
    char** ar = malloc( workers*sizeof(char*) );

    for(int i=0; i<workers; i++){
        ar[i] = NULL;
    }

    int dirsGiven=0;
    int counter=0;
    while(dirsGiven<countriesNum){
        if(ar[counter]==NULL){
            ar[counter] = malloc( (strlen(countries[dirsGiven])+1+2)*sizeof(char) );
            strcpy(ar[counter], "1");
            strcat(ar[counter], " ");
            strcat(ar[counter], countries[dirsGiven]);
        }
        else{
            char* temp  = strdup(ar[counter]);
            free(ar[counter]);
            char* tok = strtok(temp, " ");
            int num   = atoi(tok);
            tok = strtok(NULL, "\t");
            ar[counter] = malloc( (5+1+strlen(tok)+1+strlen(countries[dirsGiven])+1)*sizeof(char) );
            sprintf(ar[counter], "%d", ++num);
            strcat(ar[counter], " ");
            strcat(ar[counter], tok);
            strcat(ar[counter], " ");
            strcat(ar[counter], countries[dirsGiven]);
            free(temp);
            
        }
        counter++;
        if(counter>=workers){
            counter=0;
        }
        dirsGiven++;
    }
    return ar;
}

char* makeFullPathName(char* dir, char* filename){
    char* my_file = malloc( (strlen(dir)+2+strlen(filename))*sizeof(char) );
    strcpy(my_file, dir);
    strcat(my_file, "/");
    strcat(my_file, filename);
    return my_file;
}

void printStatString(char* word){
    // printf("Stats is %s\n", word);
    char* tok = strtok(word, " ");
    printf("File Date %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Country %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Disease %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Patients of age 20 %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Patients of age 40 %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Patients of age 60 %s\n", tok);
    tok = strtok(NULL, " ");
    printf("Patients of age over %s\n", tok);
}
// 12-08-1945 Spain Covid 0 1 0 1