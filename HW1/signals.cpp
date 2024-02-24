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
      if(kill(fg_job->pid, SIGKILL) == -1){
         perror("smash error: kill failed");
      }
      int status;
      if(waitpid(fg_job->pid, &status, 0) == -1){
         perror("smash error: waitpid failed");         
      }
      delete fg_job;
      fg_job = nullptr;
   }
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
      if(kill(fg_job->pid, SIGSTOP) == -1){
         perror("smash error: kill failed");         
      }
      fg_job->is_stopped = true;
      fg_job->time_stamp = time(NULL);
      jobs.put_job(fg_job);
      delete fg_job;
      fg_job = nullptr;
   }
   return;
}

/*******************************************/
/* Name: handler_sigchld
   Synopsis: handle SIGCHLD e.g. by child process finished */
/******************************************/
void handler_sigchld(int signum) {
   // Iterate through the list of jobs
   while(true){
      int status;
      pid_t result = waitpid(-1, &status, WNOHANG);
      if (result == -1 && errno != ECHILD) {
         perror("smash error: waitpid failed");
         break;
      } else if (result > 0) {
         // Child process terminated
         if(fg_job != nullptr && result == fg_job->pid){
            delete fg_job;
            fg_job = nullptr;
         } else {
            // Remove the terminated job from the vector
            jobs.remove_job_by_pid(result);
         }
      } else {
         // no Child process ended
         break;
      }
   }
   return;
}
