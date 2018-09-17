#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main(int argc, char* argv[]){

    // tjek for antal argumenter
    if(argc != 2){
        fprintf(stdout, "Usage: %s arg\n", argv[0]);
        return EXIT_FAILURE;
    }
    // åben file og init c
    FILE* stream = fopen(argv[1],"r");
    char c;

    if(!stream){
        // print error message
        fprintf(stdout, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    // køre lykke for hver char c og print c som hexabyte repræsentation med linjeskift
    do{
        // set c as next char in file
        c = fgetc(stream);
        
        // break loop end end of file
        if(c == EOF){
            break;
        }
        // print c as hexabyte repr plus linebreak.
        fprintf(stdout,"%02x\n", c);

    }while(1);

    return EXIT_SUCCESS;
}