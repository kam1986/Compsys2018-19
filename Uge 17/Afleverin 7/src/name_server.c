#include <stdio.h>
#include <string.h>
#include "name_server.h"

#define ARGNUM 1 // TODO: Put the number of arguments you want the
                 // program to take

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int status, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; // Enough space for any address
    char client_hostname[MAXLINE], client_port[MAXLINE];

    // buffers
    char buf_in[MAXLINE], buf_out[MAXLINE];

    pid_t childpid;

    // initiate socket
    listenfd = Open_listenfd(argv[1]);

    while(1){

        if(connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen) < 0){
            close(connfd); // close socket.
            fprintf(stderr, "[-] Acceptence error\n");
            exit(1);
        }        
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        
        fprintf(stdout,"Connection accepted from (%s, %s)\n", client_hostname, client_port);

        // making new process on server for each client. 
        if((childpid = Fork()) == 0){
            rio_t rio;

            while(1){
                // TODO - get stuff from client. 

                // TODO - respond to client. NOT DONE.
                
                // response to logout and exit
                if(strcmp("LOGOUT", buf_in) == 0){
                    fprintf(stdout,"Disconnection accepted from (%s, %s)\n", client_hostname, client_port);
                    break;
                }

                // send response to client.
            }
            exit(0);
        }
        

        // TODO - should be changed to RIO function.
        Fgets(buf_in, MAXLINE, stdin);
        // closing server, waiting for all clients to disconnect.
        if(strcmp(buf_in,"Close") == 0){
            // waiting for all processes to die.
            wait(&status);
            break;
        }
    }

    //TODO: Implement
    exit(0);
}
