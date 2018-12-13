#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define EPOLLEVENTS 100
#define LISTENQ 5
#define MAXSIZE 1024
#define FDSIZE 1000
#define EPOLLEVENTS 100

int socket_bind(const char* ip, int port)
{
  int listenfd;
  struct sockaddr_in servaddr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) 
  {
    perror("socket error:");
    exit(1);
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons(port);
  if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1)
  {
    perror("bind error: ");
    exit(1);
 
  }
  return listenfd;
}

void delete_event(int epollfd, int fd, int state)
{
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void modify_event(int epollfd, int fd, int state)
{
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void add_event(int epollfd, int fd, int state)
{
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void handle_accept(int epollfd, int listenfd)
{
  printf("accept new client");
  int clifd;
  struct sockaddr_in cliaddr;
  socklen_t cliaddrlen;
  clifd = accept(listenfd, (struct sockaddr*) &cliaddr, &cliaddrlen);
  if (clifd ==-1)
  {
    perror("accept error: ");
  } 
  else 
  {
    printf("accept a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
    printf("new client fd: %d\n", clifd);
    add_event(epollfd, clifd, EPOLLIN);
  }
}

void do_write(int epollfd, int fd, char *buf)
{
  printf("write buf: %s to fd: %d done.\n", buf, fd);
  int nwrite = write(fd, buf, strlen(buf));
  if (nwrite == -1)
  {
    perror("write failed: ");
    close(fd);
    delete_event(epollfd, fd, EPOLLOUT);
  }
  else
  {
    modify_event(epollfd, fd, EPOLLIN);
  }
  memset(buf, 0, MAXSIZE);
}

void do_read(int epollfd, int fd, char *buf)
{
  int nread = read(fd, buf, MAXSIZE);
  if (nread == -1)
  {
    perror("read failed: ");
    close(fd);
    delete_event(epollfd, fd, EPOLLIN);
  } 
  else if (nread == 0)
  {
    fprintf(stderr, "client close %d.\n", fd);
    close(fd);
    delete_event(epollfd, fd, EPOLLIN);
  }
  else 
  {
    printf("read buf: %s", buf);
    modify_event(epollfd, fd, EPOLLOUT);
  }
}

void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf)
{
  int i;
  int fd;
  for (i=0; i< num; i ++)
  {
    fd = events[i].data.fd;
    if (events[i].events & EPOLLIN)
    {
      if (fd==listenfd) 
      {
        handle_accept(epollfd, listenfd);
      }
      else 
      {
        do_read(epollfd, fd, buf);
        printf("read done from fd: %d %s\n", fd, buf);
      }
    }
    else
    {
      do_write(epollfd, fd, buf);
    }
    /*
    else if (events[i].events * EPOLLIN)
    {
      do_read(epollfd, fd, buf);
    }
    else if (events[i].events & EPOLLOUT)
    {
      do_write(epollfd, fd, buf);
    }*/
  }
}

void do_epoll(int listenfd)
{
  int epollfd;
  struct epoll_event events[EPOLLEVENTS];
  int ret;
  char buf[MAXSIZE];
  memset(buf, 0, MAXSIZE);
  epollfd = epoll_create(FDSIZE);
  add_event(epollfd, listenfd, EPOLLIN);
  for (;;)
  {
    ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
    handle_events(epollfd, events, ret, listenfd, buf);
  }
  close(epollfd);
}

int main()
{
  int listenfd = socket_bind("10.20.202.218", 6666);
  listen(listenfd, LISTENQ);
  do_epoll(listenfd);
}
