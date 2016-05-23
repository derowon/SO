#ifndef _PARSER_H
#define _PARSER_H

void initCommandList();
void processAction(char *buffer);
int commandSelector(char *command);
void split(char* buffer, char* command, char* arguments);
int strEquals(char* str1, char* str2);

int parser(char * buff, int sock);
int split2(char* args[], char* buffer);
int iniciarSesion_client(int legacy, char *password);

int help(void);
int salir(void);

int iniciarSesion_client(int legacy, char *password);
int sesion(int user, char *pass);

int inscribirseMateria_client(int legacy, int subCode);
int desinscribirseMateria_client(int legacy, int subCode);
int correlatividades_client(int subCode);
int materias_client(void);
int cupo_client(int subCode);
int misMaterias_client(int legacy);

int inscribirseMateria(int legacy, int subCode);
int desinscribirseMateria(int legacy, int subCode);
char* correlatividades(int subCode);
char* materias(void);
char* cupo(int subCode);
char* misMaterias(int legacy);

typedef int (*action)(char *);
typedef int (* func)();

void clear();
void limpiarBuffer(Package * pack);


#endif