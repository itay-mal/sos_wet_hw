/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "job.hpp"
#include "commands.hpp"
#include "signals.hpp"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

// char* L_Fg_Cmd;
Jobs jobs;//This represents the list of jobs. Please change to a preferred type (e.g array of char*)
Job *fg_job = nullptr;
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************


int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];

	struct sigaction ctrlc_act;
	ctrlc_act.sa_handler = &handler_cntlc;
	sigfillset(&(ctrlc_act.sa_mask));
	sigaction(SIGINT, &ctrlc_act, NULL);

	struct sigaction ctrlz_act;
	ctrlz_act.sa_handler = &handler_cntlz;
	sigfillset(&ctrlz_act.sa_mask);
	sigaction(SIGTSTP, &ctrlz_act, NULL);

	struct sigaction sigchld_act;
	sigchld_act.sa_handler = &handler_sigchld;
	sigfillset(&sigchld_act.sa_mask);
	sigaction(SIGCHLD, &sigchld_act, NULL);

	while (1)
	{
		printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0'; // replaces \n with \0
		ExeCmd(lineSize, cmdString);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
	return 0;
}
