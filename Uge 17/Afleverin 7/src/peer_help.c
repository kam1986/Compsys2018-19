#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library
#include <sys/types.h>  // system type library
#include <dirent.h>     // standard directory library

#include "csapp.h"      // helper functions from the book
#include "peer_help.h"

// A6

int Send(int socket, char *buf){
    if(!socket){
        fprintf(stderr, "NULL sucket.\n");
        return -1;
    }
    
    char ibuf[MAXLINE];
    // adding newline 
    sprintf(ibuf, "%s\n", buf); 
    
    
    // sending content of buf through socket.
    return rio_writen(socket, ibuf, strlen(ibuf));
}


// socket could be any file discriptor types.
int readline(int socket, char *buf){
    rio_t rio;
    Rio_readinitb(&rio, socket);
    int ret = rio_readlineb(&rio, buf, MAXLINE);


    return ret;
}

int login(int socket, char *args, void *user){
    char buf[MAXLINE], 
         nick[MAXLINE/4], 
         pass[MAXLINE/4], 
         ip[MAXLINE/4], 
         port[MAXLINE/4],
         path[MAXLINE];

    // testing for number of input
    if(sscanf(args, "%s %s %s %s\n", nick, pass, ip, port) < 4){ 
        fprintf(stderr, "Usage: /login nick passwaord ip port\n");
        return -1;
    }

    sprintf(buf, "LOGIN: %s", nick);
    Send(socket, buf);

    // test respons from server
    readline(socket, buf); // from peer_help.h
    printf("%s", buf);
    if(strcmp(buf, "NICK: OK\n") != 0){
        // should block here if either logged in or wrong nick.
        fprintf(stderr, "Login error\n");
        return -1; // reset to login
    }

    sprintf(buf, "PASS: %s", pass);
    Send(socket, pass);

    // test respons from server
    readline(socket, buf); // from peer_help.h
    printf("%s", buf);
    if(strcmp(buf, "PASS: OK\n") != 0){
        fprintf(stderr, "Login error\n");
        return -1; // reset to login
    }
    
    // login has happened finalize with sending ip and port.
    sprintf(path, "%s/Messages", nick);
    mkdir(nick, 0777);
    mkdir(path, 0777); // make message folder.
    strcpy(user,nick);


    return 0;
}


int logout(int socket, void *user, pid_t msg_handler){
    char buf[MAXLINE];
    int status; 

    sprintf(buf, "LOGOUT: %s", (char*)user);
    
    
    Send(socket, buf);
        
    // waiting for the server to respond
    // OBS server sends request for closing the other process
    readline(socket, buf);    

    
    Close(socket);
    
    kill(msg_handler, SIGKILL); // kill message handler process
    // waiting for child process to terminate.
    printf("%s\n",(char*)user);
    reap_messages(user); // delete messages

    while(waitpid(-1, &status, 0) > 0);

    return status;
}



int lookup(int socket, char* args){

    char buf[MAXLINE];

    sprintf(buf, "LOOKUP: %s", args);
    Send(socket, buf);

    readline(socket, buf);
    if(strcmp(buf, "Peer not found\n") == 0){
        fprintf(stderr, "%s", buf);
        return -1;
    }

    // print undtil end of file.
    readline(socket, buf);


    return 0;
}


int EXIT(int socket, char *user, pid_t msg_handler){
    logout(socket, user, msg_handler);

    exit(0);
}


// A7 part.

// Send msg to nick, msg must be shorter than MAXLINE.
int send_message(int socket, char *nick, char* msg){
    
    int connfd;
    char buf[MAXLINE], peer[MAXLINE], ip[MAXLINE], port[MAXLINE];

    // making protocol request
    sprintf(buf, "LOOKUP: %s", nick);
    
    // sending request for peer infotest
    Send(socket, buf);

    // get response
    readline(socket, buf);
    
    // check for not online/non user
    if(strcmp(buf, "NOT FOUND\n") == 0){
        fprintf(stderr, "Peer not found\n");
        // OBS - could make the server safe them for the peer.
        return -1;
    }

    // filter response
    if(sscanf(buf, "%s is online. IP: %s Port: %s\n", peer, ip, port) < 3){
        fprintf(stderr, "Protocol format error\n");
        return -1;
    }

    if((connfd = Open_clientfd(ip, port)) < 0){
        fprintf(stderr, "connection error\n");
        return -2;
    }
    
    // build message protocol 
    sprintf(buf, "MSG: From: %s\nMessage: %s\n", nick, msg);
    Send(connfd, buf); // send message.

    return 0;
}



// get message, formate it and save it in file
// peername.msg at the end of file.
int store_message(int socket, char* folder, char *nick){
    // Message format
    // From nick \n
    // msg\n
    // \n e.i. message end with empty line (double linefeed)
    
    int msgfd;
    char buf[MAXLINE], path[MAXLINE];

    // create path
    sprintf(path, "%s/%s.msg", folder, nick);

    // write only, if not exist then create new file
    // and append new data to the end of the file.
    msgfd = Open(path, O_CREAT | O_APPEND | O_WRONLY, 0);

    // read all of the message and write it into the file.
    while(readline(socket, buf) > 0){
        Rio_writen(msgfd, buf, MAXLINE);
    }

    Close(msgfd);

    return 0;
}


// TODO need to handle read for all messages.
// assume that the message are saved correctly formatted.
// outputfd is the file descriptor for the output. 
int get_messages(char *folder, char *nick, FILE *outputfd){
    if(folder == NULL){
        fprintf(stderr, "folder missing\n");
        exit(1);
    }

    FILE *out = outputfd;
    int msgfd;
    char path[MAXLINE], buf[MAXLINE], peer[MAXLINE/2];

    // directory
    DIR *dir;
    struct dirent *messages;

    // set to standard output if no other is given.
    if(out == NULL){
        out = stdout;
    }


    // get all messages
    if(nick != NULL){
        
        // build directory path.
        sprintf(path, "%s/Messages", folder);

        // fetch directory pointer.
        dir = opendir(path);
        if(dir == NULL){
            fprintf(out, "No New messages.\n");
            Closedir(dir);
            return 0;
        }
    

        // read data struct into messages, and set pointer dir to next struct.
        while((messages = readdir(dir)) != NULL){

            if(strcmp(messages -> d_name, ".") == 0 || strcmp(messages -> d_name, "..") == 0){
                continue;
            }

            // open file for read only        
            // printing header
            sscanf(messages -> d_name, "%s.msg", peer);
            fprintf(out, "Messages from %s\n", peer);
            
            // open file, do not need to check if it exist since
            // we fetch data from directory.
            sprintf(path, "%s/Messages/%s.msg", folder, peer);
            msgfd = Open(path, O_RDONLY, 0);
            // printing messages. 
            while(readline(msgfd, buf) > 0){
                fprintf(out, "%s", buf);
            } 
        
            // remove file from folder e.i. messages
            // remove postpond deletion, if the file is active in any other processes.
            remove(path);

            // close file descriptor
            Close(msgfd);
        }
        // close directory pointer
        Closedir(dir);
        return 0;
    }
    

    sprintf(path, "%s/Messages/%s.msg", folder, nick);
    // check for no new messages e.i. no file no messages.
    if((msgfd = Open(path, O_RDONLY, 0)) < 0){
         fprintf(out, "No new messages from %s\n", nick);
         return 0;
    }
    
    // print header
    fprintf(out, "Messages from %s\n", nick);
    // read and print messages undtil file is empty or error.
    while(readline(msgfd, buf) > 0){
        fprintf(out, "%s", buf);
    }

    // remove file from folder e.i. messages
    // remove postpond deletion, if the file is active in any other processes.
    remove(path);

    // closing file descriptor
    Close(msgfd);

    return 0;
}

int reap_messages(char* user){
    char path[MAXLINE];
    DIR *dir;
    struct dirent *messages;
    
    // build directory path.
    sprintf(path, "%s/Messages", user);
    // fetch directory pointer.
    
    dir = opendir(path);
    
    
    // read data struct into messages, and set pointer dir to next struct.
    while((messages = readdir(dir)) != NULL){
        
        if(strcmp(messages -> d_name, ".") == 0 || strcmp(messages -> d_name, "..") == 0){
            continue;
        }
        // removing file
        sprintf(path, "%s/Messages/%s", user, messages -> d_name);
        remove(path);

    } 
    
    
    sprintf(path, "%s/Messages", user);
    // removing directory
    remove(path);
    remove(user);
    return 0;
}