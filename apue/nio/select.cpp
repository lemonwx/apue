#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define LISTENQ 100

int socket_bind(const char *ip, int port)  
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

int do_select(int serverfd)
{
  int max_handle_fd_count = 100;
  fd_set client_fdset;
  int maxsock;
  struct timeval tv;
  int client_sockfd[100];
  bzero((void *)client_sockfd, sizeof(client_sockfd));
  int conn_amount = 0;
  maxsock = serverfd;
  char buffer[1024];
  int ret = 0;
  int loop_count = 0;
  for(;;)
  {
    printf("loop count: %d\n", loop_count ++);
    FD_ZERO(&client_fdset);
    FD_SET(serverfd, &client_fdset);
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    for (int i=0;i<max_handle_fd_count;i++)
    {
      if (client_sockfd[i] != 0)
      {
        printf("add %d:%d to select fdset", i, client_sockfd[i]);
        FD_SET(client_sockfd[i], &client_fdset);
      }
    }
    ret = select(maxsock+1, &client_fdset, NULL, NULL, &tv);
    if (ret < 0)
    {
      perror("select error");
      break;
    }
    else if (ret == 0)
    {
      printf("timeout !\n");
    }
    printf("there are %d client running\n", conn_amount);
    for (int i=0;i< max_handle_fd_count;i++)
    {
      if (client_sockfd[i] == 0) 
      {
        continue;
      }
      if(FD_ISSET(client_sockfd[i], &client_fdset))
      {
        printf("start recv from client[%d]:\n", i);
        ret = recv(client_sockfd[i], buffer, 1024, 0);
        if (ret <= 0)
        {
          printf("client[%d] close\n", i);
          close(client_sockfd[i]);
          FD_CLR(client_sockfd[i], &client_fdset);
          client_sockfd[i] = 0;
        }
        else 
        {
          printf("recv from client[%d]: %s\n", i, buffer);
        }
      }
    }
    if (FD_ISSET(serverfd, &client_fdset))
    {
      struct sockaddr_in  client_addr;
      size_t size = sizeof(struct sockaddr_in);
      int clifd = accept(serverfd, (struct sockaddr*) (&client_addr), (unsigned int*)(&size));
      if (clifd < 0) 
      {
        perror("accept error");
        continue;
      }
      for (int i = 0; i < max_handle_fd_count;i ++)
      {
        if(client_sockfd[i] == 0)
        {
          client_sockfd[i] = clifd;
          bzero(buffer, 1024);
          strcpy(buffer, "this is server! welcome!\n");
          send(clifd, buffer, 1024, 0);
          printf("new conn client[%d] %s:%d\n", i, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
          bzero(buffer, sizeof(buffer));
          ret = recv(clifd, buffer, 1024, 0);
          if (ret < 0)
          {
           perror("recv error!\n");
           close(serverfd);
           return -1;
          }
          printf("recv from fd: %d: %s, max: %d\n", clifd, buffer, maxsock);
          if (clifd > maxsock)
          {
           maxsock = clifd;
          }
          break;
        } 
      }
    } 
  } 
  for (int i=0;i<max_handle_fd_count;i++)
  {
    if (client_sockfd[i] != 0)
    {
      close(client_sockfd[i]);
    }
  }
  close(serverfd);
  return 0;
}

int main()
{
  int listenfd = socket_bind("10.20.202.218", 6666);
  listen(listenfd, LISTENQ);
  do_select(listenfd);
}
