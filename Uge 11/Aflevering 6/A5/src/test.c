#include "queue.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


int main(){

    struct queue *q;
    
    queue_init(q, 4);

    void *data;

    int four = 4;
    int two = 2;
    float five = 5.0f;
    char a = 'a';

    
    int out1 = 0;

    /* test for pop on empty queue*/
    assert(queue_pop(q, &data) == 0);
    
    
    /* push stuff on the queue to pop */
    assert(queue_push(q, (void*) &four) == 1);
    
    assert(queue_push(q, (void*) &two) == 2);
    
    assert(queue_push(q, (void*) &a) == 3);

    /* pop the head */
    assert(queue_pop(q, &data) == 2);
    out1 = *((int*)data);
    if(out1 != 4){
        exit(1);
    }
    
    /* test pop output */
    
        /* push some more stuff */
    assert(queue_push(q, (void*) &five) == 3);
    
    assert(queue_push(q, (void*) &two) == 4);
        

    assert(queue_push(q, (void*) &four) == 4);
    
    
    queue_destroy(q);
    
    return 0;
}