#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "packages.h"

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

void handleRequest(int conn, Package * pack){
	printf("Handle\n");
	int n1,n2;

	char buffer[256];
   	bzero(buffer,256);
	do{
		//printf("Tamanio del pack %d\n",sizeof(Package) );
		//n = read(conn,&pack,sizeof(Package));
		//printf("Este es el size de lo leido %d \n %s", n ,pack->data.response);
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
