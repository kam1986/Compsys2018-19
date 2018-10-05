#include<stdlib.h>
#include<stdio.h>

int add(int x, int y){
    return x + 2*y-y;
}

int main(void){
    int x = 3;
    int y = 4;

    return add(x,y);
}