#include "packages.h"

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

	
	return pack;
}

	