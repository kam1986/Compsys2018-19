#include<stdio.h> 	// fprintf, stdout, stderr.
#include<stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include<string.h>	// strerror.
#include<errno.h>	// errno.


int main(int argc, char *argv[]){
	
	FILE *stream;	
	
	char c;
	char* answers[3] = {
		"Empty: empty\n",
		"ascii: ASCII text\n",
		"data: data\n",
		
	};

	
	size_t size=0;
	
	// Test if the program get more then one input
	// TODO - write the correct message. 	
	if(argc != 2){
		printf("some comment");
		return EXIT_FAILURE;
	}
	
	// try to open file
	stream = fopen(argv[1],"r");
	

	// TODO - use errno.h lib to test if the file exist
	//      - and if it can be read.
	//		- write the correct messeges to the stdout.

	
	// gets size of the file
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);	
	

	// test for empty file	
	if(size){
		fseek(stream, 0, SEEK_SET);
		// assume that the file is ASCII.
		size = 1;
		
		// TODO - does not work!
		do{
            
			c = fgetc(stream);
			if(feof(stream)){
				break;
			}
			
			// test for none-ASCII text characters
			// (DEL and char with values < 20 are none text)
			if(c < 20 || c > 126){
				size = 2;
				break;			
			}
			
		}while(feof(stream));
	}	
	
	// print the correct answer
	printf(answers[size]);

	// return successfully
	return EXIT_SUCCESS;	
}