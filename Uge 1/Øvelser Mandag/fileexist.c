#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
    
    for(int i = 1; i < argc; i++){
        FILE* file = fopen(argv[i],"r");
        if(file == 0){
            perror("File not found");
        }
        else{
            printf("File %s can now be read\n\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}