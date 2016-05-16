#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "packages.h"

#define MAX_B 4112
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
   
   printf("HOla1\n");
   bzero((char *) &servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(address);
   servaddr.sin_port = htons(SERV_PORT);
   printf("HOla2\n");
}

void sendToClient(Package * pack){
	send(connfd,pack,sizeof(pack),0);
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
/*
void handleRequest(int conn, Package * pack){
	printf("Handle\n");
	int n1,n2;

	char buffer[256];
   	bzero(buffer,256);
	do{
		
		n1 = read(conn,buffer,256);
		printf("%s\n",buffer);
		if (n1 < 0) {
    	  perror("ERROR reading from socket");
     	  exit(1);
  		}
   
   		printf("Here is the message: %s\n",buffer);
   		n2= write(conn,"I got your message",18);

		}while(n1>0);
}

*/
int sendPackage(int con, Package* pack){
	printf("ACAAAA es el size del struct%d",sizeof(Package));
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

	printf("Entre al receivePackage\n");
  	int buffer_size=0;
  	if((buffer_size = read(con,data,MAX_B))<0){
    printf("no se recibio nada\n");
    return NULL;
  }
  //printf("%d\n",buffer_size );
  //printf("Este es el dato serializado %s\n " ,data);
  pack = derialize(data);

  //printf("Este es el user %d ",pack->studentID);

  //printf("ESTO ES LO QUE CONTIENE DATA DESPUES DE HACER EL MEMCPY: %d  %s\n", pack->user , pack->data.pass);

 
 // printf("toy saliendo del receivePackage\n");
  return pack;
}

