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

#include "packages.h"
#include "comm.h"


#define SFIFO "/tmp/sfifo"

char clientFile[128];
int write2=-1, read2=-1;

int iConnect_server(void){
	mknod(SFIFO, S_IFIFO | 0666, 0);
	return 0;
}

int iConnect_client(){
	
	sprintf(clientFile, "/temp/cfifo%d", getpid());
	mknod(clientFile, S_IFIFO,F_OK);
	
	if(!access (SFIFO, F_OK)){
		
		printf("Connection to server established\n");
		write2 = open(SFIFO, O_WRONLY);

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

	write(write2, data, pack->size);

	free(data);

	return 0;
}

void clientReceivePackage(int con, Package* pack){
	if (read2<0){
		read2 = open(clientFile, O_RDONLY);
	}
	read(read2, (char*)pack, sizeof(Package));
	close(read2);


}

void handleRequest(int connfd){
	Package pack;
	int i;
	read2 = open(SFIFO, O_RDONLY);
	while (!read(read2, &pack,sizeof(char)));
	read(read2, &pack + sizeof(char),sizeof(Package) - sizeof(char));
	close(read2);
	printf("%d\n", pack.function);




}
