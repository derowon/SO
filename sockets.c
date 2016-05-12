#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "packages.h"
#include "comm.h"


int clientSendPackage(int con, Package* pack){
  void* data = calloc(pack->size,1);
  memcpy(data,pack, pack->size);
  if (send(con, data,pack->size,0)<0)
    return -1;
  free(data);
  return 0;
}

void clientReceivePackage(int con,Package* pack){
  void * data = calloc(4096,1); //change for maxsize constant
  int buffer_size=0;
  if ((buffer_size = recv(con, data, 4096,0))<0){
    printf("no se recibio nada\n");
    return;
  }
  printf("ANTES DE COPIAR Y DESPUES DE RECIBIR.\n");
  memcpy(pack,data,buffer_size);
  printf("DESPUES DE COPIAR.\n");
  printf("%s\n", pack->data.response);
  free(data);

  return;
}