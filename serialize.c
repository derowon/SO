#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "packages.h"

void serialize(Package *pack,char * buff){
	int i=0;
	int size= 0;
	//SIZE
	memcpy(&buff[i],&pack->size,sizeof(pack->size));
	i+=sizeof(pack->size);

	//Function
	memcpy(&buff[i],&pack->function,sizeof(pack->function));
	i+=sizeof(pack->function);

	//StudentID
	memcpy(&buff[i],&pack->studentID,sizeof(pack->studentID));
	i+=sizeof(pack->studentID);

	//subID
	memcpy(&buff[i],&pack->subID,sizeof(pack->subID));
	i+=sizeof(pack->subID);

	//pongo el largo del pass
	size = strlen(pack->pass);
	memcpy(&buff[i],&size,sizeof(size));
	i+=sizeof(size);

	//pass
	strncpy(&buff[i],pack->pass,strlen(pack->pass));
	i+=strlen(pack->pass);
	
	//response length
	size = strlen(pack->response);
	memcpy(&buff[i],&size,sizeof(size));
	i+=sizeof(size);

	//Response

	strncpy(&buff[i],pack->response,size);
	i+=size;

	
}

Package *  derialize(char* buff){
	int i =0;
	Package *pack = calloc(sizeof(Package),1);
	int size=0;
	//SIZE
	memcpy(&pack->size,&buff[i],sizeof(int));
	i+=sizeof(int);

	//Function
	memcpy(&pack->function,&buff[i],sizeof(int));
	i+=sizeof(int);

	//StudentID
	memcpy(&pack->studentID,&buff[i],sizeof(int));
	i+=sizeof(int);
	//printf("Este es el user dentro de derialize : %d\n", pack->studentID);
	//subID 
	memcpy(&pack->subID,&buff[i],sizeof(int));
	i+=sizeof(int);


	//largo del pass
	memcpy(&size,&buff[i],sizeof(int));
	i+=sizeof(int);


	strncpy(pack->pass,&buff[i],size);
	(pack->pass)[i+size+1] = '\0';
	i+=size;	


	//largo del pass
	memcpy(&size,&buff[i],sizeof(int));
	i+=sizeof(int);
	//response
	
	strncpy(pack->response,&buff[i],size);
	i+=size;
	if(size>0){
		printf("DENTRO DEL SERIALIZE %s\n",pack->response );
	}
	
	return pack;
}

	