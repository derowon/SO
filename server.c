#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "packages.h"
#include "comm.h"
#include "dataBaseConnection.h"
#include "sharedFunctions.h"

#define MAXLINE 2048 /*max text line length*/
#define SERV_PORT 4004 /*port*/
#define LISTENQ 24 /*maximum number of client connections*/




int main (int argc, char **argv)
{
 Package pack;
 tableCreate();
 int childpid, con,list, dbpid;
 list=iConnect_server();
 while (1){
    con =serverReceivePackage( list, &pack);
    printf("Request received from client:%d \n", pack.clientid);

    if ( (childpid = fork()) ==0){
     
      handleRequest(con,&pack);
      exit(0);
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
  printf("NICO PUTO %s\n",buffer );
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






