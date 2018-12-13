#include <stdio.h>
#include <pthread.h>
#include "apue.h"

int main111()
{
#ifdef _POSIX_THREADS
  printf("support thread\n");
#else
  printf("unsupported thread\n");
#endif
  return 0;
}


pthread_t ntid;

void printids(const char *s)
{
  pid_t pid;
  pthread_t tid;
  
  pid = getpid();
  tid = pthread_self();
  printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
  printids("new thread: ");
  return ((void *)0);
}

int main()
{

  int err;
  err = pthread_create(&ntid, NULL, thr_fn, NULL);
  if (err != 0)
  {
    err_exit(err, "can't create thread");
  }
  printids("main thread:");
  sleep(1);
  return 0;
}
