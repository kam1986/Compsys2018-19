#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help.h"
#include "csapp.h"




// send content to file descriptor with line ending appended
int Send(int clientfd, char* content){
    if(clientfd < 0){
        fprintf(stderr, "NULL sucket.\n");
        return -1;
    }

    if(content == NULL){
        fprintf(stderr, "non initialized string.\n");
        return -2;
    }
    
    char buf[MAXLINE];
    // adding newline 
    sprintf(buf, "%s\n", content); 
    
    
    // sending content of buf through socket.
    return rio_writen(clientfd, buf, strlen(buf));
}

// Read next line of source into output
int Readline(int source, char* output){

    if(source < 0){
        fprintf(stderr, "Non legal Source.\n");
        return -1;
    }

    if(output == (char*) 0){
        fprintf(stderr, "Uninitialized buffer.\n");
        return -2;
    }

    rio_t rio;
    Rio_readinitb(&rio, source);
    
    return rio_readlineb(&rio, output, MAXLINE);
}


// Private function to simplify other helper functions.
// takes an input file discriptor as both in- and output.
int Source_dump(int source, int sink){

    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, source);
    int ret;
    while((ret = rio_readlineb(&rio, buf, MAXLINE)) > 0){
        rio_writen(sink, buf, MAXLINE);
    }

    return ret;
}


// A6 peer side

// takes a file descriptor or socket and a list of argument
int login(int serverfd, char* args){

    char buf[MAXLINE], nick[MAXLINE/4], pass[MAXLINE/4], ip[MAXLINE/4], port[MAXLINE/4]; 
    
    // wrong number of args and possibly empty args
    if(scanf(args, "%s %s %s %s\n") != 4){
        fprintf(stderr, P_LOGIN_ERROR);
        return -1;
    }

    // starting communication with server by sending the nick and pass
    sprintf(buf, P_LOGIN_REQUEST1, nick, pass);
    // send are define in ...
    Send(serverfd, buf);
    
    // readline are defined in ... 
    Readline(serverfd, buf);

    if(strcmp(buf, S_LOGIN_REQUEST) != 0){
        fprintf(stderr, S_LOGIN_ERROR);
        return -2;
    }

    // does not check against invalid ip and ports
    sprintf(buf, P_LOGIN_REQUEST2, ip, port);
    // send are define in ...
    Send(serverfd, buf);

    return 0;
}

// logout function
int logout(int serverfd, char* nick){
    if(nick == NULL || strcmp(nick, "") == 0){
        
        fprintf(stderr, "logout error\nUsage: /logout <nick>\n");
        
        return -1;
    }
    
    char buf[MAXLINE];

    sprintf(buf, P_LOGOUT_REQUEST, nick);
    Send(serverfd, buf);

    Readline(serverfd, buf);
    
    // checking feedback from server. 
    if(strcmp(buf, S_LOGOUT_RESPONCE) != 0){

        fprintf(stderr, buf);
        return -2;
    }

    fprintf(stdin, buf);

    return 0;

}

// lookup wrapper uses a sink as output since we will use it in A7 too.
int lookup(int serverfd, int sink, char* args){

    char buf[MAXLINE];
    sprintf(buf, P_LOOKUP_REQUEST, args);
    Send(serverfd, buf);

    // dump all from server to terminal
    // server formats the output
    if(Source_dump(serverfd, sink) != 0){
        fprintf(stderr, "Connection error\n");
        return -1;
    }
    
    return 0;
}

int Exit(int serverfd, char* args){
    
    logout(serverfd, args);
    Send(serverfd, "EXIT");
    exit(0);
}


// A6 server side
int Login(int connfd, char* args){
    char buf[MAXLINE], path[MAXLINE], user[MAXLINE/2];
    int peer;
    
    // find username
    sscanf(args, "NICK: %s ", user);
    sprintf(path, "./UserLog/%s.off", user);
    
    // peer do not exist error
    if((peer = Open(path, O_RDWR, DEF_MODE)) < 0){
        Send(connfd, S_LOGIN_ERROR);
        return -1;
    }

    Readline(peer, buf);

    // wrong nick or password
    if(strcmp(buf, args) != 0){
        Send(connfd, S_LOGIN_ERROR);
        return -1;
    }
    
    // set position to start of the file
    lseek(peer, 0,SEEK_SET);
    // set mark to prevent multiple login
    Send(peer, "LOGGED IN");
    Send(peer, buf); // read nick and password back to the userfile.

    Close(peer);
    sprintf(path, "./UserLog/%s.on", user);
    
    Send(connfd, "Send Host and ip");
    /*
        assumes that this will never return error
        since the only error are memory shortage.
    */
    peer = Open(path, O_RDWR | O_CREAT, DEF_MODE);
    
    // writing first line of format to userfile
    sprintf(buf, "Nick: %s", user);
    Send(peer, buf);
    // getting host and ip
    Readline(connfd, buf);
    Send(peer, buf);
    Close(peer);

    // Send responce to peer 
    Send(connfd, S_LOGIN_RESPONCE);
    return 0;
}

int Logout(int connfd, char* args){
    if(connfd || args){}
    return 0;
}

int Lookup(int connfd, char* args){
    if(connfd || args){}
    return 0;
}


