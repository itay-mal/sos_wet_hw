//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(Jobs_t jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		std::cout << getcwd() << std::endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{	
		std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
			return a.id < b.id;
		});

		time_t time_now = time();
 		for(const auto& j: jobs){
			std::cout << "[" << j.id << "] " << j.cmd << " : " << j.pid << " " << j.get_elapsed_time(time_now) << " secs" << (j.is_stopped) ? " (stopped)": "" << std::endl;
		}
	}



	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		std::cout << "smash pid is " << getpid() << std::endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
   		
	} 
	/*************************************************/	
	else if (!strcmp(cmd, "diff"))
	{
   		if(num_arg != 2){ std::cout << "smash error: diff: invalid arguments" << std::endl; return 1}
		

		std::ifstream f1(file1), f2(file2);
    	std::string line1, line2;

		while (std::getline(f1, line1) && std::getline(f2, line2)) {
			if (line1 != line2) {
				std::cout << "1" << std::endl;
				return 0;
			}
		}

		if (std::getline(f1, line1) || std::getline(f2, line2)) {
			std::cout << "1" << std::endl;
			return 0;
		}

    	std::cout << "0" << std::endl;
	}
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, Jobs_t jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

