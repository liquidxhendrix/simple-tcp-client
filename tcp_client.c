#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h> 
#define SA struct sockaddr
#define DEFAULT_SERV_ADDR "192.168.1.107"
#define DEFAULT_SERV_PORT 1111
#define MAXLINE 1024

ssize_t writen(int fd, const void *vptr, size_t n);
void send_string(FILE *fp, int sockfd);
volatile sig_atomic_t flag = 0;

void my_sighandler(int sig){ // can be called asynchronously
  flag = 1; // set flag
}

int main(int argc, char** argv) {
      printf("Hello, World! From Client\n");

      int sockfd;
      struct sockaddr_in servaddr;

      char servIP[]=DEFAULT_SERV_ADDR;
      in_port_t servport=DEFAULT_SERV_PORT;

        // Register signals 
         signal(SIGINT, my_sighandler); 

      /*if (argc != 3)
      {
         printf("usage: tcp_client <IP Address> <port #>\n");
         exit(0);
      }
      */


      //1. Create master socket
      sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

      if (sockfd==-1)
      {
         printf("Create socket failed. ErrNo:%d\n",errno);
         exit(-1);
      }

      bzero(&servaddr,sizeof(servaddr));
      servaddr.sin_family=AF_INET;
      inet_pton(AF_INET,servIP,&servaddr.sin_addr);
      servaddr.sin_port=htons(servport);

      //2. Connect to server

      if (connect(sockfd,(SA *)&servaddr,sizeof(servaddr))<0)
      {
         //error in connect
         printf("Connect to server (%s) on port (%d) failed. ErrNo:%d\n",servIP,servport,errno);
         exit(0);
      }

      //3. Read data from stdin and write to socket
      while (0==flag)
         send_string(stdin,sockfd);
      
   
   close(sockfd);
   exit(0);
}

void send_string(FILE *fp, int sockfd){
   char sendline[MAXLINE];

   while (fgets(sendline, MAXLINE, fp) != NULL)
   {
      //write to socket
      writen(sockfd, sendline, strlen(sendline));
   }
}

ssize_t writen(int fd, const void *vptr, size_t n){
   //Write the whole string in a loop
   
   size_t nleft;
   ssize_t nwritten;
   const char *ptr;

   ptr = vptr;
   nleft = n;
   while (nleft > 0){
      if (  (nwritten = write(fd,ptr,nleft)) <=0){
         if (nwritten < 0 && errno == EINTR) //interrupted
            nwritten = 0; 
         else
            return (-1); //error happened
      }

      nleft -= nwritten;
      ptr += nwritten;
   }

   return(n);
}
