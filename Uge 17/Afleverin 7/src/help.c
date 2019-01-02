#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "csapp.h"


int Send(int socket, char *buf){
    if(sucket == NULL){
        fprintf(stderr, "NULL sucket.\n")
        exit(1);
    }
    
    char *ibuf[MAXLINE];

    sprintf(ibuf, "%s\n", buf); 
    
    // sending content of buf throug socket.
    return Rio_writen(socket, ibuf, strlen(ibuf));

}

// rename
int readline(int socket, char **buf){
    char ibuf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio,connfd);
    int ret = sRio_readlineb(&rio, ibuf, strlen(ibuf));

    buf = &ibuf;

    return ret;
}

int look_up(int server, char* nick){

    return 0;
}

int send_message(int server, char *nick, char* msg){
    
    int connfd;
    char* buf[MAXLINE], nick[MAXLINE], ip[MAXLINE], port[MAXLINE];
    struct tm * timeinfo; // timestamp of message.
    time_t mytime = time(NULL);

    // making protocol request
    sprintf(buf, "LOOKUP: %s\n", nick);
    
    // sending request for peer info
    Send(server, buf);

    // get response
    readline(server, &buf);
    
    // check for not online/non user
    if(strcmp(buf, "NOT FOUND\n") == 0){
        fprintf(stderr, "peer not found\n");
        // OBS - could make the server safe them for the peer.
        return -1;
    }

    // filter response
    sscanf(buf, "%s is online. IP: %s Port: %s", nick, ip, port);

    if((connfd = Open_client(ip, port)) < 0){
        fprintf(stderr, "connection error\n");
        return -1;
    }

    char time_str = ctime(&mytime); // get time
    time_str[strlen(time_str)-1] = '\0'; // remove newline char.
    
    // build message protocol 
    sprintf(buf, "MSG: From: %s, Message : %s", nick, msg);
    send(connfd, msg); // send message.

    return 0;
}

int store_message(char *msg){
    
    return 0;
}

int get_messages(char* nick){
    
    int msgfd;
    char *path[MAXLINE];
    sprintf(path, "Messages/%s.msg", nick);

    if(nick != NULL){
        // TODO !!!
        // load directory struct, and iterate over it,
        // open and read all .msg files and print.
        // clean directory.
    }
    
    // check for no new messages e.i. no file no messages.
    if((msgfd = Open(path, O_RDONLY,0)) < 0){
         fprintf(stdout, "No new messages from %s\n", nick);
         return 0;
    }

    // read and print messages undtil file is empty.
    while(readline(msgfd, buf) > 0){
        print("%s", buf);
    }

    // remove file e.i. messages
    unlink(path); // unlink postpond if the file is active in any other processes.

    return 0;
}   

