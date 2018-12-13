#include "apue.h"

void sig_int(int signal)
{
  printf("interrupt %d \n%%", signal);
}

int main() 
{
  int sigs[2] = {SIGINT, SIGTERM};
  for (int i = 0; i < 2; i += 1) 
  {
    if (signal(sigs[i], sig_int) == SIG_ERR)
    {
      err_sys("my , signal error");
    }
  }
  sleep(1000);
}
