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

    int clientfd, listenfd, status;
    char user[MAXLINE/2]; // this peers nickname
    char buf[MAXLINE];
    char command[MAXLINE], args[MAXLINE]; // placeholders for command and arguments from stdin
    char nick[MAXLINE/4], pass[MAXLINE/4], host[MAXLINE/4], port[MAXLINE/4], path[MAXLINE];

    // directories for Log and Messages
    pid_t msg_handler;
    // opening a connection to the server.
    if((clientfd = Open_clientfd(argv[1], argv[2])) < 0){
        fprintf(stderr, "Connection error\nUsage: ./peer host port\n");
        exit(1);
    }

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
            Send(clientfd, pass);

            // test respons from server
            readline(clientfd, buf); // from peer_help.h
            if(strcmp(buf, "PASS OK\n") != 0){
                fprintf(stderr, "Login error\n");
                continue; // reset to login
            }
            // login has happened finalize with sending ip and port.
            sprintf(path, "%s/Messages", nick);
            mkdir(path, S_IRWXO | S_IRWXU); // make message folder.

            // setting user name for msg sending and closing peer.
            sprintf(user, "%s", nick);
            // child process for message processing
            // only use one since it takes minimal of time to write a message to file.
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
                    // getting protocol command
                    sscanf(buf, "%s: From: %s\n", command, nick);

                    if(strcmp(command, "MSG") == 0){
                        // setting nick to user name
                        
        
                        store_message(connfd, user, nick);
                        
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

                        // split nick and msg into nick an buf.
                        sscanf(args, "%s %s", nick, buf);

                        send_message(clientfd, nick, buf);
                        continue; // jump to next inpt.
                    }
                    
                    if(strcmp(command, "/show") == 0){
                        // get message case args hold nick if it is given

                        // user specefic folder, args stores the nick if any,
                        // and dumping output into terminal.
                        get_messages(user, args, STDIN_FILENO);
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
                        
                        reap_messages(nick); // delete messages
                        break; // jump to login loop
                    }
                    



                    // exit part
                    if(strcmp(command, "/exit\n") == 0){
                        
                        sprintf(buf, "LOGOUT: %s", user);
                        
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
                        reap_messages(nick);

                        exit(0); // termination the program
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
