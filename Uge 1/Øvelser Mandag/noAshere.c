#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char* argv[]){
    
    for(int i = 1; i < argc; i++){
        FILE* stream = fopen(argv[i], "r");
        int c;


        if(!stream){
            perror("File not found");
            return EXIT_FAILURE;
        }

        do{
            c = fgetc(stream);

            if(feof(stream))
            {
                break;
            }

            if(c == 'A' || c == 'a'){
                printf("No A'a are allowed\n\n");
                return EXIT_SUCCESS;
            }
        }while(1);

        printf("File %s is ok\n\n",argv[i]);
    }
    return EXIT_SUCCESS;
}