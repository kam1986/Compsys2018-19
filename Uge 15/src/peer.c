#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peer.h"
#include "help.h"

#define ARGNUM 2 // take a server ip and port as arguments


int main(int argc, char** argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return 0;
    }

    int logged_in, clientfd;

    // buffer, peername, command and arguments placeholder
    char buf[MAXLINE], username[MAXLINE/2], cmd[8], args[MAXLINE/2];

    if((clientfd = Open_clientfd(argv[1], argv[2])) < 0){
        fprintf(stderr, "No Server are found at (%s,%s)", argv[1], argv[2]);
        return 0;
    }

    while(1){
         
        if(strcmp(buf, "/exit") == 0){
            // Terminating the peer program
            return 0;
        }

        // read from stdin
        Readline(0, buf);
        sscanf(buf, "%s %[^\n]", cmd, args);

        // command error
        if(strcmp(cmd, "/login") != 0){
            fprintf(stdout, "You need to login first..!\nUsage: /login <nick> <password> <host> <port>\n");
            continue;
        }
        
        // usage error
        if(login(clientfd, args) != 0){
            fprintf(stderr, "Usage: /login <nick> <password> <host> <port>\n");
            continue;
        }

        // setting username
        sscanf(buf, "/login %s %[^\n]", username, args);

        while(logged_in){
            Readline(0, buf);
            sscanf(buf, "%s %[^\n]", cmd, args);
            
            if(strcmp(cmd, "/logout") == 0){
                // loggin out
                if(logout(clientfd, username) != 0){
                    continue;
                }

                Readline(clientfd, buf);
                fprintf(stdout, "%s", buf);
                logged_in--;
                continue;
            }

            if(strcmp(cmd, "/exit") == 0){
                // exiting but the peer log out first
                if(logout(clientfd, username) != 0){
                    continue;
                }

                Readline(clientfd, buf);
                fprintf(stdout, "%s", buf);
                logged_in--;
                continue;
            }

            if(strcmp(cmd, "/lookup") == 0){
                // send lookup request to server and dump responce to stdout.
                lookup(clientfd, 1, args);
                continue;
            }

        }
    }


    return 0;
}
