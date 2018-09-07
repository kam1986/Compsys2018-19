#include<stdio.h> 	// fprintf, stdout, stderr.
#include<stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include<string.h>	// strerror.
#include<errno.h>	// errno.

// an array were we can index the given answer
char* answers[4] = {
	"empty", 
	"very short file (no magic)",
	"ASCII text",
	"data",
};


// print correct error for handling corrupt and missing files.
int print_error(char *path, int errnum){
	return fprintf(stdout, "%s: cannot determine (%s)\n",
		path, strerror(errnum));
}

// adds line terminator comment.
char* Linecomment(int cr, int lf){
	if(lf == 0 && cr > 0){
		return ", with CR line terminators";
	}
	if(lf > 0 && cr > 0){
		return ", with CR, LF line terminators";
	}
	if(lf == 0 && cr == 0){
		return ", with no line terminators";
	}
	return "";
}

// adds escape sequence comment.
char* EscapeComment(int es){
	if(es){
		return ", with escape sequences";
	}

	return "";
}

// adds overstriking comment.
char* overstrikComment(int os){
	if(os){
		return ", with overstriking";
	}

	return "";
}

int main(int argc, char *argv[]){
	
	FILE *stream;	
	int cr = 0;
	int lf = 0;
	int os = 0;
	int es = 0;
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
	// errno = 2 none-existing files
	// errno = 13 no read permition.
	if(!stream){
		switch (errno) {
		case 13:
			fprintf(stdout, "%s: writable, regular file, no read permission\n", argv[1]);
			break;

		case 2:
			fprintf(stdout, "%s: cannot open `%s' (%s)\n", argv[1], argv[1], strerror(errno));
			break;
		default:
			print_error(argv[1],errno);
			break;
		}
		return EXIT_FAILURE;
	}
	
	// gets size of the file
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);	
	fseek(stream, 0, SEEK_SET);

	// test for small files.
	if(size ==1){
		fprintf(stdout, "%s: very short file (no magic)\n", argv[1]);
		return EXIT_SUCCESS;
	}

	// test for empty file	
	if(size){
		// assume that the file is ASCII.
		size = 2;
		
		// use do while instead since we at least once need to check c.
		while(1){
            
			c = fgetc(stream);

			// testing current character against spec cases.
			switch (c) {
				case EOF:
					break;
				
				case 0x08: // Overstriking
					os++;
					
				case 0x0a: // LF - Linefeed
					lf++;

				case 0x0d: // CR - carraige return
					cr++;

				case 0x1b: // Escape sequence.
					es++;

				default:
					break:
			

			// test for none-ASCII text characters
			// (20 < c < 126 are printable chars)
			if((c < 0x20 && (c < 0x07 || c > 0x0D) && c != 0x1B) || c > 0x7E){
				// set size to the correct index
				size = 3;
				// prevent line termination comment.
				cr = 0;
				lf = 1;
				os = 0;
				es = 0;
				break;			
			}
			
		}
	}
	else{
		cr = 0;
		lf = 1;;
	}	
	
	// print the correct answer
	fprintf(stdout, "%s: %s%s%s%s\n", argv[1], answers[size], 
		Linecomment(cr,lf),EscapeComment(es),overstrikComment(os));

	// return successfully
	return EXIT_SUCCESS;	
}