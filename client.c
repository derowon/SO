#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "packages.h"
#include "parser.h"
#include "comm.h"
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 4004 /*port*/
#define EXIT 5


void answer(Package * pack);
int login(char * buffer, int sockfd);


int sockfd = 0;
Package pack;
static int SOCKKK;


int
main(int argc, char **argv)
{
  char sendline[MAXLINE];
 /*int sockfd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];

 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server");
  exit(1);
 }

 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

 //Connection of the client to the socket
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server:s.");
  exit(3);
 }
 SOCKKK = sockfd;*/
 int aux;
 aux =iConnect_client();
 int isUserLogged=0;
 int parser_answer;
 char prompt [32];
 //while (fgets(sendline, MAXLINE, stdin) != NULL) {

  printf("Conectando con el servidor...\n");
  bzero(sendline,256);





   //int i= strlen(buffer);
    while(1){

      if(!isUserLogged){
        printf("NO ESTOY LOGUEADO \n");
         isUserLogged = login(sendline,sockfd);

      }else{

         printf("SGA $>:");
         fgets(sendline,MAXLINE,stdin);

          if((parser_answer = parser(sendline, sockfd)) == EXIT){
            isUserLogged = 0;
          }


            //TENGO QUE VER ESTE TEMA DE LIMPIAR EL BUFFER
            //printf("%s\n",sendline);
         }
         fflush(stdin);
    }

  //parser(sendline, aux);

  /*
  send(sockfd, sendline, strlen(sendline), 0);

  if (recv(sockfd, recvline, MAXLINE,0) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely");
   exit(4);
  }
  printf("%s", "String received from the server: ");
  fputs(recvline, stdout);*/
 //}

 //exit(0);
}


void answer(Package * pack){
  return;
}


int login(char * buffer, int sockfd){
    char * legajo, pass;
    int loginAccepted;
    char c;

    do{
    buffer[0]='i';
    buffer[1]='n';
    buffer[2]='i';
    buffer[3]='c';
    buffer[4]='i';
    buffer[5]='a';
    buffer[6]='r';
    buffer[7]=' ';



        int i= 8;
        printf("Bienvenido al Sistema de Gestion Academica (SGA).\n");
        printf("Ingrese numero de legajo:\n");

        while((c =getchar()) != '\n'){
          buffer[i++] = c;

        }

        buffer[i]= ' ';
        printf("Ingrese la contrasena:\n");
        ++i;

        while((c =getchar()) != '\n'){
          buffer[i++] = c;
        }

        buffer[i]='\0';

        loginAccepted = parser(buffer,sockfd);


      }while(loginAccepted < 0);

      return 1;

}
