#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int fd;
    if ((fd = open(argv[2],O_CREAT|O_WRONLY,0666)) < 0) 
        {perror(argv[2]); exit(1); } 
    if (dup2(fd,1) !=1) /*dup new fd to *standard output*/ 
    { 
        perror("Unexpected dup failure");
        exit(1); 
    }
    pid_t pid = fork();
    if (pid == 0) {
        execv(argv[1], &argv[1]);
    } else {
        int status;
        if (waitpid(pid, &status, 0) < 0){
            perror("waitpid failed:");
        }
        close(fd); /*close new fd, no longer needed*/
    }
    return 0;
}