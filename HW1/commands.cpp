//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	static std::string old_pwd;
	char delimiters[] = {' ', '\t', '\n'};
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
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
		if(num_arg != 1){
			std::cout << "smash error: diff: invalid arguments" << std::endl; 
			return 1;
		}
		// calculate where to jump .. / - / path
		std::string path_to_jump;
		if(!strcmp(args[1], "-")){
			if(old_pwd.empty()){
				std::cout << "smash error: cd: OLDPWD not set" << std::endl;
				return 1;
			}
			path_to_jump = old_pwd;
		}
		else if(!strcmp(args[1], "..")){
			std::string cwd(getcwd(NULL, 0));
			std::size_t found = cwd.find_last_of("/");
  			path_to_jump = cwd.substr(0,found);
			if(path_to_jump.empty()){
				path_to_jump = "/";
			}
		}
		else{
			path_to_jump = args[1];
		}		
		//save pwd as old_pwd
		old_pwd = getcwd(NULL, 0);
		// jump
		chdir(path_to_jump.c_str());
		return 0;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		std::cout << getcwd(NULL, 0) << std::endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{	
		jobs.print_jobs();
	}

	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		std::cout << "smash pid is " << getpid() << std::endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		// find job in jobs where job.id == arg[1]
		Job* to_fg = NULL;
		if(num_arg == 0 && jobs.jobs_list.empty()){
			std::cout << "jobs list is empty" << std::endl;
		}
		if(num_arg == 1){
			try {
				Job* to_fg = jobs.get_job_by_job_id(atoi(args[1]));
			}
			catch(){
				
			}
		}
		if(to_fg == NULL){
			int max_job_id = jobs.get_next_job_id() - 1;
			to_fg = jobs.get_job_by_job_id(max_job_id);
			kill(SIGCONT, to_fg->pid);
			jobs.remove_job_by_pid(to_fg->pid);
		}
		fg_job = to_fg;
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
   		if(num_arg != 2){ std::cout << "smash error: diff: invalid arguments" << std::endl; return 1;}
		

		std::ifstream f1(args[1]), f2(args[2]);
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
	if (illegal_cmd == true)
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
void ExeExternal(char *args[MAX_ARG-1], char* cmdString)
{
	int pID;
    	switch(pID = fork())
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
					break;
        	case 0 :
			{
                	// Child Process
               		setpgrp();
					execv(args[0], args);
			        exit(0);
			}
			default:
			{
					Job *new_job = new Job(jobs.get_next_job_id(), cmdString, pID);
					jobs.jobs_list.push_back(*new_job);
                	break;
					// Add your code here
					/* 
					your code
					*/
			}
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize)
{

	// char* Command;
	// char delimiters[] = {' ', '\t' ,'\n'};
	// char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		std::cout << "im BG" << std::endl;
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
		// int max_id = 0;
		// for(auto j) 
		// Job j()
		/* 
		your code
		*/
		return 0;
		
	}
	return -1;
}


