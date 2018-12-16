#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "csapp.h"
#include "peer.h"

#define ARGNUM 2 // the peer program takes a IP-adress 
                 // and a prot for which it shoud listingning to



int login(char* host, char* port, char* nick, char* password){
    
    // creating message to server, and responsebuffer
    char msg[7+strlen(nick)+strlen(password)], buf[MAXLINE];

    sprintf(msg, "/login %s %s", nick, password);
    
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
        return clientfd;
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
    if(clientfd != -1){
        char* msg = "close\n";
        // send closing message to server.
        while(rio_writen(clientfd, msg, 6) < 0){}
        
        // close file descriptor
        close(clientfd);
        fprintf(stdout, "You are now logged out.\n");
        return 0;
    }

    fprintf(stderr, "You are not logged in\n");
    return -1;
}

int lookup(int clientfd, char* input){
    
    char buf[MAXLINE];
    int checker;
    // init rio buffer reading
    rio_t rio;
    Rio_readinitb(&rio, clientfd);

    // send request
    while(rio_writen(clientfd, input, strlen(input)) < 0){}
    
    // read response
    while((checker = Rio_readlineb(&rio, buf, MAXLINE)) >= 0){
        fprintf(stdout, "%s", buf);
    }
    return 0;
}

// with capital E to prevent naming conflict
int Exit(int client){
    logout(client);
    exit(0);
}


int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int clientfd;
    
    // main rutine 
    while(1){
        // commandline command and arguments
        char buf[1024], command[8], args[500];
        // scanning input
        fscanf(stdin, "%s", buf);
        // filter input (at most 1 command and 4 arguments)
        sscanf(buf, "%s %s", command, args);

        if(strcmp("/exit", command)){
            // terminates the program.
            Exit(clientfd);
        }
        // send login request and set fd
        if(strcmp("/login", command)){
            char nick[100], password[1000], ip[15], port[5];
            
            // filtering arguments from args 
            sscanf(args,"%s %s %s %s", nick, password, ip, port);
            
            // try login to server
            clientfd = login(ip, port, nick, password);
        }

        // logout
        if(strcmp("/logout", command)){
            logout(clientfd);
        }

        // lookup
        if(strcmp("/lookup", command)){
            // if logged in
            if(clientfd != -1){
                // if no arguments are given
                if(strcmp("", args)){
                    lookup(clientfd, "lookup\n");
                
                // if arguments are given
                } else {
                    char request[6+strlen(args)];
                    
                    // formating protocol
                    sprintf(request, "lookup %s\n", args);
                    lookup(clientfd, request); 
                }
            }
        }
    }


    return 0;

}
