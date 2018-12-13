#include <fcntl.h>
#include "apue.h"

void set_fl(int fd, int flags);

int main(int argc, char* argv[])
{
  int val;
  if (argc != 2) 
  {
    err_quit("useage: main <fd>");
  }
  set_fl(atoi(argv[1]), O_SYNC);
  if ((val=fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
  {
    err_sys("fcntl error for fd: %d", atoi(argv[1]));
  }
  int switch_val = val & O_ACCMODE;
  printf("%d\n", switch_val);
  switch (switch_val)
  {
  case O_RDONLY:
    printf("read only");
    break;
  case O_WRONLY:
    printf("write only");
    break;
  case O_RDWR:
    printf("rd wr");
    break;
  default:
    err_dump("unknow access mode");
  }

  if (val & O_APPEND)
  {
    printf(", append");
  }
  if (val & O_NONBLOCK)
  {
    printf(", nonblocking");
  }
  if (val & O_SYNC)
  {
    printf(", synchronous writes");
  }
  putchar('\n');
  exit(0);
}

// 修改文件描述符和状态

void set_fl(int fd, int flags)
{
  int val;
  if ((val = fcntl(fd, F_GETFL, 0)) < 0)
  {
    err_sys("fcntl F_GETFL error");
  }
  val |= flags;
  if (fcntl(fd, F_SETFL, val) < 0)
  {
    err_sys("fcntl F_SETFL error");
  }
}

