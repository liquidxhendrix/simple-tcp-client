#ifndef TCP_CLIENT
#define TCP_CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h> 
#include "shapetype.h"

#define SA struct sockaddr
#define DEFAULT_SERV_ADDR "192.168.1.112"
#define DEFAULT_SERV_PORT 1111
#define MAXLINE 1024

ssize_t readline(int fd, void *vptr,size_t maxlen);
ssize_t my_read(int fd, char *ptr);
ssize_t writen(int fd, const void *vptr, size_t n);
void send_string(FILE *fp, int sockfd);
void send_shape(ShapeType *shape,int sockfd);
int read_cnt;
volatile sig_atomic_t flag = 0;
char read_buf[MAXLINE];
char send_buf[MAXLINE];
char *readptr;

#endif