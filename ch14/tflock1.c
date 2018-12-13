#include "apue.h"
#include <fcntl.h>


pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
  struct flock lock;
  lock.l_type = type;
  lock.l_start = offset;
  lock.l_whence = whence;
  lock.l_len = len;

  if (fcntl(fd, F_GETLK, &lock) < 0)
    err_sys("fcntl error");
  if (lock.l_type == F_UNLCK)
  {
    return 0;
  }
  return lock.l_pid;
}

int main()
{
  int fd = open("temp.file", O_RDWR);
  struct flock lock;
  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  lock.l_pid = getpid();
  if (fcntl(fd, F_SETLK, &lock) < 0)
  {
    err_sys("lock failed");
  }
  sleep(5);

  pid_t pid = lock_test(fd, F_RDLCK, 0, SEEK_SET, 0);
  printf("file now locked by %d\n", pid);
  sleep(1000);
  return 0;
}
