#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
    int sz = argc;

    // open an abitrary number of files and print the content of these files.
    for(int i = 1; i < sz; i++){

        FILE* f = fopen(argv[i],"r");
        if(!f){
            fseek(f,0,SEEK_END);
            size_t fsize = ftell(f);
            fseek(f,0,SEEK_SET);


            char *str = malloc(fsize+1);
            fread(str, fsize,1,f);
            printf("File : %s with content\n\n", argv[i]);
            printf("%s", str);
            printf("\n\n");
            
            fclose(f);
            free(str);
        }
    }

    return EXIT_SUCCESS;
}
