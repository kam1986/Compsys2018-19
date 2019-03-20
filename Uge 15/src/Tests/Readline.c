#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../help.h"
#include "../csapp.h"

int main(void){

    char buf[15], *expected = "Hello world\n", *expected1 = "Hello World1\n";
    int source;
    // assert file existens
    assert((source = open("./Tests/TestFiles/Readline.test", O_RDONLY, DEF_MODE)) > 0);

    // assert reading file.
    assert(Readline(source, buf) > 0);

    // test that function correctly.
    assert(strcmp(expected, buf) == 0);

    Readline(source, buf);
    assert(strcmp(expected1, buf) == 0);
    
    // assert error handling for incorrect file describtor.
    assert(Readline(-1,buf) == -1);
    
    // asserting for error handling of uninitialized buffer.
    // (char*) 0 make sure that it is a zero initialized pointer for any system.
    assert(Readline(source, (char*) 0) == -2);
    close(source);
    
    return 0;
}