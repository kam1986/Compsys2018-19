#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

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

    if(output == NULL){
        fprintf(stderr, "Uninitialized buffer.\n");
        return -2;
    }

    rio_t rio;
    Rio_readinitb(&rio, source);
    int ret, cur;
    
    cur = lseek(source, 0, SEEK_CUR);
    ret = rio_readlineb(&rio, output, MAXLINE);
    
    // error in pointer position in file descriptor when using MAXLINE
    lseek(source, cur + ret, SEEK_SET);
    
    return ret;
}


// Private function to simplify other helper functions.
// takes an input file discriptor as both in- and output.
int Source_dump(int source, int sink){

    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, source);
    int ret;
    while((ret = rio_readlineb(&rio, buf, MAXLINE)) > 0){
        rio_writen(sink, buf, strlen(buf));
    }

    return ret;
}


// A6 peer side

// takes a file descriptor or socket and a list of argument
int login(int serverfd, char* args){

    char buf[MAXLINE], nick[MAXLINE/8], pass[MAXLINE/8], ip[MAXLINE/8], port[MAXLINE/8]; 
    
    // wrong number of args and possibly empty args
    if(sscanf(args, "%s %s %s %s\n", nick, pass, ip, port) != 4){
        fprintf(stderr, P_LOGIN_ERROR);
        return -1;
    }

    // starting communication with server by sending the nick and pass
    sprintf(buf, P_LOGIN_REQUEST1, nick, pass, ip, port);
    
    Send(serverfd, buf);
    
    Readline(serverfd, buf);
    if(strcmp(buf, S_LOGIN_RESPONCE) != 0){
        fprintf(stderr, S_LOGIN_ERROR);
        return -2;
    }
    
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

    fprintf(stdout, buf);

    return 0;
}

// lookup wrapper uses a sink as output since we will use it in A7 too.
int lookup(int serverfd, int sink, char* args){

    char buf[MAXLINE], buf1[MAXLINE];
    if(args != NULL ){
        // appending
        sprintf(buf, P_LOOKUP_REQUEST, args);
        Send(serverfd, buf); // request lookup table

        Readline(serverfd, buf); // read responce
        sprintf(buf1, "%s is offline\n", args);
        if(strcmp(buf, buf1) == 0){ // testing responce
            fprintf(stderr, buf);
            return 0;
        }

        sprintf(buf, "%s is online.", args);
        Send(sink, buf);
    }

    // dump all from server to terminal
    // server formats the output
    if(Source_dump(serverfd, sink) < 0){
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
    char buf[MAXLINE], buf2[MAXLINE], path[MAXLINE], user[MAXLINE/2];
    int peer;


    // find username
    sscanf(args, "NICK: %s ", user);
  
    sprintf(path, "./UserLog/%s.off", user);
    
    // peer do not exist error
    if((peer = Open(path, O_RDONLY, DEF_MODE)) < 0){
        fprintf(stdout, "Can't find file\n");
        Send(connfd, S_LOGIN_ERROR);
        return -1;
    }

    sscanf(args, "%[^\n]", user);
    sprintf(buf2, "%s\n", user);
    
    Readline(peer, buf);

    // get nick and password of the argument e.i. first line

    // wrong nick or password
    // buf hold the real nick and pass
    // args are the once given by the peer
    if(strcmp(buf, buf2) != 0){
        Send(connfd, S_LOGIN_ERROR);
        return -2;
    }
    
    Close(peer);
    
    sscanf(args, "NICK: %s ", user);
    sprintf(path, "./UserLog/%s.on", user);
    
    peer = Open(path, O_CREAT | O_WRONLY | O_APPEND, DEF_MODE);
    
    // getting host and ip
    while(Readline(connfd, buf) > 0){
        Send(peer, buf);
        lseek(peer, -1, SEEK_CUR);
    }

    Close(peer);
    // Send responce to peer 
    Send(connfd, S_LOGIN_RESPONCE);
    return 0;
}

int Logout(int connfd, char* args){
    
    char path[MAXLINE];
    
    //
    if(connfd < 0){
        fprintf(stderr,"connection error.\n");
        return -1;
    }

    if(args == NULL){
        Send(connfd, "Usage: /logout <nick>");
        return -2;
    }
    sprintf(path,"./UserLog/%s.on", args);
    
    Send(connfd, S_LOGOUT_RESPONCE);
    
    // remove nick.on e.i. login off.
    if(remove(path) < 0){
        printf("file not found\n");
        return -1;
    }

    return 0;
}

int Lookup(int connfd, char* nick){
    
    //
    if(connfd < 0){
        fprintf(stderr,"connection error.\n");
        return -1;
    }

    char *ext, filename[MAXLINE/2], buf[MAXLINE];

    int peer,
        count = 0,
        // temporary file to hold accumulated list of online
        temp = Open("temp", O_CREAT | O_RDWR | O_APPEND, DEF_MODE);
    
    struct dirent *dep;

    // case /lookup
    if(nick == NULL){
    
        DIR *stream = Opendir("./UserLog");

        while((dep = readdir(stream))!= NULL){
            // get pointer to start of file extension if any
            ext = strrchr(dep -> d_name, '.');

            // finden online users
            if(strcmp(ext, ".on") == 0){
                // getting filename
                sscanf(dep -> d_name, "%[^'.']", filename);
                
                sprintf(buf,"NICK: %s", filename);
                Send(temp, buf);
                Lookup(temp, filename);
                
                // counting online users
                count++;
            }

        }

        lseek(temp, 0, SEEK_SET);
        sprintf(buf, "%d user online. The list follows", count);
        Send(connfd, buf);

        
        Closedir(stream);
        Close(temp);
        remove("temp");
        return 1;
    }

    // case /lookup <nick>
    sprintf(buf, "./UserLog/%s.on", nick);
    
    if((peer = open(buf, O_RDONLY, DEF_MODE)) < 0){
        
        sprintf(buf, "./UserLog/%s.off", nick);
        if((peer = open(buf, O_RDONLY, DEF_MODE)) < 0){
            sprintf(buf, "%s is not a valid user.", nick);
            Send(connfd, buf);
            return -1;
        }
        
        // lookup check return value for correct handling
        sprintf(buf, "%s is offline.", nick);
        Send(connfd, buf);
        return 0;
        
    }

    while(Readline(peer, buf) > 0){
        Send(connfd, buf);
        lseek(peer,-1,SEEK_CUR);
    }

    return 0;
}


