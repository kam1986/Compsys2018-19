#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "job_queue.h"
#include "queue.h"


int job_queue_init(struct job_queue *job_queue, int capacity) {
  
  int size = capacity;
  if(size){}
  // init internal queue.
  queue_init(job_queue -> queue, capacity);
  
  // init internal mutexes 
  pthread_mutex_init(&job_queue -> mutex_pop, NULL);
  pthread_mutex_init(&job_queue -> mutex_push, NULL);

  // init conditional variables
  pthread_cond_init(&job_queue -> c_pop, NULL);
  pthread_cond_init(&job_queue -> c_push, NULL);


  return 1;
}


int job_queue_destroy(struct job_queue *job_queue) {
  
  if(job_queue == NULL){
    // nothing really happens
    return 1;
  }
  // locking both mutex
  pthread_mutex_lock(&(job_queue -> mutex_push));
  pthread_mutex_lock(&(job_queue -> mutex_pop));

  // waits for job_queue to empty
  while(queue_count(job_queue -> queue) != 0)
  {
    // will first wake when job_queue_push calls c_push
    pthread_cond_wait(&job_queue -> c_push, &job_queue -> mutex_pop);
  }
  
  
  // destroying the inner stuff
  queue_destroy(job_queue -> queue);

  // destroying mutexes and c_variables
  pthread_cond_destroy(&(job_queue -> c_push));
  pthread_cond_destroy(&(job_queue -> c_pop));
  
 
  // unlocking mutexes
  pthread_mutex_unlock(&(job_queue -> mutex_push));
  pthread_mutex_unlock(&(job_queue -> mutex_pop));


  // destroy mutexes must be done after unlocking
  pthread_mutex_destroy(&(job_queue -> mutex_push));
  pthread_mutex_destroy(&(job_queue -> mutex_pop)); 
 
  // freeing job_queue
  free(job_queue);

  return 1;
}



int job_queue_push(struct job_queue *job_queue, void *data){

  if(job_queue == NULL || data == NULL){
    // bad reference
    return -1;
  }


  // lock mutex_push of the job queue
  pthread_mutex_lock(&(job_queue -> mutex_push));   
  
  // try pushing onto the queue, set the thread asleep if full (equal capacity).
  while(queue_push(job_queue -> queue, data) == (job_queue -> size)){
    
    // set the thread to wait for c_push.
    pthread_cond_wait(&(job_queue -> c_push), &(job_queue -> mutex_push));
  }
  
  // signal all sleeping pop threads to start again
  pthread_cond_signal(&(job_queue -> c_pop));

  // unluck the mutex of the job queue
  pthread_mutex_unlock(&(job_queue -> mutex_push));

  // return count
  return queue_count(job_queue -> queue);
}



int job_queue_pop(struct job_queue *job_queue, void **data) {
  
  if(job_queue == NULL || data == NULL){
    // bad reference
    return -1;
  }

  // lock mutex_pop of the job_queue
  pthread_mutex_lock(&(job_queue -> mutex_pop));   
  
  // try pushing onto the queue, set the thread asleep if overflow (equal 0).
  while(queue_pop(job_queue -> queue, data) == -1){
    
    // set the thread to wait for 
    pthread_cond_wait(&(job_queue -> c_pop), &(job_queue -> mutex_pop));
  }
  
  // signal all sleeping push_threads to start again.
  pthread_cond_signal(&(job_queue -> c_push));

  // unluck the mutex of the job queue
  pthread_mutex_unlock(&(job_queue -> mutex_pop));

  return queue_count(job_queue -> queue);

}
