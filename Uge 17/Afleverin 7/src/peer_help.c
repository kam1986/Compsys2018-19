#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csapp.h"


// A6

int Send(int socket, char *buf){
    if(!socket){
        fprintf(stderr, "NULL sucket.\n");
        exit(1);
    }
    
    char ibuf[MAXLINE];

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

int send_message(int server, char *nick, char* msg){
    
    int connfd;
    char buf[MAXLINE], peer[MAXLINE], ip[MAXLINE], port[MAXLINE];

    // making protocol request
    sprintf(buf, "LOOKUP: %s\n", nick);
    
    // sending request for peer info
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
    sscanf(buf, "%s is online. IP: %s Port: %s", peer, ip, port);

    if((connfd = Open_clientfd(ip, port)) < 0){
        fprintf(stderr, "connection error\n");
        return -1;
    }
    
    // build message protocol 
    sprintf(buf, "MSG: From: %s, Message : %s", nick, msg);
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
    msgfd = Open(path, O_WRONLY | O_CREAT | O_APPEND, 0);

    while(readline(socket, buf) > 0){
        Rio_writen(socket, buf, MAXLINE);
    }

    Close(msgfd);

    return 0;
}


// TODO need to handle read for all messages.
// assume that the message are saved correctly formatted.
// and only one peer for each folder. 
int get_messages(char *folder, char *nick){
    
    int msgfd;
    char path[MAXLINE], buf[MAXLINE];
    sprintf(path, "%s/Message/%s.msg", folder, nick);

    if(nick != NULL){
        // TODO !!!
        // load directory struct, and iterate over it,
        // open and read all .msg files and print.
        // clean directory.
    }
    
    // check for no new messages e.i. no file no messages.
    if((msgfd = Open(path, O_RDONLY, 0)) < 0){
         fprintf(stdout, "No new messages from %s\n", nick);
         return 0;
    }

    // read and print messages undtil file is empty or error.
    fprintf(stdout, "Messages from %s\n\n", nick);
    while(readline(msgfd, buf) > 0){
        fprintf(stdout, "%s", buf);
    }

    // remove file from folder e.i. messages
    // remove postpond deletion, if the file is active in any other processes.
    remove(path);

    // closing file descriptor
    Close(msgfd);

    return 0;
}