#ifndef _PACKAGES_H
#define _PACKAGES_H
#define MAX_STR_SIZE 	4096
#define MAX_COR			8
#define MAX_B 4368

typedef enum { SUBJECTS, CORRELATIVES, SUBSCRIBE_SUBJECT, CANCEL_SUBSCRIPTION,CHECK_USER} functions;

typedef struct {
	int conn;
}Address;




typedef struct{
	int size;
	int function;
	int studentID;
	int subID;
	char pass[256];
	char response[4096];

} Package;



#endif