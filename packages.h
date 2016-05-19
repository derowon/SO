#ifndef _PACKAGES_H
#define _PACKAGES_H

#define MAX_STR_SIZE 	4096
#define MAX_COR			8
#define MAX_B  4368
typedef enum { MATERIAS, CORRELATIVAS, INSCRIBIR_MATERIA, DESINSCRIBIR_MATERIA,CHECK_USER} functions;

typedef struct {
	int conn;
}Address;

typedef struct{
	int id;
	int cor;
}addCorStruct;

typedef struct{
	int studentID;
	int subID;
}signUpStruct;

typedef struct {
	char response[4096];
	addCorStruct cor;
	signUpStruct sign;
	int subC;
}dataUnion;

typedef struct{
	int size;
	char pass[256];
	int function;
	int clientid;
	dataUnion data;
} Package;


#endif
