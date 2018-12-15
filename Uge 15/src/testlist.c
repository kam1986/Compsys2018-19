#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "linked_list.h"

int values[10] = { 0,1,2,3,4,5,6,7,8,9 };

list* lst = NULL;

int main(){
    void* data;
    for(int i=0; i<10; i++){
        list_add(&values[i], &lst);
    }
    
    while(!list_is_empty(lst)){
        data = list_head(&lst);
        lst = list_tail(&lst);

        fprintf(stdout, "%d", *((int*) data));
    }




    return 1;
}

*((int*)data)