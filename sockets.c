#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "packages.h"
#include "comm.h"


int clientSendPackage(int con, Package* pack){
  void* data = calloc(pack->size,1);
  memcpy(data,pack, pack->size);
  if (send(con, data,pack->size,0)<0)
    return -1;
  free(data);
  return 0;
}

void clientReceivePackage(int con,Package* pack){
  void * data = calloc(4096,1); //change for maxsize constant
  int buffer_size=0;
  if ((buffer_size = recv(con, data, 4096,0))<0){
    printf("no se recibio nada\n");
    return;
  }
  printf("ANTES DE COPIAR Y DESPUES DE RECIBIR.\n");
  memcpy(pack,data,buffer_size);
  printf("DESPUES DE COPIAR.\n");
  printf("%s\n", pack->data.response);
  free(data);

  return;
}

void handleRequest(int connfd){
  
  Package data;
  int n= 0;
  while ( (n = recv(connfd, &data, sizeof(Package),0)) > 0)  {
    respond(&data);
    printf("%s\n",data.data.response);

    respond(&data);
    /*printf("%s","String received from and resent to the client:");
    puts(buf);*/
    send(connfd, data.data.response, n, 0);
  
  }
  
  if (n < 0)
   printf("%s\n", "Read error");
  exit(0);
}

int iConnect_client(){
  struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];

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
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server:s.");
  exit(3);
 }
}
