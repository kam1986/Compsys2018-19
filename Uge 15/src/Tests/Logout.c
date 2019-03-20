#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../help.h"
#include "../csapp.h"


int main(void){

    // server side

    char buf[MAXLINE],
         *expect0 = "NICK: bonii PASS: secret\n",
         *expect1 = "IP: 192.168.1.41",
         *expect2 = "Port: 8181";

    int link = Open("link", O_CREAT | O_RDWR | O_APPEND, DEF_MODE);

    // clean up before testing
    remove("./UserLog/bonii.on");
    remove("link");
    remove("link2");

    fprintf(stdout, "Testing: Server side logout procedure.\n");

    // log in
    Send(link, expect1);
    Send(link, expect2);
    lseek(link, 0, SEEK_SET);

    sprintf(buf, "%s%s\n%s\n", expect0,expect1,expect2);

    Login(link, buf);

    assert(Logout(link, "bonii") == 0);

    fprintf(stdout, "Testing wrong fd input\n");
    assert(Logout(-1,"bonii") == -1);
    assert(Logout(link, NULL) == -2);



    // peer side
    
    int link2 = Open("link2", O_CREAT | O_RDWR, DEF_MODE);
    
    fprintf(stdout, "Testing /logout\n");
    // write argument send when calling logout to make sure that
    // logout read from the correct position after calling it.
    Send(link2, "LOGOUT: bonii\n");
    
    // logout will read this line
    Send(link2, S_LOGOUT_RESPONCE);
    lseek(link2, 0, SEEK_SET);
    
    assert(logout(link2, "bonii") == 0);

    Close(link);
    Close(link2);
    remove("link");
    remove("link2");

    printf("\nSuccess\n\n");
    return 0;

}