#include "apue.h"
#include <pthread.h>

void *thd1(void *arg)
{
  printf("thd1 exit\n");
  pthread_exit((void *)1);
}

void *thd2(void *arg)
{
  printf("thd2 exit\n");
  return ((void *)2);
}


int main()
{
  int err;
  pthread_t tid1, tid2;
  void *tret;
  err = pthread_create(&tid1, NULL, thd1, NULL);
  if (err != 0)
  {
    perror("create thread failed");
  }
  err = pthread_create(&tid2, NULL, thd2, NULL);
  if (err != 0)
  {
    perror("create thread failed");
  }

  pthread_join(tid1, &tret);
  printf("thread 1 exit with code: %ld\n", (long)tret);
  pthread_join(tid2, &tret);
  printf("thread 2 exit with code: %ld\n", (long)tret);
  return 0;
}
