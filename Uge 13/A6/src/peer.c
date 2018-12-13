#include <stdio.h>
#include "peer.h"

#define ARGNUM 0 // TODO: Put the number of arguments you want the
                 // program to take

const char server_commands[] = "Server commands: \nlogin \nlookup <nick> \nlookup \nlogout \nexit\n";




int main(int argc, char**argv) {

    int clientfd;
    char *ip, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        printf(stderr, "Please insert your informations: <IP> <Port>\n", argv[0]);
        exit(0);
    }
    //TODO: Implement
    ip       = argv[1];
    port     = argv[2];

    clientfd = Open_clientfd(ip, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL){
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);



        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);

    }
    Close(clientfd);
    exit(0);
}
