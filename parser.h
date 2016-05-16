#ifndef _PARSER_H
#define _PARSER_H

void initCommandList();
int strEquals(char* str1, char* str2);
int parser(char * buff, int sock);
int split(char* args[], char* buffer);


int inscribirseMateria_client(int legacy, int subCode);
int desinscribirseMateria_client(int legacy, int subCode);
int correlatividades_client(int subCode);
int materias_client(void);
int help(void);
int salir(void);
int iniciarSesion_client(int legacy, char *password);

char* inscribirseMateria(int legacy, int subCode);
char* desinscribirseMateria(int legacy, int subCode);
char* correlatividades(int subCode);
char* materias(void);
int sesion(int user, char *pass);

typedef int (*action)(char *);
typedef int (* func)();


#endif