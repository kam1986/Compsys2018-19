#include <stdio.h>
#include "csapp.h"
#include "peer.h"
#include "buffer.h"

#define ARGNUM 2 // TODO: Put the number of arguments you want the
                 // program to take


void sigchldhandler(int sig){
    while(waitpid(-1, 0, WHOHANG) > 0){}
    return;
}

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int clientfd, mark;

    char buf_out[MAXLINE];
    char command[MAXLINE], args[MAXLINE]; // placeholders for command and arguments from stdin
    rio_t rio_out;

    // something bla bla bla
    fd_set read_set, ready_set; // selecet set specifiers -*- need better explainations -*-
    char nick[MAXLINE], pass[MAXLINE], host[MAXLINE], port[MAXLINE];
    int listenfd, peerfd;
    socket_t peerlen;
    struct sockaddr_storage peeraddr;

    // directories for Log and Messages
    struct dirent *log, *msg;

    // opening a connection to the server.
    clientfd = Open_clientfd(argv[1], argv[2]);
    Rio_readinitb(&rio_out, clientfd);

    // set selecet set
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    Signal(SIGCHLD, sigchald_handler); // reap zombies.


    mkdir("Log", NULL);
    mkdir("Messages", NULL);

    // waiting for login
    while(1){
        fgets(buf, MAXLINE, stdin);
        sscanf(buf, "%s %s", command, args);
        
        if(strcmp(command, "/login") == 0){
            sscanf(args, "%s %s %s %s", nick, pass, host, port);
            
            // check for input error
            if(strcmp(nick, "") == 0 || strcmp(pass, "") == 0 || strcmp(host, "") == 0 || strcmp(port, "") == 0){
                fprintf(stderr, "usage: /login nick password host port.\n");
                continue;
            }
            
            // TODO - make directories Log and Messages


            listenfd = Open_listenfd(port);
            FD_SET(listenfd, &read_set);
            
            while(1) {
                ready_set = read_set;
                select(listenfd+1, &ready_set, NULL, NULL, NULL);
                
                if(FD_ISSET(STDIN_FILENO, &ready_set)){
                    Fgets(buf, MAXLINE, stdin)
                    // seperate command and arguments
                    sscanf(buf, "%s %s", command, args);
                    
                    // do command.
                }

                if(FD_ISSET(listenfd, &ready_set)){
                    peerlen = sizeof(struct sockaddr_storage);
                    peerfd = Accept(listenfd, (SA *)&peeraddr, &peerlen);


                    // make this modular!! e.i. turn it into a function.    
                    // Make child process to handle incomming messages from server/peers.
                    if(FORK() == 0){
                        Close(listenfd); // should not make new childs.
                        Close(clientfd); // should not send messages to server.
                        while(1){
                            // get messages from peers/server
                            Rio_readlineb()
                            


                            // --------------------

                            // termination connection
                            if(strcmp(buf,"Close") == 0){
                              Close(peerfd);
                              exit(0);
                            }
                        }
                    } 


                }
            }
            
        }
        
        // terminate the peer program.
        if(strcmp(command,exit) == 0){
            // TODO close all fd's
            exit(0);
        }

        fprintf(stderr, "usage: /login nick password ip port.\n");
    }

    Close(clientfd);
    exit(0);
}
