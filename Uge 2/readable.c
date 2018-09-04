#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>


int main(int argc, char* argv[]){
    
    // check for correct number of arguments 
    if(argc != 2){
        printf("Usage: %s path\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* stream = fopen(argv[1],"r");

    if(!stream){
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Yes\n");
    return EXIT_SUCCESS;

}