#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


int main( int argc, char *argv[] ) {
   int sockfd, newsockfd;
   char buffer[256];
   int n, pid;
   
   /* First call to socket() function */
   sockfd = initSockets(); 
   prepareToListen(sockfd);
 
   while (1) {
		newsockfd = acceptConnection(sockfd);
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      
      /* Create child process */
      pid = fork();
		
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the client process */
         close(sockfd);
         handleRequest(newsockfd,NULL);
         exit(0);
      }
      else {
         close(newsockfd);
      }
		
   } /* end of while */
}




