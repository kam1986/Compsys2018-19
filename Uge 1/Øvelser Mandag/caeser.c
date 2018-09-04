#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
    char* caeser;
    int sz;
    char* mode;
    int pivot;
    int ret;

    switch(argc){
        case 1:
            perror("no argument given");
            break;

        case 2:
            caeser = argv[1];
            sz=strlen(caeser);
            pivot = 1;
            ret = 1;
            break;


        default:
            mode = argv[1];
            pivot =  atoi(argv[2]);
            
            caeser = argv[3];
            sz = strlen(caeser);

            // ! because we have equals = 0 when right.
            if(!strcmp("-enc",mode)){
                ret = 1;
            }
            if(!strcmp("-dec",mode)){
                ret = 2;
            }
            
            break;
    }


    if(ret == 1){
        for(int i=0;i<sz;i++){
            caeser[i]+=pivot;
        }
    }
    if(ret == 2){
        for(int i=0;i<sz;i++){
            caeser[i]-=pivot;
        }
    }

    printf("%s\n\n",caeser);

    return EXIT_SUCCESS;
    
}