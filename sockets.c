#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "packages.h"
#include "parser.h"
#include "comm.h"

#define MAXLINE 2048 /*max text line length*/
#define SERV_PORT 4004 /*port*/
#define LISTENQ 24 /*maximum number of client connections*/

int sockfd;
struct sockaddr_in cliaddr, servaddr;
socklen_t clilen;
int listennn;

int clientSendPackage(int con, Package* pack){
  void* data = calloc(pack->size,1);
  memcpy(data,pack, pack->size);
  
 
  if (send(con, data,sizeof(Package),0)<0){
    printf("error %s",strerror(errno));
    return -1;
  }
  
  

  free(data);
  return 0;
}

void clientReceivePackage(int con,Package* pack){
  void * data = calloc(sizeof(Package),1); //change for maxsize constant
  int buffer_size=0;
  if ((buffer_size = recv(con, data, sizeof(Package),0))<0){
    printf("no se recibio nada\n");
    return;
  }
  printf("Read from socket\n");
  
  memcpy(pack,data,buffer_size);
  free(data);

  return;
}

int serverReceivePackage(int listenfd, Package* pack){
  //Package data;
  int aux=0, n=0;
  aux = accept(listenfd, (struct sockaddr *) &cliaddr,&clilen);


  return aux;
}

void handleRequest(int address, Package * pack){
  close(listennn);
  Package data;
  int n= 0;
  // char * resp = "CAMPEON";
 
  //respond(&data);

  while ( (n = recv(address, pack, sizeof(Package),0)) > 0)  {
    printf("*-----------%d--------*",pack->data.subC);
     printf("ACA ESTA EL PASS %s\n", pack->pass );
  //  memcpy(pack->data.response, resp, strlen(resp));
    //memset(&data,0,sizeof(Package));
    memset(pack->data.response ,0, 4096);
    
    answer(pack);
    printf("RESPUESTA ES \n --%s-- \n", pack->data.response);
  
    //printf("%s\n",data.data.response);

    //respond(&data);
    
   
    send(address, pack, sizeof(Package), 0);

  }
  ;
  if (n < 0)
  // printf("%s\n", "Read error");
  exit(0);
}

int iConnect_client(){
  struct sockaddr_in servaddr;
  char sendline[MAXLINE], recvline[MAXLINE];
  int confd;

 //basic check of the arguments
 //additional checks can be inserted
 /*if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server");
  exit(1);
 }*/

 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

 //Connection of the client to the socket
 if ((confd =connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)))<0) {
  perror("Problem in connecting to the server:s.");
  exit(3);
 }else{
  printf("Connected to server\n");
 }
 return sockfd;
}

int iConnect_server(){
 int listenfd, connfd, n;
 pid_t childpid;
 char buf[MAXLINE];


 //Create a socket for the socletf
 //If sockfd<0 there was an error in the creation of the socket
 if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }


 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(SERV_PORT);

 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

 listen (listenfd, LISTENQ);

 printf("%s\n","Server running...waiting for connections.");

 listennn = listenfd;
 return listenfd;
}
