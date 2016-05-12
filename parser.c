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
#define TOTAL_COMMANDS 			4
#define MAX_COMMAND_LENGTH 		50
#define MAX_ARGUMENTS_LENGTH 	50
#define TRUE 					1
#define FALSE					0
#define MAX_COR 				8

int xd =54;
static int INITIALIZED = 0;
static int sockfd=0;

typedef struct commandDescriptor
{
	char *key;
	func handler;
	int argCount;
	char* info;
} commandDescriptor;

commandDescriptor commands[TOTAL_COMMANDS];

Package pack;

void initCommandList(){
	commands[0].key = "InscribirseMateria";
	commands[0].handler = (func)&inscribirseMateria_client;
	commands[0].argCount = 2;/*legajo y materia*/
	commands[0].info = "Permite al usuario inscribirse en una materia.";

	commands[1].key = "DesinscrbirseMateria";
	commands[1].handler = (func)&desinscribirseMateria_client;
	commands[1].argCount = 2; /*legajo y materia*/
	commands[1].info = "Permite al usuario desinscribirse de una materia.";

	commands[2].key = "Correlatividades";
	commands[2].handler = (func)&correlatividades_client;
	commands[2].argCount = MAX_COR;
	commands[2].info = "Permite al usuario ver las materias correlativas a una materia especifica.";

	commands[3].key = "Materias";
	commands[3].handler = (func)&materias_client;
	commands[3].argCount = 0;
	commands[3].info = "Permite al usuario ver todas las materias.";
/*
	commands[3].key = "Help";
	commands[3].handler = (func)&help;
	commands[3].argCount = 0;
	commands[3].info = "Permite al usuario ver los comandos disponibles.";	
**/}


int commandSelector(char* command){

	for(int i=0; i<TOTAL_COMMANDS;i++){
		if(strEquals(command, commands[i].key) == TRUE){
			return i;
		}
	}
	return -1;
}

void parser(char * buff, int sock){
	sockfd=sock;
	if ( INITIALIZED == 0){
		INITIALIZED++;
		initCommandList();
	}
	int index, argCount;
	char flag= 0;
	char* args [10];
	printf("%s\n.",buff);
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
				printf("%d\n",index);
				switch (commands[index].argCount){
					case 0:
						commands[index].handler();
						break;
					case 1:
						commands[index].handler(atoi(args[1]));
						break;
					case 2:
						commands[index].handler(atoi(args[1]),atoi(args[2]));
						break;
					case 3:
						commands[index].handler(atoi(args[1]),atoi(args[2]),atoi(args[3]));
						break;
						// etccccc
				}
			}
		}
	}
	if(!flag){
		printf("Invalid command.\n");
	}

}

int split2(char* args[], char* buffer){
	int i=0, m=0,j=1;
	int flag =0, er = 0;
	printf("Dentro de split   %s\n", buffer);
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
	if (er){
		printf("er true");
		//return -1;
	}
	return j;
}

void split(char* buffer, char* command, char* arguments){

  int i = 0;
  int j = 0;
  char c;

  do {
    c = buffer[i];
    command[i++] = c;

    if(c == ' ' || i == (MAX_COMMAND_LENGTH-1))
      break;

  } while( c != 0);

  command[i-1] = 0;

  if(c!=' '){
    arguments[j] = 0;
    return;
  }

  do {
    c = buffer[i++];
    arguments[j++] = c;

  } while(c != 0);

  arguments[j-1] = 0;
}

int strEquals(char* str1, char* str2){
	if(strlen(str1) != strlen(str2)) {
		return FALSE;
	}else{
		for(int i=0; i<strlen(str1); i++) {
			if(str1[i] != str2[i]) {
				return FALSE;
            }
    	}
    }

    return TRUE;
}


int inscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud.\n");
	char * result = inscribirseMateria(legacy, subCode);
	if (atoi(result) == 1){
		printf("Inscripcion realizada con exito.\n");
	}else{
		printf("Ocurrio un error durante el proceso de inscripcion.\n");
	}
	return 0;
}

int desinscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud.\n");
	char * result = desinscribirseMateria(legacy, subCode);
	if (atoi(result) == 1){
		printf("Se ha eliminado la inscripcion con exito.\n");
	}else{
		printf("Ocurrio un error durante el proceso de desinscripcion.\n");
	}
	return 0;
}

int correlatividades_client(int subCode){
	printf("Obteniendo marterias correlativas.\n");
	char* response =correlatividades(subCode);
	printf("%s\n",response);
	
	return 0;
}

int materias_client(void){
	printf("Obteniendo listado de materias");
	char*response= materias();
	printf("%s\n",response);
	return 0;
}

char* inscribirseMateria(int legacy, int subCode){
	pack.function = INSCRIBIR_MATERIA;
	pack.data.sign.subID= subCode;
	pack.data.sign.studentID = legacy;
	pack.size = sizeof(pack);
	//clientSendPackage(xd, &pack);
	//clientReceivePackage(xd, &pack);
	//return pack.data.response; 
	return "hola";
}
char* desinscribirseMateria(int legacy, int subCode){
	pack.function = DESINSCRIBIR_MATERIA;
	pack.size = sizeof(pack);
	pack.data.sign.studentID = legacy;
	pack.data.sign.subID = subCode;
//	clientSendPackage(xd, &pack);
//	clientReceivePackage(xd, &pack);
	//return pack.data.response; 
	return "hola";
}
char* correlatividades(int subCode){
	pack.function = CORRELATIVAS;
	pack.size = sizeof(pack);
	pack.data.subC = subCode;
	//clientSendPackage(xd, &pack);
	//clientReceivePackage(xd, &pack);
	return "hola";//pack.data.response; 

}


char* materias(void){
	pack.function = MATERIAS;
	pack.size= sizeof(pack);
	printf("JEJE");
	clientSendPackage(sockfd, &pack);
	clientReceivePackage(sockfd, &pack);
	//return pack.data.response;
	return "hola";
}