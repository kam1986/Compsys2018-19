#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library

#include "../peer_help.h"



int main(){

    int serverfd, Santafd, Rudolffd, status;

    int server_ip   = 127.0.0.1,
        serverport  = 1080,

        Santa_ip    = 127.0.0.2,

        Rudolf_ip   = 127.0.0.3;
    
    pid_t server, Santa, Rudolf;

    char buf[MAXLINE],
        
        // Peer 1
        *Santa_nick = "Santa",
        *Santa_pass = "HOHO",
    
        // Peer 2
        *Rudolf_nick = "Rudolf",
        *Rudolf_pass = "Reindear";
    
    if((server = Fork()) == 0){
        // execute server program
        
    } else {


        if((Santa = Fork()) == 0){
            // 
        }

        if((Rudolf == Fork()) == 0){

        }
    }
    
    exit(0);

}