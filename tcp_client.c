#include "tcp_client.h"
#include "shapetype.h"
#include <time.h>

extern void shapes_to_JSON(ShapeType *shapes, char *json_string, size_t size);
extern void JSON_to_shapes(char *json_string,size_t size,ShapeType *shapes);

void my_sighandler(int sig){ // can be called asynchronously
  flag = 1; // set flag
}

int main(int argc, char** argv) {
      printf("Hello, World! From Client\n");

/*
      //#Task 2: for shapes
      ShapeType shapetype;
      shapetype.color="blue";
      shapetype.x=1.53;
      shapetype.y=5.26;
      shapetype.shapesize=10.11;

      //For testing JSON
      shapes_to_JSON(&shapetype,send_buf,MAXLINE);

      printf("Data->JSON: %s\n",send_buf);

      ShapeType shapeConverted;

      JSON_to_shapes(send_buf,MAXLINE,&shapeConverted);

      printf("JSON->Data:\tcolor:%s,x=%lu,y=%lu,shapesize=%lu\n",shapeConverted.color,shapeConverted.x,shapeConverted.y,shapeConverted.shapesize);

 */

     int sockfd;
      struct sockaddr_in servaddr;

      
        // Register signals 
         signal(SIGINT, my_sighandler); 

      if (argc != 3)
      {
         printf("usage: tcp_client <IP Address> <port #>\n");
         exit(0);
      }
      
      char *servIP=argv[1];
      in_port_t servport=atoi(argv[2]);



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

      //Success
         printf("Connected to server (%s) on port (%d) succesfully!\n",servIP,servport);
      
      //3. Read data from stdin and write to socket
      //4. receive reply from server
     // while (0==flag)
     //{ 
        // For task 1
        // send_string(stdin,sockfd);       

        // Send a JSON shape
        // TODO : Random shape based on command line input

        ShapeType shapetype;
        shapetype.color="blue";
        shapetype.shapesize=10.11;

        srand((unsigned)time(NULL));

        while (1)
        {
            
            shapetype.x=(float)rand()/RAND_MAX*255;
            shapetype.y=(float)rand()/RAND_MAX*255;
            send_shape(&shapetype,sockfd);
            sleep(1);
        }


      //}
   
   close(sockfd);
   exit(0);
}

void send_string(FILE *fp, int sockfd){
   char sendline[MAXLINE];

   printf("Input String to send:\n");
   
   while (fgets(sendline, MAXLINE, fp) != NULL)
   {
      //write to socket
      
      writen(sockfd, sendline, strlen(sendline));

      if (0>=readline(sockfd,(void*) read_buf, sizeof (read_buf)))
                {
                    //Error or client terminated. return to main
                    printf("Error or client terminated.\n");
                  
                    close(sockfd);
                }else
                {
                    //echo to terminal. Can echo back if necessary
                    printf("Received (ECHO):%s",read_buf);
                    printf("Input String to send:\n");
                }
   }
}

void send_shape(ShapeType *shape,int sockfd){
   char sendline[MAXLINE];

  
      shapes_to_JSON(shape,send_buf,MAXLINE);

      printf("Sending\t\t:%s",send_buf);

      //write to socket
      
      writen(sockfd, send_buf, strlen(send_buf));

      if (0>=readline(sockfd,(void*) read_buf, sizeof (read_buf)))
                {
                    //Error or client terminated. return to main
                    printf("Error or client terminated.\n");
                  
                    close(sockfd);
                }else
                {
                    //echo to terminal. Can echo back if necessary
                    printf("Received (ECHO)\t\t:%s\n",read_buf);
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

ssize_t my_read(int fd, char *ptr){

    if (read_cnt <= 0){
        //No data read from socket yet, read in MAX_LINE blocks
        again:
        if ((read_cnt = read(fd,read_buf,sizeof(read_buf))) < 0){
            if (EINTR == errno){
                //Interrupted by SIGNAL, continue
                    goto again;
            return (-1); //Real error
            }
        }else if (0 == read_cnt){
            return (0);
        }

        //Block read successful, point to head of buffer
        readptr = read_buf;
    }

    //decrement readcnt by 1, and copy to calling destination memory
    //successive calls will not read from socket by return values from internal buffer
    //once m_read_cnt is 0, will read from the socket again from the loop above
    read_cnt--; 
    *ptr = *readptr++;
    return (1); 
}

ssize_t readline(int fd, void *vptr,size_t maxlen){
    ssize_t n,rc;
    char    c,*ptr;

    ptr=(char*)vptr;

    for (n=1; n<maxlen ; n++){
        if ((rc = my_read(fd, &c) == 1)){
            //Successfully read 1 char
            *ptr++ = c;
            if (c == '\n')
                break; //Newline is hit
        }else if (0 == rc){
            return (0); //Client is closed
        }else
            return (-1);    //Erro= cJSON_Print(monitor);r
    }

    *ptr = 0; //Append null at the end
    return (n);
}