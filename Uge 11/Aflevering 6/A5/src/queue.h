

typedef struct queue queue;

/* 
    initialize a new empty queue
    return 1 on success else 0
*/

int queue_init(queue *queue, int capacity);

/* 
    destroy a queue
    return 1 on success else 0
*/

int queue_destroy(queue *queue);

/* 
    push an element on to the end of the queue.
    returns the number of element of the queue.
*/
int queue_push(queue *queue, void *data);


/*
    pop an element of the head of the queue
    return number of elements in the queue.
*/
int queue_pop(queue *queue, void **data);


int queue_count(queue *queue);  