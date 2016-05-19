#ifndef _COMM_H
#define _COMM_H


int sendPackage(int con, Package* pack);
int receivePackage(int con, Package* pack);
int iConnect_client(void );
int iConnect_server(void);
int acceptConnection(int fd);
int serverReceivePackage(int sender, Package* pack);
void handleRequest(int newconn,int server, Package * pack,char*name);


void respond(Package * data);
void materias_db(Package * data);
#endif
