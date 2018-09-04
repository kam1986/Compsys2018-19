#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char* argv[]){
    

    if(argc == 2){
        printf("%s\n\n", argv[1]);
        printf("%s\n", argv[1]);
    }
    else {
        printf("Wrong number of argument");
    }

    return EXIT_SUCCESS;
}