#include "apue.h"

int main()
{
  char *ptr = "this is apue";
  ptr += 1;;
  printf("%c\n", *ptr);
  printf("%c\n", *ptr);
  printf("uid = %d, gid = %d", getuid(), getgid());
  exit(0);
}
