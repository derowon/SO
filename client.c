#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   int isUserLogged = 0;
   
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
         isUserLogged = login(&buffer,sockfd);

      }else{
         printf("Escribiendo el comando help, usted puede conocer los comandos disponibles.\n");
         fgets(buffer,255,stdin);

         /* Send message to the server */
          n = write(sockfd, buffer, strlen(buffer));

          if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);

            /* Now read server response */
            bzero(buffer,256);
            n = read(sockfd, buffer, 255);

            if (n < 0) {
               perror("ERROR reading from socket");
               exit(1);
            }
            //TENGO QUE VER ESTE TEMA DE LIMPIAR EL BUFFER
            printf("%s\n",buffer);
         }
      }
      
}
   return 0;
}


int login(char * buffer, int sockfd){
    char * legajo, pass;
    int loginAccepted;
    char c;

    buffer[0]='i';
    buffer[1]='n';
    buffer[2]='i';
    buffer[3]='c';
    buffer[4]='i';
    buffer[5]='a';
    buffer[6]='r';
    buffer[7]=' ';

    do{
               
        int i= strlen(buffer);
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

        //printf("%s\n", buffer);

        
         loginAccepted = parser(buffer,sockfd);

       // write(sockfd, buffer, strlen(buffer));

      }while(loginAccepted < 0);

      return 1;
   
}