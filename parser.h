#ifndef _PARSER_H
#define _PARSER_H

void initCommandList();
void processAction(char *buffer);
int commandSelector(char *command);
void split(char* buffer, char* command, char* arguments);
int strEquals(char* str1, char* str2);

void parser(char * buff, int sock);
int split2(char* args[], char* buffer);






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


#endif