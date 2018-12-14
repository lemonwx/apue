#include "apue.h"
#include <pthread.h>

struct foo {
  int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
  printf("%s", s);
  printf(" struct at 0x%lx\n", (unsigned long)fp);
  printf(" foo.a = %d\n", fp+1);
 /* printf(" foo.b = %d\n", fp->b);
  printf(" foo.c = %d\n", fp->c);
  printf(" foo.d = %d\n", fp->d);
*/
}

void *thd_fn(void *arg)
{
  struct foo foo  = {11121,2,3,4};
  printfoo("child thread:\n", &foo);
  pthread_exit((void *)&foo);
}


int main()
{
  pthread_t ntid;
  int err;
  struct foo *fp;

  err = pthread_create(&ntid, NULL, thd_fn, NULL);
  if (err != 0)
  {
    perror("create thread failed");
  }
  err = pthread_join(ntid, (void *)&fp);
  printfoo("parent:\n", fp);
  return 0;
}
