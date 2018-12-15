#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csapp.h"
#include "peer.h"

#define ARGNUM 2 // the peer program takes a IP-adress 
                 // and a prot for which it shoud listingning to



int login(char* host, char* port, char* nick, char* password){
    
    // creating message to server
    char *msg buf[MAXLINE];
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
    Rio_writeb(clientfd, msg, strlen(msg));

    // keeps reading response line undtil it comes
    while((rsplen = Rio_readlineb(&rio, buff, MAXLINE))<=0){}
    
    switch(rsplen){
        case (strlen("true")):
            fprintf(stdout, "You are now logged in.\n");
            return clientfd;

        case (strlen("false")):
            fprintf(stderr, "Wrong user and/or password.\n")
            return -1; // eligale fd (file discriptor) 
    }
    

    // return file descriptor for the server (read/write)
    return clientfd;
}

int logout(){
    
    return 0;
}

int lookup(char* input){
    
    return 0;
}

int exit(){
    return 0;
}


int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int checkpoint

}
