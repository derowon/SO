#ifndef _COMM_H
#define _COMM_H


int sendPackage(int con, Package* pack);
int receivePackage(int con, Package* pack);
int iConnect_client(void );
int iConnect_server(void);
// void handleRequest(int connfd);
int serverReceivePackage(int sender, Package* pack);
void handleRequest(int address, Package * pack);


void respond(Package * data);
void materias_db(Package * data);
#endif
