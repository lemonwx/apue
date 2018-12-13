#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#define TIMEOUT 5
#define BUF_LEN 1024

void preread()
{
  char buf[BUF_LEN];
  int ret = read(STDIN_FILENO, buf, BUF_LEN);
  printf("pre read buf: %s\n", buf);
}

int main()
{
  struct timeval tv;
  fd_set readfds;
  int ret;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);
  tv.tv_sec = TIMEOUT;
  tv.tv_usec = 0;
  ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
  if (ret == -1)
  {
   perror("select failed: ");
   return 1;
  }
  else if (ret == 0)
  {
    printf("select timeout, retry\n");
    return 0;
  }
  printf("now %d fds ready\n", ret);
  preread();
  if (FD_ISSET(STDIN_FILENO, &readfds))
  {
    printf("stdin ready for read\n");
    char buf[BUF_LEN];
    int readlens = read(STDIN_FILENO, buf, BUF_LEN);
    if (readlens == -1)
    {
      perror("read from stdin failed");
      return 1;
    }
    if (readlens > 0)
    {
      write(STDERR_FILENO, buf, readlens);
    }
    return 0;
  }
}
