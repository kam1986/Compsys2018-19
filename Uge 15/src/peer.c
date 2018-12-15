#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "csapp.h"
#include "peer.h"

#define ARGNUM 2 // the peer program takes a IP-adress 
                 // and a prot for which it shoud listingning to



int login(char* host, char* port, char* nick, char* password){
    
    // creating message to server, and responsebuffer
    char msg[7+sio_strlen(nick)+sio_strlen(password)], buf[MAXLINE];

    sprintf(msg, "login %s %s", nick, password);
    
    // open connection to server
    // open_clientfd from csapp.h
    int rsplen, clientfd = open_clientfd(host, port);
    
    // buffer
    rio_t rio;
    // init buffer
    Rio_readinitb(&rio, clientfd);

    // check for connection error
    if(clientfd == -1){
        fprintf(stderr,"Could not find server. Try another host\n");
        exit(1);
    }

    // sending login request to server
    Rio_writen(clientfd, msg, strlen(msg));

    // keeps reading response line undtil it comes
    while((rsplen = Rio_readlineb(&rio, buf, MAXLINE)) <= 0){}
    
    switch(rsplen){
        case 4:
            fprintf(stdout, "You are now logged in.\n");
            // return socket (file descriptor)
            return clientfd;

        default:
            fprintf(stderr, "Wrong user and/or password.\n");
            return -1; // eligale fd (file descriptor) 
    }
}

int logout(int clientfd){
    // send closing message to server.
    while(Rio_writen(clientfd, "close\n", 6) < 0){}
    
    // close file descriptor
    close(clientfd);
    fprintf(stdout, "You are now logged out.\n");
    return 0;
}

int lookup(int* clientfd, char* input){
    
    char buf[MAXLINE];
    int checker;
    // init rio buffer reading
    rio_t rio;
    Rio_readinitb(&rio, clientfd);

    // send request
    while(Rio_writen(clientfd, input, sio_strlen(input) < 0){}
    
    // read response
    while((checker = Rio_readlineb(rio, buf, MAXLINE)) >= 0){
        fprintf(stdout, "%s", buf);
    }
    return 0;
}

int exit(int client){
    logout();
    exit(0);
}


int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    return 0;

}
