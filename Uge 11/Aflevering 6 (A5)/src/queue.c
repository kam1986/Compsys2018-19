#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"

struct queue {
    /* the queue */
    void **buffer;
    int mark; // for empty/overflow 
    int end, head, size;
};

int queue_init(queue *queue, int capacity){
    (queue -> buffer) = malloc(sizeof(void*) * capacity);
        queue -> head = 0;
        queue -> end = 0;
        queue -> mark = 0; // 0 if empty, 1 if full
        queue -> size = capacity;

    

    // asserting for allocation error
    assert((queue -> buffer) != NULL); 
    
    return 1;
}

int queue_destroy(queue *queue){
    free(queue -> buffer);
    free(queue);
    
    // asserting freeing
    assert((queue -> buffer) == NULL);
    assert(queue == NULL);

    return 1;
}

int queue_push(queue *queue, void *data){
    int count = (queue -> end) - (queue -> head);
    
    // correct count depending on position of the head and end.
    if(count < 0){
        count = (queue -> size) + count;
    } 

    /*
        checking for full queue
        e.i. not mark = 1 && count < size 
    */
    if((queue -> mark) == 0 || count != (queue -> size)){
        // pushing data onto the queue at end*/
        (queue -> buffer)[queue -> end] =  data;
        
        // increment end point by 1 and wrap around to start if*/
        (queue -> end) = ((queue -> end) + 1) % (queue -> size); 
    
        // check for queue
        if(queue -> head == queue -> end){
            queue -> mark = 1; // set full queue mark
        }

        // return the new count of element in the queue 
        return count + 1;
    } 
    // full queue
    return queue -> size;

}


int queue_pop(queue *queue, void **data){
    int count = (queue -> end) - (queue -> head);
    
    // correct count depending on position of the head and end.
    if(count < 0){
        count = (queue -> size) + count;
    } 

    if((queue -> mark) == 1 || count != 0){
    
        /* put the head at datas location */
        *data = (queue -> buffer)[queue -> head];

        /* incrementing head pointer */ 
        (queue -> head) = ((queue -> head) + 1) % (queue -> size); 
        
        // check for empty
        if(queue -> head == queue -> end){
            queue -> mark = 0; // set empty mark
        }

        // return the count of element in the queue 
        return count - 1;
    }
    
    // empty
    return 0;
}

int queue_count(queue *queue){
    int count = (queue -> head) - (queue -> end);
    
    // correct count depending on position of the head and end.
    if(count < 0){
        count = (queue -> size) + count;
    } 

    return count;
}

