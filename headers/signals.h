#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

int termination;
int sigkill;
int *pidAr;

void handler(int, siginfo_t*, void*);
void HandlerInit(struct sigaction*, void (*handler)(int, siginfo_t*, void*));

void simplehandler(int sig);