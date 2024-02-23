//		commands.c
//********************************************
#include "commands.hpp"
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
	// bool illegal_cmd = false; // illegal command
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
			std::cerr << "smash error: cd: too many arguments" << std::endl; 
			return 1;
		}
		// calculate where to jump .. / - / path
		std::string path_to_jump;
		if(!strcmp(args[1], "-")){
			if(old_pwd.empty()){
				std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
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
		Job* to_fg = NULL;
		if(num_arg == 0){ // > fg
			if(jobs.jobs_list.empty()){
				std::cerr << "smash error: fg: jobs list is empty" << std::endl;
				return 1;
			} else {
				int max_job_id = jobs.get_next_job_id() - 1;
				to_fg = jobs.get_job_by_job_id(max_job_id);
			} 
		}else if(num_arg == 1){ // > fg 13
			try {
				to_fg = jobs.get_job_by_job_id(atoi(args[1]));
			}
			catch(std::runtime_error){
				std::cerr << "smash error: fg: job-id " << args[1] << " does not exist" << std::endl;
				return 1;
			}
		} else { // > fg 13 15
			std::cerr << "smash error: fg: invalid arguments" << std::endl;
			return 1;
		}
		kill(to_fg->pid, SIGCONT);
		to_fg->is_stopped = false;
		fg_job = new Job(*to_fg);
		jobs.remove_job_by_pid(to_fg->pid);
		int status;
		pid_t fg_pid = fg_job->pid;
		waitpid(fg_pid, &status, 0);
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		Job *to_bg = nullptr;
		if(num_arg == 1){ // > bg 13
			if(atoi(args[1]) <= 0){
				std::cerr << "smash error: bg: invalid arguments" << std::endl;
				return 1;
			}
			try {
				to_bg = jobs.get_job_by_job_id(atoi(args[1]));
				if(!to_bg->is_stopped){
					std::cerr << "smash error: bg: job-id " << args[1] << " is already running in the background" << std::endl;
					return 1;
				}
			}
			catch(std::runtime_error){
				std::cerr << "smash error: bg: job-id " << args[1] << " does not exist" << std::endl;
				return 1;
			}
		} else if(num_arg == 0){ // > bg
			int max_stopped_job_id = -1;
			for(auto& j : jobs.jobs_list){
				if(j.is_stopped && j.id > max_stopped_job_id) {
					max_stopped_job_id = j.id;
					}
			}
			if(max_stopped_job_id == -1){
				std::cerr << "smash error: bg: there are no stopped jobs to resume" << std::endl;
				return 1;
			}
			to_bg = jobs.get_job_by_job_id(max_stopped_job_id);
		} else { // > bg 13 13
			std::cerr << "smash error: bg: invalid arguments" << std::endl;
			return 1;
		}
		to_bg->is_stopped = false;
		std::cout << to_bg->cmd << " : " << to_bg->pid << std::endl;
		kill(to_bg->pid, SIGCONT);
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{	
		if((num_arg == 1) && !strcmp(args[1],"kill")){ // > quit kill
			while(!jobs.jobs_list.empty()){
				pid_t job_to_kill = jobs.jobs_list[0].pid;
				std::cout << "[" << jobs.jobs_list[0].id << "] " << jobs.jobs_list[0].cmd << " - Sending SIGTERM... " << std::flush;
				kill(job_to_kill, SIGTERM);
				std::this_thread::sleep_for(std::chrono::seconds(5));
				try{
					jobs.get_job_by_pid(job_to_kill);
					std::cout << "(5 sec passed) Sending SIGKILL... ";
					kill(job_to_kill, SIGKILL);
					std::this_thread::sleep_for(std::chrono::seconds(10));
					kill(getpid(), SIGCHLD); // force invoke handler 
					// waitpid(job_to_kill, NULL, WNOHANG); // just in case it is not released already
				}
				catch(std::runtime_error){
					std::cout << "job with pid " << job_to_kill << "not in list" << std::endl;
				}
				std::cout << "Done." << std::endl;
			}
		}
		exit(0);
	}


	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
   		if(num_arg != 2 || !atoi(args[2]) || atoi(args[1]) >= 0 ){
			std::cerr << "smash error: kill: invalid arguments" << std::endl;
			return 1;
		}
		Job *job = nullptr;
		try {
			job = jobs.get_job_by_job_id(atoi(args[2]));
		}
		catch(std::runtime_error){
			std::cerr << "smash error: kill: job-id " << args[2] << " does not exist" << std::endl;
			return 1;
		}
		kill(job->pid, abs(atoi(args[1])));
		std::cout << "signal number " << args[1] << " was sent to pid " << job->pid << std::endl;
	} 
	/*************************************************/	
	else if (!strcmp(cmd, "diff"))
	{
   		if(num_arg != 2){ 
			std::cerr << "smash error: diff: invalid arguments" << std::endl; return 1;
			}
		

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
		bool is_bg = false;
		if(!strcmp(args[num_arg], "&")){
			is_bg =true;
			args[num_arg] = '\0';
		}
		ExeExternal(args, cmdString, is_bg);
	}
	// if (illegal_cmd == true)
	// {
	// 	std::cerr << "smash error: " << cmd << ": " << error_msg << std::endl;
	// 	return 1;
	// }
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG-1], char* cmdString, bool is_bg)
{
	int pID;
    	switch(pID = fork())
	{
    		case -1: 
					perror("smash error: fork failed");
					break;
        	case 0 :
			{
                	// Child Process
               		setpgrp();
					execv(args[0], args);
			        exit(0);
					break;
			}
			default:
			{
					Job *new_job = new Job(jobs.get_next_job_id(), cmdString, pID);
					if (is_bg){ // bg job
						delete new_job;
						break;
					} else { // fg
						fg_job = new_job;
						int status;
						waitpid(pID, &status, 0);
					}
			}
	}
}
