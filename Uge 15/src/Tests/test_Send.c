#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "../help.h"
#include "../csapp.h"

int main(void){
    int fd;
    char *test, *expected = "Hello World\n", buf[15];

    assert((fd = Open("./TestFiles/Send.test", O_CREAT | O_RDWR, DEF_MODE)) > 0);

    // asserting normal case
    assert(Send(fd, expected) > 0);

    // asserting corner case
    assert(Send(-1, expected) == -1);

    lseek(fd, 0, SEEK_SET);
    Readline(fd, buf);
    
    // assert normal output.
    assert(strcmp(expected, buf) == 0);

    lseek(fd, 0, SEEK_SET);

    assert(Send(fd, NULL) == -2);

    lseek(fd, 0, SEEK_SET);
    Readline(fd, buf);
 
    Close(fd);
    
    return 0;
}