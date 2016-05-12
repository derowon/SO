#ifndef _COMM_H
#define _COMM_H
int clientSendPackage(int con, Package* pack);
void clientReceivePackage(int con, Package* pack);

#endif