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
	
	printf("%s \n ",clientFile);
	mknod(clientFile, S_IFIFO | 0666, 0);
	printf("%s ERRNOO mknod\n",strerror(errno));
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
	printf("Awaiting message\n");
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, &paq, sizeof(int)));
	printf("After read while\n");
	int size = * ((int*)(&paq));
	read(read2, ((char*)(&paq)) + sizeof(int), size- sizeof(int) );
	printf("Request coming from pack-> %d with function %d \n", paq.clientid, paq.function);
	printf("legajo :  %d   Pass :  %s\n", paq.data.sign.studentID, paq.pass);
	close(read2);
	return 0;
}

int clientSendPackage(int con, Package* pack){
	
	void* data = calloc(pack->size,1);
  	memcpy(data,pack, pack->size);
  	// printf("BEFORE WRITEAUX\n");
  	// printf("CLIENTFILE ***%s***\n",clientFile);
  	// sleep(1);
  	// writeaux= open(clientFile,O_WRONLY);
  	// printf("After WRITEAUX\n");
  	// sleep(5);
	if (write2 <0 || access(SFIFO, F_OK)){
		printf("Connection failed\n");
		return -1;
	}

	if (write(write2, data, pack->size)<0){
		printf(" Y ACA QUE PASO%d", errno);
	}else{
		printf("Data sent correctly\n");
	}
	

	// writeaux= open(clientFile,O_RDONLY);
	//printf("DESPUES DEL OPEN\n");
	free(data);

	return 0;
}


int clientReceivePackage(int con, Package* pack){
	int aux=0;
	printf("Awaiting to read response\n");
	//sprintf(clientFile,"/tmp/cf%d \n",getpid());
	printf("Attempting to open fd %s\n", clientFile);
	printf("BEFOREWRITEAUX\n");
	printf("CLIENTFILE ***%s***\n",clientFile);
  	printf("ANTES DE OPEN\n");
  	writeaux= open(clientFile,O_RDONLY);
  	printf("After WRITEAUX\n");
  	sleep(0.1);

	while(!(aux = read(writeaux, pack, sizeof(int)))){
		printf("Attempting to read\n");
		sleep(0.5);
	}
	printf("After reading ***%d***\n",aux);
	int size= sizeof(Package);
	
	read(writeaux, ((char*)pack) + sizeof(int), size-sizeof(int));
	
	//printf("The response is %s \n",pack->data.response);
	sleep(3);
	//close(read2);

	return 0;
}

void sendToClient(int newconn,Package * pack){
	printf("OPENING PARA ENVIAR A -----------%s-------- \n", clientFile);
	
	write2= open(clientFile, O_WRONLY);
	printf("AFTER OPENED %s y write devolvio : %d\n", clientFile, write2);
	sleep(0.5);
	int count=0;
	printf("BEFORE WRITE con ans\n_______%s_______\n",pack->data.response);
	// count =write(open(clientFile,O_WRONLY), &paq, sizeof(Package));
	count =write(write2, pack, sizeof(Package));
	printf("after write se escribieron *************%d**********\n", count);
	sleep(5);
	memset(&paq,0,sizeof(Package));
	close(write2);
}

int serverReceivePackage(int sender, Package* pack){
	/*printf("Awaiting message\n");
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, pack, sizeof(int)));
	printf("After read while\n");
	int size = * ((int*)pack);
	read(read2, ((char*)pack) + sizeof(int), size- sizeof(int) );*/
	memcpy(pack, &paq, sizeof(Package));
	
	if (paq.clientid == 0){
		printf("\n\n\nlooping request    --->>> exiting\n\n\n");
		
		_Exit(1);
	}
	sprintf(clientFile, "/tmp/cf%d", paq.clientid);
	printf("*****************************************************************\n");
	printf("Received package from client and fileName is : %s \n", clientFile);
	
	sleep(0.1);
	printf("Request coming from pack-> %d with function %d \n", paq.clientid, paq.function);
	printf("legajo :  %d   Pass :  %s\n", paq.data.sign.studentID, paq.pass);
	close(read2);
	return 0;
}

/*void handleRequest (int address,Package * pack){
	printf(" Handling request\n");


	answer(pack);

	printf("Lo que tiene materias es: %s\n", pack->data.response);
	//memcpy(pack->data.response,materias, strlen(materias));
	sprintf(clientFile, "/tmp/cf%d", pack->clientid);


	write2= open(clientFile, O_WRONLY);
	int count=0;
	
	count =write(open(clientFile,O_WRONLY), pack, *(int*)pack);
	memset(pack,0,sizeof(Package));
	close(write2);
	//printf("%d bytes written, %d should have been \n", count, sizeof(Package));
	//close(write2);
	return;
}*/


