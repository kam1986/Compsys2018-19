#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library
#include <sys/types.h>  // system type library
#include <dirent.h>     // standard directory library

#include "csapp.h"      // helper functions from the book


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
/*
int login(char *nick, char *pass, char *ip, char* port){
    return 0;
}

int logout(){
    // TODO
    // CLOSE socket.
    // remove 
    return 0;
}

int look_up(int server, char* nick){
    // TODO
    return 0;
}

int EXIT(){
    logout();

    exit(0);
}
*/

// A7 part.

// Send msg to nick, msg must be shorter than MAXLINE.
int send_message(int server, char *nick, char* msg){
    
    int connfd;
    char buf[MAXLINE], peer[MAXLINE], ip[MAXLINE], port[MAXLINE];

    // making protocol request
    sprintf(buf, "LOOKUP: %s", nick);
    
    // sending request for peer infotest
    Send(server, buf);

    // get response
    readline(server, buf);
    
    // check for not online/non user
    if(strcmp(buf, "NOT FOUND\n") == 0){
        fprintf(stderr, "peer not found\n");
        // OBS - could make the server safe them for the peer.
        return -1;
    }

    // filter response
    sscanf(buf, "%s is online. IP: %s Port: %s\n", peer, ip, port);

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
    char path[MAXLINE], buf[MAXLINE], peer[MAXLINE];

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

            if(!strcmp(messages -> d_name, ".") || !strcmp(messages -> d_name)){
                continue;
            }

            // open file for read only        
            // printing header
            sscanf(messages -> d_name, "%s.msg", peer);buf
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

int reap_messages(char* folder){
    char path[MAXLINE];
    
    // build directory path.
    sprintf(path, "%s/Messages", folder);

    // fetch directory pointer.
    dir = opendir(path);
    if(dir == NULL){
        // nothing to do.
        Closedir(dir);
        return 0;
    }

    // read data struct into messages, and set pointer dir to next struct.
    while((messages = readdir(dir)) != NULL){
        
        if(!strcmp(messages -> d_name, ".") || !strcmp(messages -> d_name)){
            continue;
        }
        // removing file
        sprintf(path, "%s/Messages/%s", folder, messages -> d_name);
        remove(path);

        // close file descriptor
        Close(msgfd);
    }
    
    sprintf(path, "%s/Messages", folder);
    // removing directory
    remove(path);
    return 0;
}