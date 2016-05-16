#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include "packages.h"
#include "serialize.h"



#define SERV_PORT 3686 /*port*/
#define LISTENQ 24 /*maximum number of client connections*/

struct sockaddr_in cliaddr, servaddr;
socklen_t clilen;
int connfd, n;
int socketfd;
int conn;
struct hostent *server;

int initSockets(){
	printf("Init Sockets\n");
	if ((socketfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  		perror("Problem in creating the socket");
 	 	exit(2);
 	}
 	return socketfd;
 }

 void prepareToListen(int conn){
 	printf("Preparing to Listen\n");
 	bzero((char *) &servaddr, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	bind (conn, (struct sockaddr *) &servaddr, sizeof(servaddr));
 	listen (conn, LISTENQ);
	clilen = sizeof(cliaddr);
}

	
int  acceptConnection(int sock){
	int conn;
	printf("Waiting for connection\n");
	conn = accept(sock, (struct sockaddr *)&cliaddr, &clilen);
	return conn;

}

void prepareToConnect(char * address){

	server = gethostbyname(address);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
  	
   bzero((char *) &servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(address);
   servaddr.sin_port = htons(SERV_PORT);
}


void connectToServ(int sock){
	 if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  		perror("Problem in connecting to the server:s.");
  		exit(3);
 	}
}

void closeAcceptFD(int sock){
	close(sock);
}

int sendPackage(int con, Package* pack){

  char* data = calloc(MAX_B,1);
  serialize(pack,data);
 // printf("%d\n %d\n",pack->size,sizeof(data) );
 // memcpy(data,pack, pack->size);
 // printf("Este es el pass %d \n Este es el id %s\n",pack->studentID, pack->pass);//pack.data.pass

  if ((write(con, data, pack->size))<0){
    return -1;
  }

  free(data);
  return 0;
}

Package * receivePackage(int con,Package* pack){
	char* data = calloc(MAX_B,1);
  	int buffer_size=0;
  	if((buffer_size = read(con,data,MAX_B))<0){
    printf("no se recibio nada\n");
    return NULL;
  }

  pack = derialize(data);

  return pack;
}

