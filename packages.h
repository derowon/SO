#ifndef _PACKAGES_H
#define _PACKAGES_H
#define MAX_STR_SIZE 	4096
#define MAX_COR			8

typedef enum { SUBJECTS, CORRELATIVES, SUBSCRIBE_SUBJECT, CANCEL_SUBSCRIPTION} functions;

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

typedef union {
	char response[4096];
	addCorStruct cor;
	signUpStruct sign;
	int subC;
}dataUnion;

typedef struct{
	int size;
	int function;
	dataUnion data;
} Package;



#endif