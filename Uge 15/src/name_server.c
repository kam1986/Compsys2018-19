#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "csapp.h"
#include "help.h"
#include "name_server.h"

#define ARGNUM 1 // port number

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    // open listening connection on port given at startup
    int connfd, listenfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE], buf[MAXLINE], cmd[MAXLINE/4], args[MAXLINE/2];
    
    listenfd = Open_listenfd(argv[1]);

    // main procedure
    while(1){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        if(Fork() == 0){
            Close(listenfd);
            
            Getnameinfo((SA *) &clientaddr, clientlen, 
                client_hostname, MAXLINE, client_port, MAXLINE, 0);
            
            // peer procedure
            while(1){

                // cmd will still be set to EXIT after exiting the 'logged in loop'
                if(strcmp(cmd, "EXIT") == 0){
                    fprintf(stdout,"Goodbye to %s\n", client_hostname);
                    Close(connfd);
                    exit(0);
                }

                // read next request
                Readline(connfd, buf);

                // splitting buf into command and arguments 
                sscanf(buf, "%s %[^\n]", cmd, args);
                // checking for login request
                if(strcmp(cmd, "LOGIN") != 0){
                    Send(connfd, S_LOGIN_ERROR);
                    continue; // jump to the start of the loop
                }

                // try to login
                if(Login(connfd, args) != 0){
                    Send(connfd, S_LOGIN_ERROR);
                    continue; // jump to the start of the loop
                }

                // send 
                Send(connfd,"GOOD TO GO!");
                // logged in loop
                while(1){
                    // connection error
                    if(Readline(connfd, buf) < 0){
                        // error for incorrect closing of socket on peer side
                        fprintf(stderr, "Connection error for %s\n", client_hostname);
                        exit(0);
                    }

                    // splitting buf into command and arguments 
                    sscanf(buf, "%s %[^\n]", cmd, args);

                    if(strcmp(cmd, "LOGOUT\n") == 0){
                        Logout(connfd,args);
                        break;
                    }

                    if(strcmp(cmd, "LOOKUP\n") == 0){
                        Lookup(connfd, args);
                    }

                }
            }

        } else {

            /*

                main process interaktion with the servers terminal

            */

            Close(connfd);
            // read commandline input from serverside
            while(1){
            
                Readline(0, buf);
                // closing server
                if(strcmp(buf, "/exit") == 0){
                    // send SIGKILL to all its' children processes
                    // and terminates all child processes
                    Kill(-1, SIGKILL);
                    // wait for all children to terminate
                    while(waitpid(-1, NULL, 0) > 0){
                        // waiting for all children to close all connections
                    }
                    exit(0);
                    /*
                    
                    OBS: The peer check for closed sockets so a forced shut down
                         will bi spotted by any peer
                    
                    Not sure to use waitpid or not after Kill?

                    */
                }
            }   
        }
    }
}
