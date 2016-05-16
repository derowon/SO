#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include "packages.h"
#include "dataBaseConnection.h"



int main( int argc, char *argv[] ) {
   int fd,fd2;
   int sockfd, newsockfd;
   char buffer[MAX_B];
   Package *pack;
   int i=0;
   int n, pid;
   char *path = "/tmp/baseDeDatos";
   char *fromserver ="/tmp/client";
   char name[50];
   mkfifo(path, 0666);

   
   /* First call to socket() function */
   sockfd = initSockets(); 
   prepareToListen(sockfd);

   pid= fork();
   if(pid == 0){
      //este seria el proceso que se encarga de la base de datos
      //fd2 = open
      fd = open(path,O_RDONLY);
      while(read(fd,buffer,MAX_B)> 0){
         pack = derialize(buffer);
         printf("Recibi bien ID %d  y %s !!\n",pack->studentID,pack->pass);
         snprintf(name, 50, "/tmp/client%i", pack->studentID);
         fd2 = open(name, O_WRONLY);
         //hago la consulta
         respond(pack);

         write(fd2, buffer, MAX_B);
         close(fd2);
         printf("SALGO DE LA CONSULTA DE BASE DE DATOS \n");
      }
      close(fd);

   }else{
 
      while (1) {
   		newsockfd = acceptConnection(sockfd);
         if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
         }
         
         /* Create child process */
         pid = fork();
   		
         if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
         }
         
         if (pid == 0) {
            /* This is the client process */
            close(sockfd);
            while(1){
               pack = receivePackage(newsockfd,pack);
               printf("RECIBI el pack\n");
               printf("\n\n\n************LO QUE ALMACENE EN DATA COMO USER ES: %d**********\n", pack->studentID);
               printf("\n\n\n************LO QUE ALMACENE EN DATA COMO PASS ES: %s**********\n", pack->pass);
               
               snprintf(name, 50, "/tmp/client%i", pack->studentID);
               printf("%s\n",name );
               mkfifo(name,0666);

               serialize(pack,buffer);
               fd = open(path, O_WRONLY);
               write(fd, buffer, MAX_B);

               fd = open(name,O_RDONLY);
               read(fd,buffer,MAX_B);
               pack = derialize(buffer);
               printf("Justo antes de mandarselo al cliente\n");
               printf("ESTE ES el response %s\n", pack->response);
               sendPackage(newsockfd,pack);
               close(fd);
               exit(0);
            }
         }
         else {
            close(newsockfd);
         }
   		
      } /* end of while */
   }
}

void respond(Package * data){
  // printf("\n\nESTOY EN respond:\n");
  // printf("EN RESPOND , A PUNTO DE INGRESAR EN LA FUNCTION %d\n", data->function);
  switch (data->function){
    case SUBJECTS:
    //  printf("\n\n\n@@@@@@@@La funcion que me toca atender en la base de datos es:@@@@@@@@\n");
    //  printf("@@@@@@@@subjects_db@@@@@@@@\n");
      subjects_db(data);
      break;

    case SUBSCRIBE_SUBJECT:
      subscribeSubject_db(data);
      break;

    case CANCEL_SUBSCRIPTION:
      cancelSubscription_db(data);
      break;

    case CORRELATIVES:
      correlatives_db(data);
      break;

    case CHECK_USER:
   //   printf("Estoy por entrar en checkUser_db \n");
      checkUser_db(data);
      break;
  }
  return;
}


void subjects_db(Package * data){
  char* buffer;
 
  buffer = getSubjects();
 // printf("\n\n####Lo que me devolvio la base de datos es: #####\n");
 // printf("####%s #####\n",buffer);

 // printf("\n\n\n^^^^^^^^Copiando a data.response:^^^^^^\n");
  memcpy(data->response, buffer,strlen(buffer));
//  printf("\n^^^^^^^^Ahora data->data.response contiene lo siguiente:^^^^^^\n");
//  printf("\n^^^^^^^^%s^^^^^^\n", data->response);
}

void subscribeSubject_db(Package * data){
  int result;
  char * buffer;
  
  result = subscribeSubject(data->subID, data->studentID);
  
  if(result == 1){
    buffer = "La inscripcion se realizo con exito.";
    memcpy(data->response, buffer, strlen(buffer));
  }else{
    buffer = "No se pudo realizar la inscripcion.";
    memcpy(data->response, buffer, strlen(buffer));
  }

}

void cancelSubscription_db(Package * data){
  int result;
  char * buffer;
  
  result = cancelSubscription(data->subID, data->studentID);

  if(result == 1){
    buffer = "Usted ya no se encuentra inscripto en esta materia.";
    memcpy(data->response, buffer, strlen(buffer));
  }else{
    buffer = "No se pudo realizar el pedido con exito.";
    memcpy(data->response, buffer, strlen(buffer));
  }
}


void correlatives_db(Package * data){
  char* buffer;

  buffer = seeCorrelatives(data->subID);
  //printf("EL BUFFER CONTIENE: \n%d\n", buffer);
  memcpy(data->response, buffer,strlen(buffer));

 // printf("ESTO ES LO QUE CONTIENE DATA-> DATA.RESPONSE:---     \n%s\n", data->response);

}

void checkUser_db(Package * data){
  char * buffer;

 // printf("\nEstoy en checkUser_db\n");
  //printf("\n\n****EL PAQUETE ENVIADO POR EL CLIENTE CONTIENE:***\n");
 // printf("EL USUARIO QUE ES: %d\n", data->studentID);
 // printf("LA CONTRASENA QUE ES: %s\n", data->pass);

  buffer = checkUser(data->studentID, data->pass);

  printf("VOLVI DEL CHECKUSER");

  if(strcmp(buffer, "USUARIO INCORRECTO") == 0 || strcmp(buffer, "USUARIO INCORRECTO")==0){
    strcpy(data->response, "-1");
  }else{
    strcpy(data->response, "1");
  }
}




