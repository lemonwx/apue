#include "apue.h"


int main() 
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL ) 
    {
      if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = 0;;
      }
      pid = fork();
      if (pid < 0)
      {
        err_sys("fork failed");
      } 
      else if (pid == 0 ) 
      {
        execlp(buf, buf, (char *)0);
        err_ret("couldn't execute: %s", buf);
        exit(127);
      }
      else
      {
        pid = waitpid(pid, &status, 0);
        if (pid < 0) 
        {
          err_sys("waitpid error");
        }
      }
      printf("%% ");
    }
    exit(0);
}
