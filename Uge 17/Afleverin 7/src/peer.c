#include <stdio.h>
#include <unistd.h>

#include "csapp.h"
#include "peer.h"
#include "peer_help.h"


#define ARGNUM 2 // TODO: Put the number of arguments you want the
                 // program to take

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int clientfd, listenfd;
    char user[MAXLINE/2]; // this peers nickname
    char buf[MAXLINE];
    char command[MAXLINE], args[MAXLINE]; // placeholders for command and arguments from stdin
    char nick[MAXLINE/4], port[MAXLINE/4];

    // directories for Log and Messages
    pid_t msg_handler;
    // opening a connection to the server.
    if((clientfd = Open_clientfd(argv[1], argv[2])) < 0){
        fprintf(stderr, "Connection error\nUsage: ./peer host port\n");
        exit(1);
    }

    // waiting for login
    while(1){
        // reset buffers to prevent leaking of old data.
        command[0] = '\0';
        args[0] = '\0';
        nick[MAXLINE/4] = '\0';
        port[MAXLINE/4] = '\0';

        Fgets(buf, MAXLINE, stdin);

        // split command and arguments into two 
        // %[^\n] takes all char's after the first whitespace until but not including \n
        sscanf(buf, "%s %[^\n]", command, args);
        if(strcmp(command, "/login") == 0){
            
            if(login(clientfd, args, user) < 0){
                continue; // login error try again.
            }
            
            if((msg_handler = Fork()) == 0){
                // closing server connection - no need for it -
                Close(clientfd);
                
                int connfd;
                socklen_t clientlen;
                char client_hostname[MAXLINE];
            
                // opening listening socket for messages.
                listenfd = Open_listenfd(port);

                while(1){
                    // don't fork because the transaction time are limited.
                    connfd = Accept(listenfd, (SA *) &client_hostname, &clientlen);
                    readline(connfd, buf);

                    sscanf(buf, "%s: From: %[^\n]", command, nick);

                    if(strcmp(command, "MSG:") == 0){ // anything else gets ignoret.
                        
                        store_message(connfd, user, nick);
                        
                        Close(connfd); // close connection after sending message
                        continue;
                    }

                    
                    Close(connfd);
                }                

            }else{

                while(1){
                    // parent process
                   
                    
                    // reset buffers to prevent leaking of old data.
                    command[0] = '\0';
                    args[0] = '\0';
                    nick[MAXLINE/4] = '\0';

                    
                    Fgets(buf, MAXLINE, stdin);

                    sscanf(buf, "%s %[^\n]", command, args);
                    
                    if(strcmp(command, "/login") == 0){
                        fprintf(stderr, "You are already logged in\n");
                        continue;
                    }

                    if(strcmp(command, "/logout") == 0){
                        
                        logout(clientfd, user, msg_handler);
                        exit(0); // jump to login loop
                    }  

                    if(strcmp(command, "/lookup") == 0){
                        if(strcmp(args, "") == 0 ){
                            lookup(clientfd, NULL);
                            continue;
                        }
                        lookup(clientfd, args);
                        continue;
                    }

                    if(strcmp(command, "/msg") == 0){
   
                    // split command and arguments into two nick' and 'msg' into nick an buf.
                        sscanf(args, "%s %[^\n]", nick, buf);

                        send_message(clientfd, nick, buf);
                        continue; // jump to next inpt.
                    }
                    
                    if(strcmp(command, "/show") == 0){
                        // get message case args hold nick if it is given

                        // user specefic folder, args stores the nick if any,
                        // and dumping output into terminal.
                        get_messages(user, args, STDIN_FILENO);
                    }

                    // exit part
                    if(strcmp(command, "/exit\n") == 0){
                        
                        logout(clientfd, user, msg_handler);                      

                        exit(0); // termination the program
                    }

                }
            }
        }
        // exit before login nothing to do but terminate process.
        if(strcmp(buf, "/exit\n") == 0){
            Close(clientfd);
            exit(0);
        }
    
    }

}
