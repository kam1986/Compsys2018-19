#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s path", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* stream = fopen(argv[1],"r");

    if(!stream){
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    fseek(stream,0,SEEK_END);
    long bytes = ftell(stream);
    fclose(stream);
    

    fprintf(stdout, "%ld\n", bytes);
    return EXIT_SUCCESS;
}