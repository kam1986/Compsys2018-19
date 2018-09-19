#include<stdio.h> 	// fprintf, stdout, stderr.
#include<stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include<string.h>	// strerror.
#include<errno.h>	// errno.

// an array were we can index the given answer
char* answers[7] = {
	"empty", 
	"ASCII text",
	"data",
	"ISO-8859",
	"UTF-8 Unicode",

	// endian order determenes te order in which bits are read.
	// e.i. left ot right or right to left.
	"Little-endian UTF-16 Unicode",
	"Big-endian UTF-16 Unicode"
};



// print correct error for handling corrupt and missing files.
int print_error(char *path, int errnum){
	return fprintf(stdout, "%s: cannot determine (%s)\n",
			// strerror takes a error code number and return a standard error string
		path, strerror(errnum));
}


// check for none ascci data charachters
int CheckForData(FILE* stream){
	char c;

	int ret = 1;

	while(1){
				
		c = fgetc(stream);

		// breaks loop if we get a eof char (End Of File).
		if(c==EOF) {
			break;
		}

		// test for none-ASCII text characters
		// if c in not in the set describet ad p. 4 assignment 0.
		if((c < 0x20 && (c < 0x07 || c > 0x0D) && c != 0x1B) || c > 0x7E){
			// set size to the correct index
			ret = 2;
			break;			
		}
		
	}

	// reset file pointer
	fseek(stream, 0, SEEK_SET);
	return ret;
}


// TODO remember to add the function to the main function
// and add comment on what this function does.
int CheckForISO(FILE* stream){
	// return value reflex index of answer array.
	ret = 1;
	
	// make ISO check function
	/* 	check if check if ther exist a char c
		such that 159 < c < 255
		OBS! do not need to check for ascii chars
	*/
	
	// reset stream pointer to start of the file
	fseek(stream, 0, SEEK_SET);
	return ret;
}


// TODO remember to add the function to the main function
// and add comment on what this function does.
//
// Been worked on - Kasper

int CheckForUTF8(FILE* stream){
	// return value reflex index of answer array.
	ret = 1;
	
	// make UFT8 check function
	/*
		check for correct byte sequence
		e.i. if first bytes most significant (left most) bit
		equals 0 then there is only this byte to check
		if the two most significant bits equals 1 and 
		the third equals 0 the next byte should have
		its' two most signiciant byte set to 1 and 0.
		e.i. for 3 and 4 bytes.
	*/

	// reset stream pointer to start of the file
	fseek(stream, 0, SEEK_SET);
	return ret;
}


// TODO remember to add the function to the main function
// and add comment on what this function does.
int CheckForUTF16(FILE* stream){
	// return value reflex index of answer array.
	ret = 1;
	
	// make UFT16 check function
	// check first to bytes as described in the assignment.
	// reset stream pointer to start of the file
	fseek(stream,0,SEEK_SET);
	return ret;
}


int main(int argc, char *argv[]){
	
	FILE *stream;	
	 
	size_t size=0;




	// index to answer array.
	int index = 0;

	// Test if the program get other then one input
	if(argc < 2){
		fprintf(stdout, "Usage: ./file path");
		return EXIT_FAILURE;
	}
	
	for (int i = 1; i < argc; i++){

	
		// try to open file ("r" == readonly mode)
		stream = fopen(argv[i],"r");
		
		// test if stream is a NULL pointer aka. file reading error
		if(!stream){
			// argv[1] are the first argument of the main function, errno (from errno.h)
			// is a number code for the last tricked error.
			print_error(argv[i], errno);
			
			// jump to next i in the loop
			continue;
		}

		// gets size of the file ftell returns the displacement of the pointer in bytes,
		// from the start of the stream
		fseek(stream, 0, SEEK_END);
		size = ftell(stream);

		// reset stream pointer to head of the stream.
		fseek(stream, 0, SEEK_SET);

		
		

		// test for empty file	
		// TODO Insert functions in correct order
		if(size){
			// uses function that check the stream for none ascii chars. 
			index = CheckForData(stream);
		}
		
		
		// print the correct answer
		fprintf(stdout, "%s: %s\n", argv[i], answers[index]);
		
		index = 0;
		
	}

	// return successfully
	return EXIT_SUCCESS;	
}