#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#include "peer_help.h"

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

    int clientfd, listenfd, status, mark;
    char user[MAXLINE]; // this peers nickname
    char buf_out[MAXLINE];
    char command[MAXLINE], args[MAXLINE]; // placeholders for command and arguments from stdin
    char nick[MAXLINE], pass[MAXLINE], host[MAXLINE], port[MAXLINE], path[MAXLINE];

    // directories for Log and Messages
    struct dirent *log, *msg;
    pid_t msg_handler;
    // opening a connection to the server.
    clientfd = Open_clientfd(argv[1], argv[2]);
    Rio_readinitb(&rio_out, clientfd);

    // set selecet set
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(clientfd, &read_set);
    Signal(SIGCHLD, sigchald_handler); // reap zombies.

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
                // should block here if either logged in or wrong nick.
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
            // login has happened finalize with sending ip and port.
            sprintf(path, "%s/Messages", nick);
            mkdir(path); // make message folder.

            // setting user name for msg sending and closing peer.
            sprintf(user, "NICK: %s IP: %s Port: %s", nick, ip, port);
            
            // child process for message processing
            // only use one since it takes minimal of time to write a message to file.
            if((msg_handler = Fork()) == 0){
                // closing server connection - no need for it -
                Close(clientfd);
                
                int connfd;
                socketlen_t clientlen;
                struct sockaddr_storage peeraddr;
                char client_hostname[MAXLINE], client_hostport[MAXLINE];
            
                // opening listening socket for messages.
                Signal(SIGCHLD, sigchald_handler);
                listenfd = Open_listenfd(port);

                while(1){
                    connfd = Accept(listenfd, (SA *) &clie);
                    readline(connfd, buf);                   
                    // getting protocol command
                    sscanf(buf, "%s: From: %s\n", command, nick);

                    if(strcmp(command, "MSG") == 0){
                        // setting nick to user name
                        sscanf(user, "NICK: %s %s", path, args);
        
                        store_message(connfd, path, nick);
                        
                        Close(connfd);
                        continue;
                    }
                    
                    Close(connfd);
                }                

            }

            while(1){
                    // parent process
                    // TODO

                    if(strcmp(command, "/msg") == 0){
                        // send message case args hold nick if nick.
                    }
                    
                    if(strcmp(command, "/show") == 0){
                        // get message case args hold nick if it is given
                    }

                    if(strcmp(command, "/logout") == 0){
                        sscanf(user, "NICK: %s %s", nick, args);
                        
                        sprintf(buf, "LOGOUT: %s", nick);
                        
                        while(1){
                            // sending logout request.
                            if(Send(clientfd, buf) < 0){
                                break; // connection issues
                            }
                            // waiting for the server to respond
                            // OBS server sends request for closing the other process
                            readline(clientfd, buf);
                            if(strcmp(buf, "LOGGED OUT\n")){
                                break; // trying to log out until it successed
                            }
                        }
                        Close(clientfd);
                        
                        kill(msg_handler, SIGKILL); // kill message handler process
                        // waiting for child process to terminate.
                        
                        reap_message(nick); // delete messages
                        break; // jump to login loop
                    }
                    



                    // exit part
                    if(strcmp(command, "/exit\n") == 0){
                        sscanf(user, "NICK: %s %s", nick, args);
                        
                        sprintf(buf, "LOGOUT: %s", nick);
                        
                        while(1){
                            // sending logout request.
                            if(Send(clientfd, buf) < 0){
                                break; // connection issues
                            }
                            // waiting for the server to respond
                            // OBS server sends request for closing the other process
                            readline(clientfd, buf);
                            if(strcmp(buf, "LOGGED OUT\n")){
                                break; // trying to log out until it successed
                            }
                        }
                        Close(clientfd);
                        
                        kill(msg_handler, SIGKILL); // kill message handler process
                        // waiting for child process to terminate.
                        while(waitpid(-1, &status, 0)>0){}
                        
                        // deleting messages
                        reap_message(nick);
                        exit(0);
                    }
                }
        }
        // exit before login nothing to do but terminate process.
        if(strcmp(buf, "/exit\n") == 0){
            exit(0); 
        }
    }

    Close(clientfd);
    exit(0);
}
