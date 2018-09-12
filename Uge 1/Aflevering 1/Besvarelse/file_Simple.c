#include<stdio.h> 	// fprintf, stdout, stderr.
#include<stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include<string.h>	// strerror.
#include<errno.h>	// errno.

// an array were we can index the given answer
char* answers[3] = {
	"empty", 
	"ASCII text",
	"data",
};


// print correct error for handling corrupt and missing files.
int print_error(char *path, int errnum){
	return fprintf(stdout, "%s: cannot determine (%s)\n",
			// strerror takes a error code number and return a standard error string
		path, strerror(errnum));
}

int main(int argc, char *argv[]){
	
	FILE *stream;	
	
	char c;
	
	size_t size=0;
	
	// Test if the program get other then one input
	if(argc != 2){
		fprintf(stdout, "Usage: ./file path");
		return EXIT_FAILURE;
	}
	
	// try to open file ("r" == readonly mode)
	stream = fopen(argv[1],"r");
	
	// test if stream is a NULL pointer aka. file reading error
	if(!stream){
		// argv[1] are the first argument of the main function, errno (from errno.h)
		// is a number code for the last tricked error.
		print_error(argv[1], errno);
		return EXIT_FAILURE;
	}

	// gets size of the file ftell returns the displacement of the pointer in bytes,
	// from the start of the stream
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);

	// reset stream pointer to head of the stream.
	fseek(stream, 0, SEEK_SET);

	
	

	// test for empty file	
	if(size){
		// assume that the file is ASCII.
		size = 1;
		
		// infinity loop condition
		while(1){
            
			c = fgetc(stream);

			// breaks loop if we get a eof char (End Of File).
			if(c==EOF) {
				break;
			}

			// test for none-ASCII text characters
			// (20 < c < 126 are printable chars)
			if((c < 0x20 && (c < 0x07 || c > 0x0D) && c != 0x1B) || c > 0x7E){
				// set size to the correct index
				size = 2;
				break;			
			}
			
		}
	}
	
	
	// print the correct answer
	fprintf(stdout, "%s: %s\n", argv[1], answers[size]);

	// return successfully
	return EXIT_SUCCESS;	
}