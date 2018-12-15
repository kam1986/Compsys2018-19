#include <stdio.h>
#include "name_server.h"

#define ARGNUM 2 // TODO: Put the number of arguments you want the
                 // program to take

#define Log "users.user"


// user struct 
struct user{
    char* nick;
    char* password;
    char* IP;
    int port;
    int active;
}

int maxusers = 4;

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
        return(0);
    }
    //TODO: Implement
    return 0;
}
