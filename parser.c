#include <string.h>
#include <stdio.h>

#define TOTAL_COMMANDS 3
#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS_LENGTH 50
#define TRUE 1
#define FALSE 0


void initCommandList();
void processAction(char *buffer);
int commandSelector(char *command);
void split(char* buffer, char* command, char* arguments);
int strEquals(char* str1, char* str2);

int inscribirseMateria(char *args);
int desinscribirseMateria(char *args);
int correlatividades(char *args);
int materias(char *args);

typedef int (*action)(char *);

typedef struct commandDescriptor
{
	char *key;
	action handler;
	char* info;
} commandDescriptor;

commandDescriptor commands[TOTAL_COMMANDS];


void initCommandList(){
	commands[0].key = "InscribirseMateria";
	commands[0].handler = &inscribirseMateria;
	commands[0].info = "Permite al usuario inscribirse en una materia.";

	commands[1].key = "DesinscrbirseMateria";
	commands[1].handler = &desinscribirseMateria;
	commands[1].info = "Permite al usuario desinscribirse de una materia.";

	commands[2].key = "Correlatividades";
	commands[2].handler = &correlatividades;
	commands[2].info = "Permite al usuario ver las materias correlativas a una materia especifica.";

	commands[3].key = "Materias";
	commands[3].handler = &materias;
	commands[3].info = "Permite al usuario ver todas las materias.";
}

void processAction(char *buffer){
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

}

int commandSelector(char* command){

	for(int i=0; i<TOTAL_COMMANDS;i++){
		if(strEquals(command, commands[i].key) == TRUE){
			return i;
		}
	}
	return -1;
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


int inscribirseMateria(char *args){
	
	return 0;
}

int desinscribirseMateria(char *args){
	
	return 0;
}

int correlatividades(char *args){
	return 0;
}

int materias(char *args){

	return 0;
}
