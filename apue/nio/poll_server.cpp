#include "common.h"
#include <unistd.h>
#include <poll.h>
#include <errno.h>


#define OPEN_MAX 1024
#define MAXLINE 1024
#define INFTIM -1
void do_poll(int listenfd)
{
  char buf[MAXLINE];
  int connfd, sockfd;
  struct sockaddr_in cliaddr;
  socklen_t cliaddrlen;
  struct pollfd clientfds[OPEN_MAX];
  int maxi;
  int i;
  int nready;
  clientfds[0].fd = listenfd;
  clientfds[0].events = POLLIN;

  for (i=1;i<OPEN_MAX;i++)
  {
    clientfds[i].fd = -1;
  }
  maxi = 0;
  int loop_count = 0;
  while(1)
  {
    printf("now loop count: %d, max idx: %d\n", loop_count++, maxi);
    nready = poll(clientfds, maxi+1, INFTIM);
    if (nready == -1)
    {
      perror("poll error");
      exit(1);
    }
    if (clientfds[0].revents & POLLIN)
    {
      cliaddrlen = sizeof(cliaddr);
      if ((connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &cliaddrlen)) == -1)
      {
        if (errno == EINTR)
        {
          continue;
        }
        else 
        {
          perror("accept error:");
          exit(1);
        }
      }
      fprintf(stdout, "accept a new client: %s:%d at fd: %d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port, connfd);
      for (i = 1; i < OPEN_MAX; i ++)
      {
        if (clientfds[i].fd < 0)
        {
          clientfds[i].fd = connfd;
          clientfds[i].events = POLLIN;
          maxi = i;
          break;
        }
      }
    }
    // end handle new connected client 
    for (i=1;i<OPEN_MAX;i++)
    {
      if (clientfds[i].fd < 0)
      {
        continue;
      }
      if (clientfds[i].revents & POLLIN)
      {
        printf("fd: %d recv event", clientfds[i]);
        int readlen = read(clientfds[i].fd, buf, MAXLINE);
        if (readlen==0)
        {
          close(clientfds[i].fd);
          clientfds[i].fd = -1;
          maxi -= 1;
          continue;
        }
        write(STDOUT_FILENO, buf, readlen);
        write(clientfds[i].fd, buf, readlen);
      }
    }
  }

  for(i=0;i<OPEN_MAX;i++)
  {
    if(clientfds[i].fd < 0 )
    {
      continue;
    }
    close(clientfds[i].fd);
  }
}

int main()
{
  int listenfd = bind_listen("10.20.202.218", 6666);
  printf("listen fd: %d\n", listenfd);
  do_poll(listenfd);
  close(listenfd);
}
