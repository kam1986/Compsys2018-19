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
		path, strerror(errnum));
}

int main(int argc, char *argv[]){
	
	FILE *stream;	
	
	char c;
	
	size_t size=0;
	
	// Test if the program get more then one input
	// TODO - write the correct message. 	
	if(argc != 2){
		printf("Usage: ./file path");
		return EXIT_FAILURE;
	}
	
	// try to open file
	stream = fopen(argv[1],"r");
	
	
	// gets size of the file ftell returns the displacement of the pointer in bytes,
	// from the start of the stream
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);

	// reset stream pointer to head of the stream.
	fseek(stream, 0, SEEK_SET);

	// test for small files.
	

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