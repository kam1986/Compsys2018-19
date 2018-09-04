#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){

    if(argc != 2){
        printf("Usage: %s argument\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Hello %s!\n", argv[1]);
    return EXIT_SUCCESS; 
}