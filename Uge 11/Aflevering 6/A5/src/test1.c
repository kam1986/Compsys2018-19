#include "job_queue.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


int main(){

    struct job_queue *q;
    
    job_queue_init(q, 4);

    void *data;

    int four = 4;
    int two = 2;
    float five = 5.0f;
    char a = 'a';

    
    int out1 = 0;
    /* test for pop on empty queue*/
    assert(job_queue_pop(q, &data) == 0);
    
    
    /* push stuff on the queue to pop */
    assert(job_queue_push(q, (void*) &four) == 1);
    
    assert(job_queue_push(q, (void*) &two) == 2);
    
    assert(job_queue_push(q, (void*) &a) == 3);

    /* pop the head */
    assert(job_queue_pop(q, &data) == 2);
    out1 = *((int*)data);
    if(out1 != 4){
        exit(1);
    }
    
    /* test pop output */
    
        /* push some more stuff */
    assert(job_queue_push(q, (void*) &five) == 3);
    
    assert(job_queue_push(q, (void*) &two) == 4);
        
    assert(job_queue_push(q, (void*) &four) == 4);
  

    job_queue_destroy(q);
    
    return 0;
}