#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include "sockets.h"
#include "parser.h"

#define SALIR 5

int login(char * buffer, int sockfd);

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   int isUserLogged = 0;
   int parser_answer;   
   char buffer[256];
   
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
  
   
   /* Create a socket point */
   sockfd  = initSockets(); 
   prepareToConnect(argv[1]);

   /* Now connect to the server */
    connectToServ(sockfd);
   
  
    //TODO 
    //hay que poner el parser aca
   //printf("Please enter the message: ");
   printf("Conectando con el servidor...\n");
   printf("Bienvenido al Sistema de Gestion Academica (SGA).\n");
   bzero(buffer,256);




   
   //int i= strlen(buffer);
    while(1){

      if(!isUserLogged){
         isUserLogged = login(buffer,sockfd);
         printf("QUE CARAJO ME DEVOLVIO LOGIN??. ESTO : %d \n", isUserLogged);

      }else{
         printf("Escribiendo el comando help, usted puede conocer los comandos disponibles.\n");
         fgets(buffer,255,stdin);

          if(parser_answer = parser(buffer, sockfd) == SALIR){
            isUserLogged = 0;
          }

        
            //TENGO QUE VER ESTE TEMA DE LIMPIAR EL BUFFER
            printf("%s\n",buffer);
         }
      }
    

   return 0;
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

        printf("DO WHILE STRING BUFFER%s\n", buffer);

          printf("antes de parsear HIJO DE PUTA\n");
         loginAccepted = parser(buffer,sockfd);
         printf("loginAccepted valeeee: %d \n", loginAccepted);

       // write(sockfd, buffer, strlen(buffer));

      }while(loginAccepted < 0);

      return 1;
   
}