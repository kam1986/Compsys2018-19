#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "../help.h"
#include "../csapp.h"

// TODO need to test error cases

int main(void){

    // Server side testing
    // OBS do not test errors

   char buf1[MAXLINE], buf2[MAXLINE],
        *expect0 = "NICK: bonii PASS: secret\n",
        *expect1 = "IP: 192.168.1.41",
        *expect2 = "Port: 8181";

    // simulate a socket
    int result, link = Open("link", O_CREAT | O_RDWR, DEF_MODE),
        expected_file = Open("./UserLog/bonii.expected", O_RDONLY, DEF_MODE);
    
    // clean up before testing
    remove("./UserLog/bonii.on");
    
    fprintf(stdout, "Testing: Server side login procedure.\n");
    

    Send(link, expect1);
    Send(link, expect2);
    lseek(link, 0, SEEK_SET);

    sprintf(buf1, "%s%s\n%s\n", expect0,expect1,expect2);

    assert(Login(link, buf1) == 0);
    
    Close(link);
    remove("./link");

    result = Open("./UserLog/bonii.on", O_RDWR | O_APPEND, DEF_MODE); 

    while(Readline(result, buf1) <= 0 && Readline(expected_file, buf2) <= 0){
        assert(strcmp(buf1, buf2) == 0);
    }

    Close(result);
    Close(expected_file);


    // Peer side testing
    // OBS does not test errors
    
    fprintf(stdout, "Testing: Peer side login procedure.\n");

    char *args1 = "NICK: bonii PASS: secret\nIP: 192.168.1.41\nPort: 8181\n";

    int logfile = Open("./UserLog/bonii.test", O_RDONLY, DEF_MODE),
        errorfile = Open("./UserLog/bonii.error", O_RDONLY, DEF_MODE);

    assert(login(logfile, args1) == 0);

    printf("\ntesting argument error\n");
    assert(login(logfile, "") == -1);


    lseek(logfile, 0, SEEK_SET);
    // password error
    printf("\ntesting responce message error\n");
    assert(login(errorfile, " <-- fd Get error message back test") == -2);

    Close(logfile);

    return 0;

}