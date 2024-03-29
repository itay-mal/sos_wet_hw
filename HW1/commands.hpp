#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <algorithm>
#include "job.hpp"
#include <thread>
#include <chrono>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20

extern Jobs jobs;
extern Job *fg_job;
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, bool is_bg);
#endif
