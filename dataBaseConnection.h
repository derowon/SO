#ifndef DATABASECONN_H
#define DATABASECONN_H

char * subscribeSubject(int subjectCode, int studentCode);
char * cancelSubscription(int subjectCode, int studentCode);
char * getSubjectsByCareer(char* careerName);
char * getSubjects();
char * seeCorrelatives(int subjectCode);
char * getApprovedSubjectsByStudentCode(int studentCode);
char * getSubjectsByStudentCode(int studentCode);
char* getCupo(int subjectCode);
char * checkUser(int username, char* password);
int checkPassword(int legajo, char* password);
void tableCreate();


#endif