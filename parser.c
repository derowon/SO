#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "packages.h"


#define TOTAL_COMMANDS 			3
#define MAX_COMMAND_LENGTH 		50
#define MAX_ARGUMENTS_LENGTH 	50
#define TRUE 					1
#define FALSE					0
#define MAX_COR 				8

void initCommandList();
void processAction(char *buffer);
int commandSelector(char *command);
void split(char* buffer, char* command, char* arguments);
int strEquals(char* str1, char* str2);

void parser(char * buff);
int split2(char* args[], char* buffer);

typedef enum { MATERIAS, CORRELATIVAS, INSCRIBIR_MATERIA, DESINSCRIBIR_MATERIA} functions;


int inscribirseMateria_client(int legacy, int subCode);
int desinscribirseMateria_client(int legacy, int subCode);
int correlatividades_client(int subCode);
int materias_client(void);


char* inscribirseMateria(int legacy, int subCode);
char* desinscribirseMateria(int legacy, int subCode);
char* correlatividades(int subCode);
char* materias(void);

typedef int (*action)(char *);
typedef void (* func)();

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
}

/*void processAction(char *buffer){
	char command[MAX_COMMAND_LENGTH];
  	char arguments[MAX_ARGUMENTS_LENGTH];
	int commandId;
	int response = 0;

	if(strlen(buffer) == 0)
		return;

	split(buffer, command, arguments);

	commandId = commandSelector(command);

	if(commandId == -1){
		printf("%s is not a valid command", buffer);
	} else {
		response = commands[commandId].handler(arguments);
	}

}*/

int commandSelector(char* command){

	for(int i=0; i<TOTAL_COMMANDS;i++){
		if(strEquals(command, commands[i].key) == TRUE){
			return i;
		}
	}
	return -1;
}

void parser(char * buff){
	int index, argCount;
	char flag= 0;
	char* args [10];

	argCount = split2(args, buff);

	if (argCount == -1){
		printf("Error\n");
	}

	argCount -=1;

	for (index = 0; !flag && index < TOTAL_COMMANDS; index++){
		if (!strcmp(args[0],commands[index].key)){
			flag =1;

			if (argCount != commands[index].argCount){
				printf("%i arguments are required and %i were received.\n",commands[index].argCount, argCount);
			} else {
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
	args[0] = buffer;
	while(buffer[i] && !er){
		if (buffer[i] == '\n'){
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
		return -1;
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
	int result = inscribirseMateria(legacy, subCode);
	if (result == 1){
		printf("Inscripcion realizada con exito.\n");
	}else{
		printf("Ocurrio un error durante el proceso de inscripcion.\n");
	}
	return 0;
}

int desinscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud.\n");
	int result = desinscribirseMateria(legacy, subCode);
	if (result == 1){
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
	sendPackage(pack);
	receivePackage(pack);
	return pack.data.response; 
}
char* desinscribirseMateria(int legacy, int subCode){
	pack.function = DESINSCRIBIR_MATERIA;
	pack.size = sizeof(pack);
	pack.data.sign.studentID = legacy;
	pack.data.sign.subID = subCode;
	sendPackage(pack);
	receivePackage(pack);
	return pack.data.response; 
}
char* correlatividades(int subCode){
	pack.function = CORRELATIVAS;
	pack.size = sizeof(pack);
	pack.data.subC = subCode;
	sendPackage(pack);
	receivePackage(pack);
	return pack.data.response; 

}


char* materias(void){
	pack.function = MATERIAS;
	pack.size= sizeof(pack);
	sendPackage(pack);
	receivePackage(pack);
	return pack.data.response;
}