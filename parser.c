#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "packages.h"
#include "parser.h"

#define TOTAL_COMMANDS 			6
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
	int enabled;
} commandDescriptor;

commandDescriptor commands[TOTAL_COMMANDS];

Package * pack;

void initCommandList(){
	commands[0].key = "inscribirse";
	commands[0].handler = (func)&inscribirseMateria_client;
	commands[0].argCount = 2;/*legajo y materia*/
	commands[0].info = "Permite al usuario inscribirse en una materia.";
	commands[0].enabled = 0;

	commands[1].key = "desinscrbirse";
	commands[1].handler = (func)&desinscribirseMateria_client;
	commands[1].argCount = 2; /*legajo y materia*/
	commands[1].info = "Permite al usuario desinscribirse de una materia.";
	commands[1].enabled = 0;

	commands[2].key = "correlatividades";
	commands[2].handler = (func)&correlatividades_client;
	commands[2].argCount = 1;
	commands[2].info = "Permite al usuario ver las materias correlativas a una materia especifica.";
	commands[2].enabled = 0;

	commands[3].key = "materias";
	commands[3].handler = (func)&materias_client;
	commands[3].argCount = 0;
	commands[3].info = "Permite al usuario ver todas las materias.";
	commands[3].enabled = 0;

	commands[4].key = "help";
	commands[4].handler = (func)&help;
	commands[4].argCount = 0;
	commands[4].info = "Permite al usuario ver los comandos disponibles.";
	commands[4].enabled = 0;

	commands[5].key = "iniciar";
	commands[5].handler = (func)&iniciarSesion_client;
	commands[5].argCount = 2;
	commands[5].info = "Permite al usuario iniciar sesion.";	
	commands[0].enabled = 1;


}

/*
int loggin(char* buff, int sock){

	sockfd=sock;
	
	if ( INITIALIZED == 0){
		INITIALIZED++;
		initCommandList();
	}

	int argCount;
	char* args [10];
	printf("%s\n.",buff);
	argCount = split2(args, buff);



	if (argCount == -1){
		printf("Error\n");
		return -1;
	}

	argCount -=1;
	printf("\n\n\n*********Lo que estoy parseando es: ******** \n");
	printf("%s \n",buff);
	printf("$$$$$$$Los argumentos que le estoy pasando son: $$$$$$\n");
	printf("%s - %s\n", args[1],args[2]);
	if (argCount != commands[5].argCount){
				printf("%i arguments are required and %i were received.\n",commands[5].argCount, argCount);
				return -1;
	} else{
		commands[5].handler(atoi(args[1]),args[2]);
	}

	return 1;
}*/


int parser(char * buff, int sock){
	pack = calloc(MAX_B,1);
	sockfd=sock;
	int a;
	if (INITIALIZED == 0){
		INITIALIZED++;
		initCommandList();
	}

	int index, argCount,j;
	char flag= 0;
	char* args [10];
	printf("%s\n.",buff);
	argCount = split(args, buff);



	if (argCount == -1){
		printf("Error\n");
	}

	argCount -=1;
	printf("\n\n\n*********Lo que estoy parseando es: ******** \n");
	printf("*********%s ******** \n",buff);

	for (index = 0; !flag && index < TOTAL_COMMANDS; index++){
		if (!strcmp(args[0],commands[index].key)){
			flag =1;

			if (argCount != commands[index].argCount){
				printf("%i arguments are required and %i were received.\n",commands[index].argCount, argCount);
			} else {

				//si es iniciar sesion, tengo que habilitar los otros comandos y deshabilitar el enabled de iniciar sesion.

				if(!strcmp(args[0],"iniciar")){
					for(j =0; j < TOTAL_COMMANDS;j++){
						if(commands[j].enabled == 0){
							commands[j].enabled = 1;
						}else{
							commands[j].enabled = 0;
						}
					}
				}

				//si es cerrar sesion, tengo que deshabilitar los comandos y habilitar el enabled de iniciar sesion.
				if(!strcmp(args[0],"salir")){
					for(j =0; j < TOTAL_COMMANDS;j++){
						if(commands[j].enabled == 0){
							commands[j].enabled = 1;
						}else{
							commands[j].enabled = 0;
						}
					}	
				}
				
				printf("%d\n",index);
				switch (commands[index].argCount){
					case 0:
						printf("\n\n*****La funcion a la que estoy entrando es:*****\n");
						printf("****%s******\n",commands[index].info);
						commands[index].handler();
						
						break;
					case 1:
						printf("\n\n*****La funcion a la que estoy entrando es:*****\n");
						printf("****%s******\n",commands[index].info);
						commands[index].handler(atoi(args[1]));
						break;
					case 2:
						printf("\n\n*****La funcion a la que estoy entrando es:*****\n");
						printf("****%s******\n",commands[index].info);
						//si soy el comando 'inscribirse' o 'desinscribirse', retorno 0

						//caso contrario, soy el comando 'iniciar', en cuyo caso debo retornar algo positivo o negativo
						//de acuerdo a si se acepto el username y password

						if(!strcmp(args[0],"inscribirse") || !strcmp(args[0],"desinscribirse")){
							commands[index].handler(atoi(args[1]),atoi(args[2]));
							return 0;	
						}else{
							return (commands[index].handler(atoi(args[1]),args[2]));
							
													}
						
						break;
					case 3:
						printf("\n\n*****La funcion a la que estoy entrando es:*****\n");
						printf("****%s******\n",commands[index].info);
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

	return 0;

}

int split(char* args[], char* buffer){
	int i=0, j=1;  //m=0,
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


int help(){
	for(int i=0; i< TOTAL_COMMANDS; i++){

		if(commands[i].enabled == 1){
			printf("- %s: %s\n", commands[i].key, commands[i].info);	
		}
		
	}

	return 0;	
}

int iniciarSesion_client(int legacy, char *password){
	printf("\n\nENTRE A iniciarSesion_client\n");
	int result;
	printf("Iniciando sesion.\n");
	printf("EL USUARIO VALE: %d\n", legacy);
	printf("PASSWORD VALE: %s\n", password);
	return sesion(legacy,password);
	

}

int inscribirseMateria_client(int legacy, int subCode){
	printf("Intentando completar la solicitud.\n");
	char* result;

	result = inscribirseMateria(legacy, subCode);
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
	printf("Obteniendo materias correlativas.\n");


	char *response = calloc(1024,sizeof(char));

	response = correlatividades(subCode);
	printf("LO QUE TENGO EN RESPONSE ES:   \n%s\n",response);
	
	return 0;
}

int materias_client(void){
	printf("Obteniendo listado de materias...\n");

	char*response= materias();
	printf("%s\n",response);
	return 0;
}



char* inscribirseMateria(int legacy, int subCode){
	pack->function = SUBSCRIBE_SUBJECT;
	pack->subID= subCode;
	pack->studentID = legacy;
	pack->size = sizeof(Package);
	sendPackage(sockfd, pack);
	pack = receivePackage(sockfd, pack);
	return pack->response; 
}
char* desinscribirseMateria(int legacy, int subCode){
	pack->function = CANCEL_SUBSCRIPTION;
	pack->size = sizeof(Package);
	pack->studentID = legacy;
	pack->subID = subCode;
	sendPackage(sockfd, pack);
	pack = receivePackage(sockfd, pack);
	return pack->response; 
}
char* correlatividades(int subCode){
	pack->function = CORRELATIVES;
	pack->size = sizeof(Package);
	pack->subID = subCode;
	sendPackage(sockfd, pack);
	pack = receivePackage(sockfd, pack);
	return pack->response; 
}


char* materias(void){
	pack->function = SUBJECTS;
	pack->size= sizeof(Package);
	printf("\n\n######Estoy enviando un paquete con el siguiento pedido:############  \n");
	printf("########En este caso el paquete lo mando con VOID#######\n");
	sendPackage(sockfd, pack);
	

	printf("\n\n#####Obtuve la respuesta del servidor!!!!#####\n");
	pack = receivePackage(sockfd, pack);

	printf("\n\n#####La respuesta es:######\n");

	printf("####CHAN CHAN CHAN: %s######\n",pack->response);
	return pack->response;
}

int sesion(int user, char *pass){

	printf("\n\nINGRESE A SESION\n");
	printf("EL USUARIO VALE: %d\n", user);
	printf("PASSWORD VALE: %s\n", pass);
	//pack = (char *)calloc(sizeof(Package),1);
	pack->function = CHECK_USER;
	pack->size= sizeof(Package);
	strcpy(pack->pass,pass);
	//pack.pass = pass;
	pack->studentID  = user;
	
	printf("\n\n\n************LO QUE ALMACENE EN DATA COMO USER ES: %d**********\n", pack->studentID);
	printf("\n\n\n************LO QUE ALMACENE EN DATA COMO PASS ES: %s**********\n", pack->pass);
//	printf("ACA estan los datos %d \n %d \n", pack.data.sign.studentID,pack.data.pass );
//	printf("ESTOY EN SESION Y ESTOY ENVIANDO ESTA PASSWORD EN UN PAQUETE: %s\n", pass);
	sendPackage(sockfd, pack);
	pack = receivePackage(sockfd, pack);

	printf("Recibi datoss!! %s %d %s \n",pack->response,pack->studentID ,pack->pass);

	if(atoi(pack->response) == -1){
		return -1;
	}else{
		return 1;
	}
	//return checkUser(user,pass);
}