// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

#include "signals.hpp"
#include <time.h>


/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
/******************************************/
void handler_cntlc(int signum) {
   std::cout << "caught ctrl-C" << std::endl;
   if(fg_job){
      std::cout << "process " << fg_job->pid << " was killed" << std::endl;
      kill(fg_job->pid, SIGKILL);
      int status;
      waitpid(fg_job->pid, &status, 0);
      delete fg_job;
      fg_job = nullptr;
   }
   std::cout << "exiting ctrl-C handler" << std::endl;
   return;
}


/*******************************************/
/* Name: handler_cntlz
   Synopsis: handle the Control-Z */
/******************************************/
void handler_cntlz(int signum) {
   std::cout << "caught ctrl-Z" << std::endl;
   if(fg_job){
      std::cout << "process " << fg_job->pid << " was stopped" << std::endl;
      kill(fg_job->pid, SIGSTOP);
      fg_job->is_stopped = true;
      fg_job->time_stamp = time(NULL);
      jobs.put_job(fg_job);
      delete fg_job;
      fg_job = nullptr;
   }
   std::cout << "exiting ctrl-z handler" << std::endl;
   return;
}

/*******************************************/
/* Name: handler_sigchld
   Synopsis: handle SIGCHLD e.g. by child process finished */
/******************************************/
void handler_sigchld(int signum) {
   //  std::cout << "Received SIGCHLD" << std::endl;

    // Iterate through the list of jobs
    while(true){
        int status;
        pid_t result = waitpid(-1, &status, WNOHANG);
        if (result == -1) {
            // Error while waiting for the child process
            // perror("waitpid");
            return;
        } else if (result > 0) {
            // Child process terminated
            // std::cout << "Child process with PID " << result << " terminated" << std::endl;
            // Optionally, perform any additional actions (e.g., cleanup)
            // Remove the terminated job from the vector
            if(fg_job != nullptr && result == fg_job->pid){
               delete fg_job;
               fg_job = nullptr;
            } else {
               jobs.remove_job_by_pid(result);
            }
        } else {
            // no Child process ended
            return;
        }
    }
}
