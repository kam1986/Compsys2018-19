#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"
//And we implement the job_queue.h 
int job_queue_init(struct job_queue *job_queue, int capacity) {
job_queue->capacity = capacity;
job_queue->element_count = 0;
job_queue->start = 0;
job_queue -> end = 0;
job_queue->die = 0;

(job_queue -> buffer) = malloc(sizeof(void*) * capacity);

// Using the documentation for the pthread library, we define initial values 
pthread_mutex_init(&job_queue->mutex, NULL);
pthread_cond_init(&job_queue->closed, NULL);
pthread_cond_init(&job_queue->open, NULL);

return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {

  // we block the shared resource (Job queue) 
  pthread_mutex_lock(&job_queue->mutex);

  while((job_queue-> element_count !=-1)){
    pthread_cond_wait(&job_queue->closed, &job_queue->mutex);

  }
  job_queue -> die = 1;

  free (job_queue->buffer);

  return EXIT_SUCCESS;
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  pthread_mutex_lock(&job_queue->mutex);

  
    int placeholder = (job_queue ->end + job_queue ->element_count )%(job_queue ->capacity);
    job_queue->buffer[placeholder] = data;
    job_queue->element_count +=1;

      while(&job_queue->buffer, data == &job_queue->capacity){
        pthread_cond_wait(&job_queue->closed, &job_queue->mutex);
      }
    
    pthread_cond_signal(&job_queue->closed);
    pthread_mutex_unlock(&job_queue->mutex);

    return EXIT_SUCCESS;


  }


int job_queue_pop(struct job_queue *job_queue, void **data) {
    pthread_mutex_lock(&job_queue->mutex);
 
    if(job_queue-> die == 1){
      return -1;
    }

    while(&job_queue->element_count == 0){
      pthread_cond_wait(&job_queue->closed, &job_queue->mutex);

  }


  *data = (job_queue->buffer [(job_queue ->start +1)%(job_queue ->capacity)]);
  job_queue->element_count -= 1;

  pthread_mutex_unlock(&job_queue->mutex);


  return 0;

    }
  
