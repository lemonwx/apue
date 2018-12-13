#ifndef COMMON_H__
#define COMMON_H__
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
int bind_listen(const char *ip, int port)
{
  int listenfd;
  struct sockaddr_in my_addr;
  unsigned int sin_size;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) ==-1)
  {
    perror("socket");
    return -1;
  }
  printf("socket ok");
  bzero(&my_addr, sizeof(my_addr));


  my_addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &my_addr.sin_addr);
  my_addr.sin_port = htons(port);
  if (bind(listenfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)
  {
    perror("bind error: ");
    exit(1);
  }
  listen(listenfd, 100);
  return listenfd;
}
#endif
