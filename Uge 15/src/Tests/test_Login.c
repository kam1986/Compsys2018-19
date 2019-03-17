#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "../help.h"
#include "../csapp.h"

// TODO need to test error cases

int main(void){

   char buf1[MAXLINE], buf2[MAXLINE],
        *expect0 = "NICK: bonii PASS: secret\n",
        *expect1 = "IP: 192.168.1.41",
        *expect2 = "Port: 8181\n";

    // simulate a socket
    int result, link = Open("link", O_CREAT | O_RDWR, DEF_MODE),
        expected_file = Open("./UserLog/bonii.expected", O_RDONLY, DEF_MODE);
    
    remove("./UserLog/bonii.on");
    
    fprintf(stdout, "Testing: Server side login procedure.\n");
    

    Send(link, expect1);
    Send(link, expect2);
    lseek(link, 0, SEEK_SET);

    assert(Login(link, expect0) == 0);
    
    Close(link);
    remove("./link");

    result = Open("./UserLog/bonii.on", O_RDONLY, DEF_MODE); 

    while(1){
        if( Readline(result, buf1) <= 0 || Readline(expected_file, buf2) <= 0){
            break;
        }
        assert(strcmp(buf1, buf2) == 0);
    }

    close(result);
    close(expected_file);

    return 0;

}