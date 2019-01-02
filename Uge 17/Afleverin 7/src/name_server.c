#include <stdio.h>
#include <string.h>
#include "csapp.h"
#include "name_server.h"

#define ARGNUM 1 // TODO: Put the number of arguments you want the
                 // program to take

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int status, connfd, listenfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; // Enough space for any address
    char client_hostname[MAXLINE], client_port[MAXLINE];

    // buffers
    char buf[MAXLINE];

    pid_t childpid;

    // initiate socket
    listenfd = Open_listenfd(argv[1]);

    while(1){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        
        fprintf(stdout,"Connection accepted from (%s, %s)\n", client_hostname, client_port);

        // making new process on server for each client. 
        if((childpid = Fork()) == 0){

            rio_t rio;
            Rio_readinitb(&rio, connfd);

            while(1){
                // TODO - get stuff from client. 
                Rio_readlineb(&rio, buf, MAXLINE);

                // TODO - respond to client. NOT DONE.
                Rio_writen(connfd, "got message.\n", 13);
                // response to logout and exit
                

                // TODO - send response to client.

            }

            // terminate process.
            close(connfd);
            exit(0);
        }
        

        // TODO - should be changed to RIO function.
        Fgets(buf, MAXLINE, stdin);
        // closing server, waiting for all clients to disconnect.
        if(strcmp(buf,"Close") == 0){
            // waiting for all processes to die.
            while(waitpid(-1, &status, 0) > 0){
                int ret;
                if((ret = WIFEXITED(status)) < 0) {
                    printf("child %d terminated normally with exit status=%d\n", childpid, ret);
                }
                printf("child %d terminated abnormally\n", childpid);
            }
            break;
        }
    }

    //TODO: Implement
    exit(0);
}
