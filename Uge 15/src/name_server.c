#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "csapp.h"
#include "name_server.h"

#define ARGNUM 1 // TODO: Put the number of arguments you want the
                 // program to take port number

#define LOG "users.user"


// user list struct 
struct user{
    char* nick;
    char* password;
    char* IP;
    char* port;
    int state;
    user* next;
}

struct user* new_user(struct user *peer, char* userinfo, int state){
    char* nick[100], ip[15], port[5];
    
    struct user user = malloc(sizeof(struct user));

    // filter userinfo
    sscanf(userinfo, "%s %s %s", nick, ip, port);
    // setting this user to new head of the list
    (user -> next)      = peer;
    (user -> nick)      = nick;
    (user -> password)  = password;
    (user -> port)      = port;
    (user -> IP)        = ip;
    (user -> state)     = state; // 0 for offline, 1 for online


}

void login(char* nick, char* password, char* ip, int port){
    
}

void logout(){

}

void lookup(){

}

void exit(){

}


int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return 0;
    }

    rio_t rio;
    char buf[MAXLINE];
    struct user *peers;

    // open listening socket, bind it and set it to reusable.
    int listen;
    
    if((listen = Open_listeningfd(argv[1])) < 0){
        fprintf(stderr, "port: %s not useable\n", argv[1]);
        exit(1);
    }

    int users = Open(LOG, "wr");
    rio_readinitb(rio, users);

    // get users when startup
    while(1){
        if(rio_readlineb(rio, buf, MAXLINE) <= 0){
            break;
        }
        struct user acc = malloc(sizeof(struct user));

        (user -> nick) = buf;
        rio_readlineb(rio, buf, MAXLINE);
        
        
    }
    



    return 0;
}
