#include <unistd.h>
#include <poll.h>
#include <stdio.h>

#define TIMEOUT 5

int main()
{
  struct  pollfd fds[2];
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = STDOUT_FILENO;
  fds[1].events = POLLOUT;
  while(1)
  {
  int ret = poll(fds, 2, -1);
  if (ret == -1)
  {
    perror("poll failed");
    return 1;
  }
  if (ret == 0)
  {
    printf("no fd ready, exit\n");
    return 1;
  }
  printf("there are %d fds ready\n", ret);
  printf("stdin %d\n", fds[0].revents & POLLIN);
  printf("stdout %d\n", fds[1].revents & POLLOUT);

  char buf[1024];
  int readlens;
  if (fds[0].revents & POLLIN)
  {
    readlens = read(fds[0].fd, buf, 1024);
    if (readlens == -1)
    {
      perror("read from stdin failed:");
    }
  }
  if (fds[1].revents & POLLOUT)
  {
   int writelens =  write(fds[1].fd, buf, readlens);
   if (writelens == -1)
   {
     perror("write to stdout failed:");
   }
   if (writelens != 0)
   {
      printf("dump from stdin to stdout finish\n");
      break;
   }
  }
  }
  return 0;
}
