#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "packages.h"
#include "parser.h"
#include "comm.h"
#include <unistd.h>
#define TOTAL_COMMANDS 			9
#define MAX_COMMAND_LENGTH 		50
#define MAX_ARGUMENTS_LENGTH 	50
#define TRUE 					1
#define FALSE					0
#define MAX_COR 				8


static int INITIALIZED = 0;
static int sockfd=0;
void clear();

typedef struct commandDescriptor
{
	char *key;
	func handler;
	int argCount;
	char* info;
	int enabled;
} commandDescriptor;

commandDescriptor commands[TOTAL_COMMANDS];

Package pack;
int legajo;

void initCommandList(){
	commands[0].key = "inscribirse";
	commands[0].handler = (func)&inscribirseMateria_client;
	commands[0].argCount = 1;/*legajo y materia*/
	commands[0].info = "Permite al usuario inscribirse en una materia.";

	commands[1].key = "desinscribirse";
	commands[1].handler = (func)&desinscribirseMateria_client;
	commands[1].argCount = 1; /*legajo y materia*/
	commands[1].info = "Permite al usuario desinscribirse de una materia.";

	commands[2].key = "correlatividades";
	commands[2].handler = (func)&correlatividades_client;
	commands[2].argCount = 1;
	commands[2].info = "Permite al usuario ver las materias correlativas a una materia especifica.";

	commands[3].key = "materias";
	commands[3].handler = (func)&materias_client;
	commands[3].argCount = 0;
	commands[3].info = "Permite al usuario ver todas las materias.";

	commands[4].key = "cupo";
	commands[4].handler = (func)&cupo_client;
	commands[4].argCount = 1;
	commands[4].info = "Permite al usuario ver el cupo de la materia especificada con codigo.";

	commands[5].key = "misMaterias";
	commands[5].handler = (func)&misMaterias_client;
	commands[5].argCount = 0;
	commands[5].info = "Permite al usuario ver las materias correspondiente a su carrera.";	

	commands[6].key = "help";
	commands[6].handler = (func)&help;
	commands[6].argCount = 0;
	commands[6].info = "Permite al usuario ver los comandos.";

	commands[7].key = "salir";
	commands[7].handler = (func)&salir;
	commands[7].argCount = 0;
	commands[7].info = "Permite que el usuario cierre sesion.";

	commands[8].key = "iniciar";
	commands[8].handler = (func)&iniciarSesion_client;
	commands[8].argCount = 2;
	commands[8].info = "Permite al usuario ver todas las materias.";

}


int parser(char * buff, int sock){

	sockfd=sock;
	if ( INITIALIZED == 0){
		INITIALIZED++;
		initCommandList();
	}
	int index, argCount;
	char flag= 0;
	char* args [10];
	//printf("%s\n.",buff);
	argCount = split2(args, buff);

	if (argCount == -1){
		printf("Error3\n");
	}



	argCount -=1;
	for (index = 0; !flag && index < TOTAL_COMMANDS; index++){
		if (!strcmp(args[0],commands[index].key)){
			flag =1;
			if (argCount != commands[index].argCount){
				printf("%i arguments are required and %i were received.\n",commands[index].argCount, argCount);
			} else {
				//printf("%d\n",index);
				switch (commands[index].argCount){
					case 0:
						//commands[index].handler();

						if(!strcmp(args[0],"salir")){
							return 5;
						}else{
							commands[index].handler();
						}

						break;
					case 1:
						commands[index].handler(atoi(args[1]));
						break;
					case 2:
						//commands[index].handler(atoi(args[1]),atoi(args[2]));

						if(!strcmp(args[0],"inscribirse") || !strcmp(args[0],"desinscribirse")){
							commands[index].handler(atoi(args[2]),atoi(args[1]));
							return 0;
						}else{

							return (commands[index].handler(atoi(args[1]),args[2]));
						}

						break;
					case 3:
						commands[index].handler(atoi(args[1]),atoi(args[2]),atoi(args[3]));
						break;

				}
			}
		}
	}
	if(!flag){
		printf("Comando invalido.\n");
	}

	return -1;

}

int split2(char* args[], char* buffer){
	int i=0, m=0,j=1;
	int flag =0, er = 0;
	args[0] = buffer;
	while(buffer[i] && !er){
		if (buffer[i] == '\n'){
			buffer[i] = 0 ;
			er=1;
		} else if (buffer[i] == ' '&& !flag){
			buffer[i] = 0;
			args[j++] = buffer +i +1;
		} else if (buffer[i] == '\"') {
			if(!flag)
				args[j-1]= buffer +i +1;
			else
				buffer[i] = 0;
			flag=1-flag;
		}
		i++;
	}

	return j;
}




int salir(){
	return 5;
}

int help(){
	for(int i=0; i< TOTAL_COMMANDS -1 ; i++){

			printf("- %s: %s\n", commands[i].key, commands[i].info);

	}

	return 0;
}

int iniciarSesion_client(int legacy, char *password){

	return sesion(legacy,password);


}

int inscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud..\n");
	int result = inscribirseMateria(legajo, subCode);

	if (result == 1){
		printf("Inscripcion realizada con exito.\n");
	}else{
		printf("Ocurrio un error durante el proceso de inscripcion.\n");
	}
	return 0;
}

int desinscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud..\n");
	int result = desinscribirseMateria(legajo, subCode);
	if (result == 1){
		printf("Se ha eliminado la inscripcion con exito.\n");
	}else{
		printf("Ocurrio un error durante el proceso de desinscripcion.\n");
	}
	return 0;
}

int correlatividades_client(int subCode){
	printf("Obteniendo marterias correlativas..\n");
	char* response =correlatividades(subCode);
	printf("MATERIAS CORRELATIVAS\n%s",response);

	return 0;
}

int materias_client(void){
	printf("Obteniendo listado de materias..");
	char*response= materias();
	printf("CODIGO\tMATERIA\n%s \n", response);
	return 0;
}

int cupo_client(int subCode){
	printf("Obteniendo el cupo de la materia..\n");
	char *response = cupo(subCode);
	printf("CUPO DISPONIBLE DE LA MATERIA: %s\n",response);
	return 0;
}

int misMaterias_client(int legacy){
	printf("Obteniendo listado de materias correspondiente a la carrera del alumno..");

	char*response= misMaterias(legajo);
	printf("CODIGO\t\t\t\tNOTA\n%s \n", response);

	return 0;
}

int inscribirseMateria(int legacy, int subCode){
	pack.function = INSCRIBIR_MATERIA;
	pack.data.subC= subCode;
	pack.clientid = getpid();
	pack.data.sign.studentID = legacy;
	pack.size = sizeof(pack);
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);	
	clientReceivePackage(sockfd, &pack);

	if(atoi(pack.data.response) == -1){
		return -1;
	}else{
		 //clear();
		return 1;

	}
}
int desinscribirseMateria(int legacy, int subCode){
	pack.function = DESINSCRIBIR_MATERIA;
	pack.size = sizeof(pack);
	pack.clientid = getpid();
	pack.data.sign.studentID = legacy;
	pack.data.subC = subCode;
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);

	if(atoi(pack.data.response) == -1){
		return -1;
	}else{
		 //clear();
		return 1;

	}

}
char* correlatividades(int subCode){
	pack.function = CORRELATIVAS;
	pack.size = sizeof(pack);
	pack.clientid = getpid();
	pack.data.subC = subCode;
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);
	return pack.data.response;

}


char* materias(void){
	pack.function = MATERIAS;
	pack.size= sizeof(pack);
	pack.clientid= getpid();
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);
	return pack.data.response;
}


int sesion(int user, char *pass){

	pack.function = CHECK_USER;
	pack.size= sizeof(Package);
	strcpy(pack.pass,pass);

	pack.data.sign.studentID  = user;

	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);

	if(atoi(pack.data.response) == -1){
		return -1;
	}else{
		 legajo = user;
		 clear();
		return 1;

	}

}

char* cupo(int subCode){
	pack.function = CUPO;
	pack.size= sizeof(pack);
	pack.clientid= getpid();
	pack.data.subC = subCode;
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);
	return pack.data.response;
}

char * misMaterias(int legacy){
	pack.function = ALUMNO_MATERIAS;
	pack.size= sizeof(pack);
	pack.clientid= getpid();
	pack.data.sign.studentID  = legacy;
	limpiarBuffer(&pack);
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);
	return pack.data.response;
}

void clear(){
  for(int i=0; i< 25 ; i++){
    printf("\n");
  }
}


 void limpiarBuffer(Package * pack){
	memset(pack->data.response,0,4096);
}
