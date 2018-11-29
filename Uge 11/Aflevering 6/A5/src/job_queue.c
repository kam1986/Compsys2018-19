#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "job_queue.h"
#include "queue.h"


int job_queue_init(struct job_queue *job_queue, int capacity) {
  // init internal queue.
  queue_init(&(job_queue -> queue), capacity);
  // init internal mutex
  (job_queue -> mutex)   = PTHREAD_MUTEX_INITIALIZER;
  (job_queue -> c_val)   = PTHREAD_COND_INITIALIZER;
  (job_queue -> c_empty) = 
}

int job_queue_destroy(struct job_queue *job_queue) {
  
  // wiats for the 
  while(queue_count(job_queue -> queue) != 0)
  {
    pthread_cond_wait(&(job_queue -> c_empty));
  }

  // destroying the inner stuff
  queue_destroy(&(job_queue -> queue));
  
  pthread_mutex_unlock(job_queue -> mutex);
  pthread_mutex_destroy(&(job_queue -> mutex));
  pthread_cond_destroy(&(job_queue -> c_val));
  
  // freeing object
  free(job_queue);
}

int job_queue_push(struct job_queue *job_queue, void *data) {

  // lock mutex of the job queue
  pthread_mutex_lock(&(job_queue -> mutex));   
  
  // try pushing onto the queue, set the thread asleep if overflow (equal 0).
  while(queue_push(&(job_queue -> queue), data) == 0){
    pthread_cond_wait(&(job_queue -> c_val), &(job_queue -> mutex));
  }
  
  // unluck the mutex of the job queue
  pthread_cond_signal(&(job_queue -> c_val));

  // signal all sleeping threads to start again.
  pthread_mutex_unlock(&(job_queue -> mutex)); 
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
 
  // lock mutex of the job queue
  pthread_mutex_lock(&(job_queue -> mutex));   
  
  // try pushing onto the queue, set the thread asleep if overflow (equal 0).
  while(queue_push(&(job_queue -> queue), data) == 0){
    pthread_cond_wait(&(job_queue -> c_val), &(job_queue -> mutex));
  }
  
  // unluck the mutex of the job queue
  pthread_cond_signal(&(job_queue -> c_val));

  // signal all sleeping threads to start again.
  pthread_mutex_unlock(&(job_queue -> mutex)); 
}
