#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <mqueue.h>
#include <string.h>
#include "packages.h"
#include "dataBaseConnection.h"
#include "sockets.h"
#include "serialize.h"

#define SERVER_QUEUE_NAME   "/serverQueue"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

Package * checkUser_db(Package * data);
Package * respond(Package * data);
Package * subjects_db(Package * data);
Package * subscribeSubject_db(Package * data);
Package * cancelSubscription_db(Package * data);
Package * correlatives_db(Package * data);


int main( int argc, char *argv[] ) {
   int fd,fd2;
   int sockfd, newsockfd;
   char buffer[MAX_B];
   Package *pack;
  char in_buffer [MSG_BUFFER_SIZE];
    char temp_buf [10];
   int i=0;
   struct mq_attr attr;
   int n, pid;
   char *path = "/tmp/baseDeDatos";
   char *fromserver ="/tmp/client";
   mqd_t qd_server, qd_client; 
   sem_t mutex;
   char name[50];
   mkfifo(path, 0666);

   
   /* First call to socket() function */
   sockfd = initSockets(); 
   prepareToListen(sockfd);
   /*Inicializo semaforo*/
    if( sem_init(&mutex,1,1) < 0)
    {
      perror("semaphore initilization");
      exit(0);
    } 
  pid = fork();
  if(pid==0){
    //creo el servidor del loggin  
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
      //este seria el proceso que se encarga de la base de datos
      //fd2 = open
      tableCreate();
      fd = open(path,O_RDONLY);
      while(read(fd,buffer,MAX_B)> 0){
         pack = derialize(buffer);
      
         snprintf(name, 50, "/tmp/client%i", pack->studentID);
         fd2 = open(name, O_WRONLY);
         //hago la consulta
         pack = respond(pack);
         
         serialize(pack,buffer);
         write(fd2, buffer, MAX_B);
         close(fd2);
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


            //AGREGADOO
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
          
             // 

            close(sockfd);
            while(1){
               pack = receivePackage(newsockfd,pack);
               if(pack == NULL){
                  exit(0);
               }
               if((pack->size == 0 )&&(pack->function == 0 )&&(pack->studentID == 0 )&& (pack->subID == 0 )){
                exit(0);
               }
               
               snprintf(name, 50, "/tmp/client%i", pack->studentID);
               mkfifo(name,0666);

               serialize(pack,buffer);
               fd = open(path, O_WRONLY);
               sem_wait(&mutex);
               write(fd, buffer, MAX_B);
               sem_post(&mutex);
               fd = open(name,O_RDONLY);
               read(fd,buffer,MAX_B);
               pack = derialize(buffer);
            
               sendPackage(newsockfd,pack);
               close(fd);
               
            }
         }
         else {
            close(newsockfd);
         }
   		
      } /* end of while */
   }
 }
}

Package * respond(Package * data){
   

  switch (data->function){
    case SUBJECTS:
    
      return subjects_db(data);
      break;

    case SUBSCRIBE_SUBJECT:
      return subscribeSubject_db(data);
      break;

    case CANCEL_SUBSCRIPTION:
      return cancelSubscription_db(data);
      break;

    case CORRELATIVES:
      return correlatives_db(data);
      break;

    case CHECK_USER:
   
      return checkUser_db(data);
      
      break;
  }
  return NULL;
}


Package * subjects_db(Package * data){
  char* buffer;
 
  buffer = getSubjects();
 
  memcpy(data->response, buffer,strlen(buffer));

  return data;
}

Package * subscribeSubject_db(Package * data){
  int result;
  char * buffer;
  
  result = subscribeSubject(data->subID, data->studentID);
  
  if(result == 1){
    buffer = "1";
    memcpy(data->response, buffer, strlen(buffer));
  }else{
    buffer = "0";
    memcpy(data->response, buffer, strlen(buffer));
  }

  return data;

}

Package * cancelSubscription_db(Package * data){
  int result;
  char * buffer;
  
  result = cancelSubscription(data->subID, data->studentID);

  if(result == 1){
    buffer = "1";
    memcpy(data->response, buffer, strlen(buffer));
  }else{
    buffer = "0";
    memcpy(data->response, buffer, strlen(buffer));
  }
  return data;
}


Package * correlatives_db(Package * data){
  char* buffer;

  buffer = seeCorrelatives(data->subID);
  
  memcpy(data->response, buffer,strlen(buffer));

  return data;
}

Package * checkUser_db(Package * data){
  char * buffer;


  buffer = checkUser(data->studentID, data->pass);

  printf("VOLVI DEL CHECKUSER");

  if(strcmp(buffer, "USUARIO INCORRECTO") == 0 || strcmp(buffer, "PASSWORD INCORRECTA")==0){
    strcpy(data->response, "-1");

  }else{
    strcpy(data->response, "1");
  }

  return data;
}




