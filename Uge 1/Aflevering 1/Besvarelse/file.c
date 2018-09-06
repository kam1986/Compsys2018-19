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

// adds line terminator comment if endings == 0.
char* Linecomment(int endings){
	if(endings == 0){
		return ", with no line terminators";
	}
	return "";
}

int main(int argc, char *argv[]){
	
	FILE *stream;	
	int newlines = 0;
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
	

	// test for open file error.
	if(!stream){
		print_error(argv[1],errno);
	}
	
	// gets size of the file
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);	
	fseek(stream, 0, SEEK_SET);
	

	// test for empty file	
	if(size){
		// assume that the file is ASCII.
		size = 1;
		
		// use do while instead since we at least once need to check c.
		do{
            
			c = fgetc(stream);
			
			// testing for end of file
			if(c == EOF){
				break;
			}

			// testing for lineterminators
			// 10 = 0000 1010 = LF (NL line feed, newline)
			// 13 = 0000 1101 = CR (carriage return)
			if(c == (char) 10 || c == (char) 13){
				newlines++;
			}

			// test for none-ASCII text characters
			// (20 < c < 126 are printable chars)
			if((32 > c || c > 126) && c != 13 && c != 10){
				// set size to the correct index
				size = 2;
				// prevent line termination comment.
				newlines++;
				break;			
			}
			
		}while(1);
	}
	else{
		newlines++;
	}	
	
	// print the correct answer
	fprintf(stdout, "%s: %s%s\n", argv[1], answers[size], Linecomment(newlines));

	// return successfully
	return EXIT_SUCCESS;	
}