#include "../headers/generalFuncs.h"

int main(int argc, char** argv){
    // ./diseaseAggregator –w numWorkers -b bufferSize -i input_dir
    for(int i = 0; i < argc; i++){
        printf("for %d have %s\n", i, argv[i]);
    }
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
    if (dir) {
        printf("Directory exists!\n");
        closedir(dir);
    } else if (ENOENT == errno) {
        fprintf(stderr, "Directory does not exist!\n");
        exit(1);
    } else {
        fprintf(stderr, "opendir() failed for some other reason!\n");
        exit(1);
    }

    // if( isNumber(argv[4])==false || isNumber(argv[6])==false || isNumber(argv[8])==false ){
    //     fprintf(stderr, "diseaseHashtableNumOfEntries, countryHashtableNumOfEntries and bucketSize must be numbers. Abort...\n");
    //     exit(1);
    // }
    
    
    printf("5\n");

    return 0;
}