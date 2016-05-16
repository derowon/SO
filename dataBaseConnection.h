#ifndef DATABASECONN_H
#define DATABASECONN_H

int subscribeSubject(int subjectCode, int studentCode);
int cancelSubscription(int subjectCode, int studentCode);
char * getSubjectsByCareer(char* careerName);
char * getSubjects();
char * seeCorrelatives(int subjectCode);
char * getApprovedSubjectsByStudentCode(int studentCode);
char * getSubjectsByStudentCode(int studentCode);
char * checkUser(int username, char* password);
int checkPassword(char* password);

#endif