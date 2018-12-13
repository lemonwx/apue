#include "apue.h"
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int foo() 
{
  int fd;
  if ((fd = creat("file.nohole", FILE_MODE)) < 0 )
  {
    err_sys("create error");
  }
    int idx = 0;
    for (idx = 0; idx < 16394; idx += 1) 
    { 
      char buf[] = "a";
      while(write(fd, buf, 1) != 1)
      {
        err_sys("write error");
      }
    }
  return 0;
}

int main(void)
{
  int fd;
  if ((fd = creat("file.hole", FILE_MODE)) < 0 )
  {
    err_sys("create error");
  }
  if (write(fd, buf1, 10) != 10) 
  {
    err_sys("buf1 write error");
  }
  if (lseek(fd, 16384, SEEK_SET) == -1)
  {
    err_sys("lseek error");
  }
  // offset now 16384
  if (write(fd, buf2, 10) != 10) 
  {
    err_sys("buf2 write error");
  }
  // offset now 16394
  foo();
  exit(0);
}
