#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
    char buf[1000], arg0[10], arg1[10], arg2[10];
    int i;
    fscanf(stdin, "%s", buf);
    sscanf(buf, "%s %s", arg0);
    
}