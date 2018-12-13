#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>

int main()
{ 
  char buf;
  int keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
  assert(keyboard > 0);
  fd_set readfd;
  struct timeval timeout;
  while (1)
  {
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    FD_ZERO(&readfd);
    FD_SET(keyboard, &readfd);
    int ret = select(keyboard+1, &readfd, NULL, NULL, &timeout);
    if (ret == 0)
    {
      printf("timeout\n");
      exit(0);
    }
    if (FD_ISSET(keyboard, &readfd))
    {
      int i = read(keyboard, &buf, 1);
      if ('\n' == buf)
      {
        continue;
      }
      printf("the input is %c----\n", buf);
      if (buf == 'q')
      {
        break;
      }
    }
  }
}
