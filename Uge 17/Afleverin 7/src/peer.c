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

    int clientfd, listenfd, mark;
    char user[MAXLINE]; // this peers nickname
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
    FD_SET(clientfd, &read_set);
    Signal(SIGCHLD, sigchald_handler); // reap zombies.

    // TODO make when login
    mkdir("Messages", NULL);

    // waiting for login
    while(1){
        Fgets(buf, MAXLINE, stdin);
        sscanf(buf, "%s %s", command, args);
        if(strcmp(command, "/login") == 0){
            sscanf(args, "%s %s %s %s", nick, pass, host, port);
            // initiate login
            sprintf(buf, "LOGIN: %s", nick);
            Send(clientfd, buf);


            // test respons from server
            readline(clientfd, buf); // from peer_help.h
            if(strcmp(buf, "Nick OK\n") != 0){
                fprintf(stderr, "Login error\n");
                continue; // reset to login
            }

            sprintf(buf, "PASS: %s", pass);
            Send(clientfd, PASS);

            // test respons from server
            readline(clientfd, buf); // from peer_help.h
            if(strcmp(buf, "PASS OK\n") != 0){
                fprintf(stderr, "Login error\n");
                continue; // reset to login
            }

            sprintf(buf, "IP: %s\nPort: %s");
            Send(clientfd, buf):
            
        }
        
    }

    Close(clientfd);
    exit(0);
}
