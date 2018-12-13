#include "apue.h"
#include <errno.h>
int main(int argc, char *argv[])
{
  fprintf(stderr, "this is EACCES: %s\n", strerror(EACCES));
  /*
  
  errno = ENOENT;
  char *buf = "this is log";
  perror(buf);
  printf("this is log");
  */ 
  exit(0);
}
