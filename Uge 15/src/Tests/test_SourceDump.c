#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../help.h"
#include "../csapp.h"


int main(){

    int s, d;
    char source, destination, sbuf[MAXLINE], dbuf[MAXLINE];

    source      = "./TestFiles/source.test";
    destination = "./TestFiles/destination.test"

    int s = Open(source, O_RDONLY, DEF_MODE);
    int d = Open(destination, O_CREAT | O_RDWR, DEF_MODE);

    // asserting for read/write.
    assert(Source_dump(s,d) == 0);

    // reseting file pointers
    lseek(s, 0, SEEK_SET);
    lseek(d, 0, SEEK_SET);

    while(Realine(s,sbuf) > 0 && Realine(d,dbuf) > 0){
        // asserting correctness
        assert(strcmp(sbuf,dbuf) == 0);
    } 
    Close(s);
    Close(d);

    remove(destination);
    return 0;

}