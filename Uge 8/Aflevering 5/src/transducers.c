#include "transducers.h"
#include "errno.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h" 
#include "unistd.h" /* pipe */


struct stream {
  FILE* file;
  unsigned char flag; /* least significant byte set if 'in use' */
  };

/* from HINTS in assigment page 7 */
static int file_pipe(FILE* files[2]){
  int fds[2];

  int r = pipe(fds);

  if(r == 0){
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");

    if(files[0] && files[1]){
      return 0;
    } else {
      return 1;
    }
  } else {
    return r;
  }
}

void unix_error(char* msg){
  fprintf(stderr, "%s_ %s\n", msg, strerror(errno));
}

pid_t Fork(){
  
  pid_t  pid;

  if ((pid = fork()) < 0){
    unix_error("Fork error");
  }

  return pid;
}

/* ---------------- self define helper functions -------------------- */

/* check if a stream is not in use */
unsigned char not_in_use(stream *s){
  return (s -> flag) ^ 1;
}

/* set the flag for 'in use' */
void use(stream *s){
  (s -> flag) |= 1;
}

/* unset the flag for 'in use' */
void unuse(stream *s){
  (s -> flag) &= 1;
}

/* ------------------------------------------------------------------ */

void transducers_free_stream(stream *s) {
   if(s){}
}

int transducers_link_source(stream **out,
                            transducers_source s, const void *arg) {
  /* new stream */
  stream *ss;
  
  /* the stream should only been read after return */
  FILE *fp[2];

  if(file_pipe(fp)  == 0 ){

    /* create child process */
    if(Fork() == 0){ /* run if child */
      
      /* close for reading */
      fclose(fp[0]);
      
      /* execute the tranducer s with argument arg and send the result to fp */
      s(&arg, fp[1]);      

      /* set the pointer to the new stream ss */
      out = &ss;

      /* return success */
      return 0;
    }
    
    (ss -> file) = fp[0];
    
    /* close for writing */
    fclose(fp[1]);
    
  }
  /* return fault */
  return 1;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
  /*s=s; 
  arg=arg; 
  in=in; 
  */

  /* if the stream 'in' is not in use */
  if(not_in_use(in)){
    
    // block read from other tranducers.
    use(in);
    
    // execute sink
    s(&arg, &in);

    /* unblock stream */
    unuse(in);
  
  } else {
    fprintf(stderr, "stream in use\n");
    
    /* return fault */
    return 1;
  }

  /* return succes */
  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in=in; /* unused */
  return 1;
}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in1=in1; /* unused */
  in2=in2; /* unused */
  return 1;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  out1=out1; /* unused */
  out2=out2; /* unused */
  in=in; /* unused */
  return 1;
}
