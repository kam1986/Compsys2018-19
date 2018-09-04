#include<stdio.h>
#include<string.h>

int main(int argc,char *arg[]){
    if(argc > 1){
        char c = arg[1][0];
        if (c == 'A'){
            printf("No beginning A's are Allowed\n\n");
        }
        else{
            printf("%s\n\n",arg[1]);
        }
    }
    return 0;
}