#include "common.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
  int fd = bind_listen("10.20.202.218", 6666);
  printf("listen at %d\n", fd);

  char buf[100];
  read(STDIN_FILENO, buf, 100);
  printf("read done.\n");
  close(fd);
  read(STDIN_FILENO, buf, 100);
}
