#ifndef _SCK
#define _SCK

#include "packages.h"


int initSockets();
void prepareToListen(int conn);
int  acceptConnection(int sock);
void prepareToConnect(char * address);
void connectToServ(int sock);
void closeAcceptFD(int sock);
int sendPackage(int con, Package* pack);
Package * receivePackage(int con,Package* pack);

#endif