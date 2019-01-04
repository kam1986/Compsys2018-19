#include <stdio.h>      // standard input/output library
#include <stdlib.h>     // standard function library
#include <string.h>     // string library
#include <sys/types.h>  // system type library
#include <dirent.h>     // standard directory library


int main(){
    DIR *dir;
    struct dirent *file;
    char buf[MAXLINE];

    dir = opendir(".");

    while((file = readdir(dir)) != NULL){
        printf("%s\n", file -> d_name);
    }
    return 0;
}