#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
  if (access("apue.h", X_OK) == -1)
  {
    err_ret("access error for %s", argv[1]);
  } 
  else 
  {
    printf("read access ok");
  }
}
