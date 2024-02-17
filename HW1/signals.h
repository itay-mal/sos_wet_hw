#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "job.h"
extern Job* fg_job;
extern Jobs jobs;
void handler_cntlc(int signum);
void handler_cntlz(int signum);
void handler_sigchld(int signum);
#endif

