#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "apue.h"
int foo()
{
  return 0;
}


int main(int argc, char *argv[])
{
  foo();
  struct stat buf;
  char *ptr;xw
  for (int i = 1; i < argc; i ++)
  {
    printf("%s: ", argv[i])
    if (stat(argv[i], &buf) < 0)
    {
      printf("stat error");
      continue;
    }
    if (S_ISREG(buf.st_mode))
    {
      ptr = "regular";
    }
    else if (S_ISDIR(buf.st_mode))
    {
        ptr = "dir";
    }
    else 
    {
      ptr = "unknown mode";
    }
    printf("%s\n", ptr);
  }
}
