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

static int COUNT = 0;

#define SFIFO "/tmp/sfifo"

char clientFile[128];
int write2=-1, read2=-1;
int writeaux= 0;
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
		//writeaux= open(clientFile,O_WRONLY);
	}else{
		printf("Server could not be reached\n");
	}
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
		printf(" Y ACA QUE PASO%d", errno);
	}else{
		printf("Data sent correctly\n");
	}

	free(data);

	return 0;
}

void clientReceivePackage(int con, Package* pack){
	printf("Awaiting to read response\n");
	sprintf(clientFile,"/tmp/cf%d \n",getpid());
	printf("Attempting to open fd %s\n", clientFile);
	sleep(1);
	if(!access(clientFile, F_OK)){
		printf("fd exists\n");
	}
	if(!access(clientFile,R_OK)){
		printf("Can be read\n");
	}else {
		printf("Fail on read permits\n");
	}

	read2 = open(clientFile, O_RDONLY);
	
	printf("%s OPEN ERRNO \n", strerror(errno));

	while(!read(read2, pack, sizeof(int)));
	printf("After reading\n");
	int size= *((int*)pack);
	printf("%d read \n",read(read2, ((char*)pack) + sizeof(int), size-sizeof(int)));
	printf("%s errno",strerror(errno));
	printf("The response is %s \n",pack->data.response);
	close(read2);


}

/*void handleRequest(int connfd){
	Package pack;
	int i;
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, &pack,sizeof(char)));
	read(read2, &pack + sizeof(char),sizeof(Package) - sizeof(char));
	close(read2);
	printf("%d\n", pack.function);

}*/

void handleRequest (Package * pack){
	printf(" Handling request\n");
	char* hey ="Muy bien, aqui envio la respuesta correspondiente";
	memcpy(pack->data.response,(char*)hey, strlen(hey));
	sprintf(clientFile, "/tmp/cf%d", pack->clientid);
	if (0==	access(clientFile,F_OK))
		printf("Existe\n");
	
	if (0==access(clientFile,W_OK)){
		printf( "Puedo escribir el archivo \n");
	}
	
	//write2= open(clientFile, O_WRONLY);
	printf("%s ERRNO \n", strerror(errno));
	int count=0;
	
	count =write(open(clientFile,O_WRONLY), pack, *(int*)pack);
	printf("%d bytes written, %d should have been \n", count, sizeof(Package));
	//close(write2);
	return;
}


int serverReceivePackage(int sender, Package* pack){
	printf("Awaiting message\n");
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, pack, sizeof(int)));
	printf("After read while\n");
	int size = * ((int*)pack);
	read(read2, ((char*)pack) + sizeof(int), size- sizeof(int) );
	printf("Request coming from pack-> %d with function %d \n", pack->clientid, pack->function);
	close(read2);
	return 0;
}