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
 int aux;
 aux =iConnect_client();
 int isUserLogged=0;
 int parser_answer;
 char prompt [32];

  printf("Conectando con el servidor...\n");
  bzero(sendline,256);


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
         }
    }
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
