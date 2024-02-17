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
#include "job.h"
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

// char* L_Fg_Cmd;
Jobs jobs;//This represents the list of jobs. Please change to a preferred type (e.g array of char*)
Job *fg_job = NULL;
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/
	struct sigaction ctrlc_act;
	ctrlc_act.sa_handler = &handler_cntlc;
	sigfillset(&ctrlc_act.sa_mask);
	sigaction(SIGINT, &ctrlc_act, NULL);

	struct sigaction ctrlz_act;
	ctrlz_act.sa_handler = &handler_cntlz;
	sigfillset(&ctrlz_act.sa_mask);
	sigaction(SIGTSTP, &ctrlz_act, NULL);

	struct sigaction sigchld_act;
	sigchld_act.sa_handler = &handler_sigchld;
	sigfillset(&sigchld_act.sa_mask);
	sigaction(SIGCHLD, &sigchld_act, NULL);

	/************************************/
	// Init globals 


	
	// L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	// if (L_Fg_Cmd == NULL) 
	// 		exit (-1); 
	// L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
			while(fg_job){}; // busy wait for fg job to be over
			printf("smash > ");
			fgets(lineSize, MAX_LINE_SIZE, stdin);
			strcpy(cmdString, lineSize);
			cmdString[strlen(lineSize)-1]='\0'; // replaces \n with \0
						// background command
	 		if(!BgCmd(lineSize)) continue;
						// built in commands
			ExeCmd(lineSize, cmdString);

			/* initialize for next line read*/
			lineSize[0]='\0';
			cmdString[0]='\0';
	}
    return 0;
}
