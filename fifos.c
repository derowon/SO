#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "packages.h"
#include "comm.h"
#include "dataBaseConnection.h"
#include "sqlite.h"



static int COUNT = 0;

#define SFIFO "/tmp/sfifo"

char static clientFile[128];
int write2=-1, read2=-1;
int writeaux= 0;
Package paq;
int iConnect_server(void){
	printf("Server fifo created\n");
	mknod(SFIFO, S_IFIFO | 0666, 0);
	printf("Server could be created %d  \n",access(SFIFO, F_OK));
	return 0;
}

int iConnect_client(){
	
	sprintf(clientFile, "/tmp/cf%d", getpid());
	
	
	mknod(clientFile, S_IFIFO | 0666, 0);
	
	if(!access(clientFile, W_OK))
		printf("Client file ready to be written\n");
	if(!access (SFIFO, F_OK)){
		printf("Connection to server established\n");
		write2 = open(SFIFO, O_WRONLY);
	}else{
		printf("Server could not be reached\n");
	}
	return 0;
}


int acceptConnection(int fd){
	
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, &paq, sizeof(int)));

	int size = * ((int*)(&paq));
	read(read2, ((char*)(&paq)) + sizeof(int), size- sizeof(int) );

	close(read2);
	return 0;
}

int clientSendPackage(int con, Package* pack){
	
	void* data = calloc(pack->size,1);
  	memcpy(data,pack, pack->size);
 
	if (write2 <0 || access(SFIFO, F_OK)){
		printf("Connection failed\n");
		return -1;
	}

	if (write(write2, data, pack->size)<0){

	}else{

	}
	

	free(data);

	return 0;
}


int clientReceivePackage(int con, Package* pack){
	int aux=0;

	writeaux= open(clientFile,O_RDONLY);

	while(!(aux = read(writeaux, pack, sizeof(int)))){

	}
	int size= sizeof(Package);
	
	read(writeaux, ((char*)pack) + sizeof(int), size-sizeof(int));


	return 0;
}

void sendToClient(int newconn,Package * pack){

	write2= open(clientFile, O_WRONLY);
	
	int count=0;
	
	count =write(write2, pack, sizeof(Package));
	sleep(5);
	memset(&paq,0,sizeof(Package));
	close(write2);
}

int serverReceivePackage(int sender, Package* pack){
	
	memcpy(pack, &paq, sizeof(Package));
	
	if (paq.clientid == 0){
	
		
		_Exit(1);
	}
	sprintf(clientFile, "/tmp/cf%d", paq.clientid);
	
	close(read2);
	return 0;
}

