#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library
#include <assert.h>

#include "../peer_help.h"
#include "../csapp.h"

int main(){


    int input, 
        line = 0;
    
    char buf[MAXLINE], 
        *Inputfile = {   
                "Test input/Empty", 
                "Test input/Oneliner", 
                "Test input/Hello World"
            },

        *expected = {
            "",
            "First line",
            "HELO WORLD\n",
            "Second line\n"
        };


    for(int file = 0; file < 4; file++){

        input = Open(Inputfile[file], O_RDONLY, 0666);
        
        while(1){

            readline(input, buf);
            
            // make sure that we check for empty file. 
            if(line != 0 && strcmp(buf, "") == 0){
                break;
            }
            
            assert(strcmp(buf, expected[line]) == 0);

            line++;
        }

        Close(input);
    }
    // read first line.
    readline(input, buf);

    assert(strcmp(buf,expected) == 0);

}