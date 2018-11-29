#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"

struct queue {
    /* the queue */
    void **buffer;
    
    int end, head, size, count;
};

int queue_init(queue *queue, int capacity){
    (queue -> buffer) = malloc(sizeof(void*) * capacity);
        queue -> head = 0;
        queue -> count = 0;
        queue -> end = 0;
        queue -> size = capacity;
    
    return 1;
}

int queue_destroy(queue *queue){
    free(queue -> buffer);

    return 1;
}

int queue_push(queue *queue, void *data){
    /* check for none emptyness */
    if(queue -> count < queue -> size){

        /* pushing data onto the queue at end*/
        (queue -> buffer)[queue -> end] =  data;
        
        /* increment end point by 1 and wrap around to start if*/
        (queue -> end) = ((queue -> end) + 1) % (queue -> size); 
        
        /* increment counter by 1 */
        (queue -> count)++;
        
        return (queue -> count) ;
    }

    /* 
        return the number of elements in the queue
        or 0 if empty and overflow.
    */
    return 0;
}


int queue_pop(queue *queue, void **data){
    
    if(queue -> head != queue -> end){
        /* put the head at datas location */
        *data = (queue -> buffer)[queue -> head];

        /* incrementing head pointer */ 
        (queue -> head) = ((queue -> head) + 1) % (queue -> size); 
        
        /* decrementing counter by 1 */
        (queue -> count)--;
    }
    
    return (queue -> count) ;
}

int queue_count(queue *queue){
    return queue -> count;
}