#include <stdio.h>
#include "csapp.h"
#include "peer.h"
#include "buffer.h"

#define ARGNUM 2 // TODO: Put the number of arguments you want the
                 // program to take




int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int clientfd, responsefd, mark;
    char *host, *port,buf_stdin[MAXLINE], buf_in[MAXLINE], buf_out[MAXLINE];
    char command[MAXLINE], args[MAXLINE];
    rio_t rio_in, rio_out;




    host = argv[1];
    port = argv[2];

    // open connection to server
    clientfd = Open_clientfd(host, port);
    printf("connected\n"); // should be deleted when done
    Rio_readinitb(&rio, clientfd);
    
    while(Fgets(buf_stdin, MAXLINE, stdin) != 0){
        sscanf(buf_stdin, "%s %s", command, args); // splitting input.
        // mark = 0 for logged off, testing for login status.
        if(mark != 1 && !strcmp(command,"/login")){
            fprintf(stderr, "You are not logged in\n");
            continue;
        }

        if(strcmp(command, "/login")){
            if(mark != 0){
                fprintf(stderr, "you are already logged in\n");
                continue; // logged in
            }

            // creating message and sending it to server.
            sprintf(buf_out,"LOGIN %s", args);
            Rio_writen(clientfd, buf_out, strlen(buf_out));
            
        }





        
    }





    //TODO: Implement
    return 0;
}
