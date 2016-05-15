#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "packages.h"
#include "comm.h"

#define MAXLINE 2048 /*max text line length*/
#define SERV_PORT 3686 /*port*/
#define LISTENQ 24 /*maximum number of client connections*/

void respond(Package * data);
void materias_db(Package * data);
void handleRequest (int connfd);

int main (int argc, char **argv)
{
 int listenfd, connfd, n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;

 //Create a socket for the soclet
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

 for ( ; ; ) {

  clilen = sizeof(cliaddr);
  //accept a connection
  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

  printf("%s\n","Received request...");

  if ( (childpid = fork ()) == 0 ) {//if it’s 0, it’s child process

  printf ("%s\n","Child created for dealing with client requests");

  //close listening socket
  close (listenfd);
  handleRequest(connfd);
  
 }
 //close socket of the server
 close(connfd);
}
}




void respond(Package * data){
  switch (data->function){
    case MATERIAS:
      materias_db(data);
      break;
  }
  return;
}


void materias_db(Package * data){
  char* buffer = "LAS MATERIAS\n \n XD \n XJAJA\n....";
  memcpy(data->data.response, buffer,strlen(buffer));
  
}