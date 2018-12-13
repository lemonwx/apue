#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main()
{
  umask(0000);
  if (creat("foo", RWRWRW) < 0)
  { 
     err_sys("create rw file bar failed");
  }

  umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
  if (creat("bar", RWRWRW) < 0)
  {
    err_sys("create file bar failed");
  }
}
