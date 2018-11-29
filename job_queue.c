#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"
//And we implement the job_queue.h 
int job_queue_init(struct job_queue *job_queue, int capacity) 
job_queue->capacity = capacity;
job_queue->element_count = 0;
job_queue->start = 0;
job_queue -> end = 0;

(job_queue -> buffer) = malloc(sizeof(void*) * capacity);

// Using the documentation for the pthread library, we define initial values 
pthread_mutex_init(&job_queue->mutex, NULL);
pthread_cond_init(&job_queue->closed, NULL);
pthread_cond_init(&job_queue->open), NULL;

return 1;
}

int job_queue_destroy(struct job_queue *job_queue) {
  // We assume an empty queue so we block the shared resource (Job queue) 

  if(job_queue-> element_count !=0)
  pthread_mutex_lock(&job_queue->mutex);

  free (job_queue->buffer);

  pthread_mutex_unlock(&job_queue->mutex);

  return 1;
  
  /*//Since we want to destroy, we want it the threads to be 1.
  //Advanced Concurrency slides
  if(job_queue-> die == 1){ 

  //Closing everything after we destroy. 
  pthread_cond_broadcast(&job_queue->open);
  pthread_cond_broadcast(&job_queue->closed);
  pthread_mutex_lock(&job_queue->mutex);


  
  }
  
  return 0;*/
}

int job_queue_push(struct job_queue *job_queue, void *data) {
   // We assume an empty queue = block shared resource
  thread_mutex_lock(&job_queue->mutex);
  
  // And then handle when the queue isn't empty
  if(job_queue -> element_count < job_queue -> capacity){ 

    pthread_cond_wait(job_queue->queue)

    (job_queue->buffer [job_queue ->closed]) = data;

  }



  //if full then block
  //if not full then go
  // unblock queue
  // block queue
  //return

  if(&job_queue-> )
  // TODO change return value if needed
  return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
   // We assume an empty queue  = block shared resource 
  thread_mutex_lock(&job_queue->mutex);

  

  if(job_queue -> element_count != 0){

  *data = (queue -> buffer)[queue -> head];

        /* incrementing head pointer */ 
        (queue -> head) = ((queue -> head) + 1) % (queue -> size); 
        
        /* decrementing counter by 1 */
        (queue -> count)--;
    }
    
}

  //lock
  //compare capactiy to POP and modolous 
  // if queue is empty 
  // block 
  return (job_queue->element_count && -1);
}
