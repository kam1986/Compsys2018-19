#include <stdlib.h>
#include <stdio.h>

long p2(long *x, long y){
    long a = 0;
    long b = 0;
    long c = *x;

    while (c != 0){
        if (y == c){
            a++;
        }
        else{
            *(x + b*8) = c;
            b++;
            a++;
        }
        c = *(x + a*8);
    }
    *(x + b*8) = 0;
    
    return (a - b);
}