#include "apue.h"

int main()
{
  if (chmod("bar", S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH) == -1) 
  {
    err_sys("chmod err for file bar");
  }
}
