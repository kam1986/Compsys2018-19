#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stdout, "Usage: %s value index", argv[0]);
        return EXIT_FAILURE;
    }
    
    long value;
    long index;
    
    // 
    if(!sscanf(argv[1], "%ld", value)){
        fprintf(stdout, "value : %s is not of type int64", argv[1]);
        return EXIT_FAILURE;
    }

    // 
    if(!sccanf(argv[2], "%ld", index)){
        fprintf(stdout, "index : %s is not of type int64", argv[2]);
        return EXIT_FAILURE;
    }

    if(index < 0 || index > 63){
        fprintf(stdout, 
            "index : %ld is out of range\nTry -1 < index < 64 ");
    }

    // shift the least significand bit index to the left.
    long bit = 1L << index;

    value = value ^ bit; 

    fprintf(stdout, "%ld\n", value);
    return EXIT_SUCCESS;
}