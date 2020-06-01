#include "../headers/signals.h"
#include "../headers/general.h"
#include "../headers/workers.h"


void HandlerInit(struct sigaction *act, void (*handler)(int, siginfo_t*, void*)) {
    
    act->sa_handler = (void*)handler;

    act->sa_sigaction = handler;
    
    sigfillset(&act->sa_mask);
    
    // act->sa_flags = SA_SIGINFO;
    act->sa_flags = SA_RESTART;
    
    // sigaction(SIGCHLD, act, NULL);
    // sigaction(SIGPIPE, act, NULL);
    // sigaction(SIGTERM, act, NULL);
    sigaction(SIGINT, act, NULL);
    sigaction(SIGQUIT, act, NULL);
}



void handler(int sig, siginfo_t* siginfo, void* buf) {
    if(sig==SIGINT || sig==SIGQUIT) {
        printf("Tried to kill child %d\n", getpid());

        FILE* fptr;

        char* filName = malloc((strlen("log_file."))+12);
        strcpy(filName, "log_file.");
        char* tmpNum = malloc(12);
        sprintf(tmpNum, "%d", getpid());
        strcat(filName, tmpNum);

        free(tmpNum);

        char* dir = malloc( strlen("./logFiles/")+12+strlen("log_file.") );
        strcpy(dir, "./logFiles/");
        strcat(dir, filName);

        free(filName);

        fptr = fopen(dir, "w");

        if(fptr==NULL) {
            perror("create File.");
        }

        free(dir);

        fclose(fptr);
    }
    // else if(sig==SIGPIPE) {
    //     printf("Tried to kill pipe\n");
    // }
}

// void simplehandler(int sig) {
//     if(sig==SIGTERM) {
//         printf("Tried to kill child %d\n", getpid());

//         FILE* fptr;

//         char* filName = malloc((strlen("log_file."))+12);
//         strcpy(filName, "log_file.");
//         char* tmpNum = malloc(12);
//         sprintf(tmpNum, "%d", getpid());
//         strcat(filName, tmpNum);

//         free(tmpNum);

//         char* dir = malloc( strlen("./logFiles/")+12+strlen("log_file.") );
//         strcpy(dir, "./logFiles/");
//         strcat(dir, filName);

//         free(filName);

//         fptr = fopen(dir, "w");

//         if(fptr==NULL) {
//             perror("create File.");
//         }

//         free(dir);

//         fclose(fptr);


//         // printf("PID and countries arr:\n");
//         // for (int i=0; i < 5; i++) {
//         //     printf("For i %d -> %d\n", i, WorkersArr[i]->pid);
//         // }

//         // extern int mySignalFlag=0;

//     }
// }