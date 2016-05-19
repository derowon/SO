#include <stdlib.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <mqueue.h>
#include "packages.h"
#include "comm.h"
#include "dataBaseConnection.h"
#include "sharedFunctions.h"

#define MAXLINE 2048 /*max text line length*/
#define SERV_PORT 4004 /*port*/
#define LISTENQ 24 /*maximum number of client connections*/

#define SERVER_QUEUE_NAME   "/serverQueue"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10


int main (int argc, char **argv)
{
  int pid,fd,fd2, newconn,connfd,num;
  char *path = "/tmp/baseDeDatos";
	char *fromserver ="/tmp/client";
	char in_buffer [MSG_BUFFER_SIZE];
  void * buffer[sizeof(Package)];
	struct mq_attr attr;
	char name[50];
	mqd_t qd_server, qd_client;
	sem_t mutex;
	Package pack;

  /* Retornando el file descriptor de la conexcion */
  connfd = iConnect_server();
  /*Inicializo semaforo*/
  if(sem_init(&mutex,1,1) < 0){
        perror("semaphore initilization");
        exit(0);
  }

  pid = fork();
  if(pid==0){
      //creo el servidor del loggin
			printf("Sserver de login creado!\n" );
      int token_number = 1000;
      struct mq_attr attr;
      attr.mq_flags = 0;
      attr.mq_maxmsg = MAX_MESSAGES;
      attr.mq_msgsize = MAX_MSG_SIZE;
      attr.mq_curmsgs = 0;
      if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
          perror ("Server: mq_open (server)");
          exit (1);
      }

      char in_buffer [MSG_BUFFER_SIZE];
      char out_buffer [MSG_BUFFER_SIZE];

      while (1) {
          // get the oldest message with highest priority
          if (mq_receive (qd_server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
              perror ("Server: mq_receive");
              exit (1);
          }

          // send reply message to client
          if ((qd_client = mq_open (in_buffer, O_WRONLY)) == 1) {
              perror ("Server: Not able to open client queue");
              continue;
          }

          sprintf (out_buffer, "%ld", token_number);

          if (mq_send (qd_client, out_buffer, strlen (out_buffer), 0) == -1) {
              perror ("Server: Not able to send message to client");
              continue;
          }
          //printf ("Server: response sent to client.\n");
          token_number++;
      }

    }else{
      pid= fork();
      if(pid == 0){
      //Este seria el proceso que se encarga de la base de datos
        mkfifo(path, 0666);
			  printf("SERver de database creada\n" );
        tableCreate();
        fd = open(path,O_RDONLY);
        if(fd==-1){
          printf("ERROR en abrir el fifo \n" );
        }
        while(1){
           num = read(fd,buffer,sizeof(Package));
           if(num>0){

    					memcpy(&pack,buffer,sizeof(Package));
              snprintf(name, 50, "/tmp/client%i", pack.data.sign.studentID);
              fd2 = open(name, O_WRONLY);
    					printf("RECIBI EN EL DATABASE SERVER studentID : %d  y pass %s\n",pack.data.sign.studentID,pack.pass );
    					answer(&pack);
    					memcpy(buffer,&pack,sizeof(Package));
    					write(fd2,buffer,sizeof(Package));
    	        close(fd2);

          }
        }
        close(fd);
			  printf("DATABASE SERVER CLOSED\n");
      }else{
        while (1) {
   		     newconn = acceptConnection(connfd);
        		/* Create child process */
          pid = fork();
          if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
          }
          if (pid == 0) {
            /* This is the client process */
						close(connfd);
            //AGREGADOO PARA MESSAGE QUEUE
            char client_queue_name [64];
            sprintf (client_queue_name, "/sp-client-%d", getpid ());
            attr.mq_flags = 0;
            attr.mq_maxmsg = MAX_MESSAGES;
            attr.mq_msgsize = MAX_MSG_SIZE;
            attr.mq_curmsgs = 0;
            if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
              perror ("Client: mq_open (client)");
              exit (1);
            }
            if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
              perror ("Client: mq_open (server)");
              exit (1);
            }
              // send message to server
            if (mq_send (qd_server, client_queue_name, strlen (client_queue_name), 0) == -1) {
              perror ("Client: Not able to send message to server");
              continue;
            }
            // receive response from server
            if (mq_receive (qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
             perror ("Client: mq_receive");
             exit (1);
            }
             //termina lo de queue del cliente

						 //WHILE del server que se queda escuchando a los clientes
            while(1){
							printf("Ready to listen!\n" );
              receivePackage(newconn,&pack);
               if(&pack == NULL){
								 printf("SALIR PORQUE PACK ERA NULL\n" );
                  exit(0);
               }
							 snprintf(name, 50, "/tmp/client%i", pack.data.sign.studentID);
							 mkfifo(name,0666);
							 fd = open(path,O_WRONLY);
               if(fd<0){
                 printf("Error en el fd del proceso del cliente\n");
               }

              memcpy(buffer,&pack,sizeof(Package));
              sem_wait(&mutex);
             	write(fd,buffer,sizeof(Package));
              sem_post(&mutex);
             	fd2 = open(name,O_RDONLY);
             	read(fd2,buffer,sizeof(Package));
             	memcpy(&pack,buffer,sizeof(Package));
              sendToClient(newconn,&pack);

               close(fd);
               close(fd2);

            }
         }else{
           //Estoy en el padre que cierra el fd el cliente forkeado
            close(newconn);
         }

      } /* end of while */

    }
  }
}



void answer(Package * data){
  switch (data->function){
    case MATERIAS:
      materias_db(data);
      break;
    case INSCRIBIR_MATERIA:
      subscribeSubject_db(data);
      break;
    case DESINSCRIBIR_MATERIA:
      cancelSubscription_db(data);
      break;
    case CORRELATIVAS:
      correlatives_db(data);
      break;
    case CHECK_USER:
      checkUser_db(data);
      break;
  }
  return;
}

//materias
void materias_db(Package * data){
  char* buffer = getSubjects();
  memcpy(data->data.response, buffer,strlen(buffer));
}


//suscrbirseMateria
Package * subscribeSubject_db(Package * data){
  int result;
  char * buffer;

  result = subscribeSubject(data->data.sign.subID, data->data.sign.studentID);

  if(result == 1){
    buffer = "1";
    memcpy(data->data.response, buffer, strlen(buffer));
  }else{
    buffer = "0";
    memcpy(data->data.response, buffer, strlen(buffer));
  }

  return data;

}

//Cancelarsubscripcion
Package * cancelSubscription_db(Package * data){
  int result;
  char * buffer;

  result = cancelSubscription(data->data.sign.subID, data->data.sign.studentID);

  if(result == 1){
    buffer = "1";
    memcpy(data->data.response, buffer, strlen(buffer));
  }else{
    buffer = "0";
    memcpy(data->data.response, buffer, strlen(buffer));
  }
  return data;
}


//correlatividades

Package * correlatives_db(Package * data){
  char* buffer;
  printf("Correlatividades con codigo %d \n",data->data.subC);
  buffer = seeCorrelatives(data->data.subC);

  memcpy(data->data.response, buffer,strlen(buffer));

  return data;
}


//chequear usuario

Package * checkUser_db(Package * data){
  char * buffer;


  buffer = checkUser(data->data.sign.studentID, data->pass);

  printf("VOLVI DEL CHECKUSER");

  if(strcmp(buffer, "USUARIO INCORRECTO") == 0 || strcmp(buffer, "PASSWORD INCORRECTA")==0){
    strcpy(data->data.response, "-1");

  }else{
    strcpy(data->data.response, "1");
  }

  return data;
}
