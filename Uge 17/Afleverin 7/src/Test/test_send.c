#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library
#include <unistd.h>
#include "csapp.h"
#include "peer_help.h"
#include <assert.h>


int main(){
    int fd, ret1, ret2;
    rio_t rio;

    char *buf = "HELLO WORLD";
    char *exp = "HELLO WORLD\n";
    char buf2[MAXLINE];
    fd = Open("Test output/Test1.txt", O_CREAT | O_RDWR,  0666);

    Send(fd, buf);
    
    // reset pointer position in file
    lseek(fd, 0, 0);

    ret1 = readline(fd, buf2);
    assert(strcmp(exp, buf2) == 0);


}