#include "apue.h"

int main()
{
  int newfd = dup(STDIN_FILENO);
  printf("%d\n", newfd);
  exit(0);
}
