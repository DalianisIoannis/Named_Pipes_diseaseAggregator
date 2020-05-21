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

int countDirFiles(char*);
char** getCountriesDirs(char*, int);
char** strtokDirs(char*, int);