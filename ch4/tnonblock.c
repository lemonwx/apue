#include "apue.h"
#include <fcntl.h>

int main()
{
fcntl(0, F_SETFL, O_NONBLOCK);



  char buf[10];
  while(1)
  {
    int n = read(STDIN_FILENO, buf, 10);
    if (n > 0)
    {
      printf("%d--%s", n, buf);
    }
    else 
    {
      printf("read failed: %d", n); 
      break;
    }
  }
    return 0;
}
