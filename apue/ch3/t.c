#include <fcntl.h>
#include <stdio.h>
#include "apue.h"

int main()
{
  int fd = open("apue.h", O_RDWR);
  printf("open fd: %d\n", fd);
  if (close(fd) == -1) 
  {
    printf("close %d failed", fd);
  }
  printf("finish");
  off_t cur_off_set = lseek(STDIN_FILENO, 100, SEEK_CUR);
  printf("seek offset: %lld\n", cur_off_set);
  return 0x00;
}
