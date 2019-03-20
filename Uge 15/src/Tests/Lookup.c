#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "../help.h"
#include "../csapp.h"

int main(void){

    // Serverside testing

    char buf[MAXLINE];
    int pos = 0, acc = 0, peer, exp;

    // setting up bonii.on file
    exp = Open("./UserLog/bonii.on", O_CREAT | O_RDWR, DEF_MODE);
    Send(exp, "IP: 192.168.1.41\nPort: 8181\n");
    Close(exp);

    peer = Open("peer", O_CREAT | O_RDWR, DEF_MODE);
    
    // unknown user test.
    assert(Lookup(peer, "xyz") == -1);

    lseek(peer, 0, SEEK_SET);

    pos = Readline(peer, buf);
    assert(strcmp(buf, "xyz is not a valid user.\n") == 0);

    
    // lookup a valid user test.
    assert(Lookup(peer, "bonii") == 0);

    lseek(peer, pos, SEEK_SET);
    buf[0] = '\0';
    while((acc =Readline(peer, (buf + strlen(buf)))) > 0){
        pos += acc;
    }
    printf(buf);  
    assert(strcmp(buf, "IP: 192.168.1.41\nPort: 8181\n\n") == 0);
    
    assert(Lookup(peer,NULL) == 0);

    lseek(peer, pos, SEEK_SET);
    
    buf[0] = '\0';
    while((acc = Readline(peer, (buf+strlen(buf)))) > 0){
        pos += acc;
    }
    printf(buf);
    

    Close(peer);
    remove("./UserLog/bonii.on");
    remove("peer");  

}