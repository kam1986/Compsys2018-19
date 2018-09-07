#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <errno.h>  // error handling
#include <string.h>

enum file_type {
  DATA,
  EMPTY,
  ASCII
};

const char* const FILE_TYPE_STRING[] = {
  "empty",
  "ASCII text",
  "data"
};

// Assumes: errnum is valid number
int print_error(char *path, int errnum){
  return fprintf(stdout, "%s: cannot determine (%s)\n", 
    path, strerror(errnum));
}

// return the correct responce of if there is linebreaks
char* lineending(int input){
  if(input != 0){
    return "";
  }
  return ", with no line terminators";
}


int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;
  int linebreaks = 0;
  size_t size = 0;
  FILE* stream;
  char c;

  if(argc != 2){
    printf("Usage: ./file path");
    retval = EXIT_FAILURE;
    
  }
  
  else{
      // Try to open file
      stream = fopen(argv[1], "r");
      if(!stream){

        print_error(argv[1], errno);
        retval = EXIT_FAILURE;
        return retval;
      }

      // find file size
      fseek(stream, 0, SEEK_END);
      size = ftell(stream);
      fseek(stream, 0, SEEK_SET);

      // test if empty
      if(size){
          
        // Assumes that the file is ascii
        size = 1;

        while(1){
            // read next character of the file stream.
            c = fgetc(stream);
            
            // check if at file end
            if(c == EOF){
              break;
            }

            if(c == '\n'){
              linebreaks ++;
            }

            
            // check if c is none ascii.
            if(32 > c || c > 126){
              size = 2;
              linebreaks = 1;
              break;
            }
        }
      }
  
  // print the correct answer
  fprintf(stdout, "%s: %s%s\n", 
    argv[1], FILE_TYPE_STRING[size], lineending(linebreaks));
  return retval;
  }
}
