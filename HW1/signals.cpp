// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
// struct sigaction {
//                void     (*sa_handler)(int);
//                void     (*sa_sigaction)(int, siginfo_t *, void *);
//                sigset_t   sa_mask;
//                int        sa_flags;
//                void     (*sa_restorer)(void);
//            };

void handler_cntlc(int signum) {
   std::cout << "caught ctrl-C" << std::endl;
   if(fg_job){
      std::cout << "process " << fg_job->pid << " was killed" << std::endl;
      kill(SIGKILL, fg_job->pid);
      fg_job = nullptr;
   }
   return;  
}

void handler_cntlz(int signum) {
   std::cout << "caught ctrl-Z" << std::endl;
   if(fg_job){
      std::cout << "process " << fg_job->pid << " was stopped" << std::endl;
      kill(SIGSTOP, fg_job->pid);
      fg_job->is_stopped = true;
      fg_job = nullptr;
   }
   return;  
}

void handler_sigchld(int signum) {
    std::cout << "Received SIGCHLD" << std::endl;

    // Iterate through the list of jobs
   //  for (auto it = jobs.jobs_list.begin(); it != jobs.jobs_list.end(); ++it) {
    while(true){
        int status;
        pid_t result = waitpid(-1, &status, WNOHANG);
        if (result == -1) {
            // Error while waiting for the child process
            // perror("waitpid");
            return;
        } else if (result > 0) {
            // Child process terminated
            std::cout << "Child process with PID " << result << " terminated" << std::endl;
            // Optionally, perform any additional actions (e.g., cleanup)
            // Remove the terminated job from the vector
            jobs.remove_job_by_pid(result);
        } else {
            // no Child process ended
            return;
        }
    }
}