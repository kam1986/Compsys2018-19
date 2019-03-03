#include <stdio.h> 	// fprintf, stdout, stderr.
#include <stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h>	// strerror.
#include <errno.h>	// errno.

#define UTF8_CONT(value) (((value & 0xc0) == 0x80) ? 1 : 0) // continuation byte
#define UTF8_2B(value)   (((value & 0xe0) == 0xc0) ? 1 : 0) // 2-byte
#define UTF8_3B(value)   (((value & 0xf0) == 0xe0) ? 1 : 0) // 3-byte
#define UTF8_4B(value)   (((value & 0xf8) == 0xf0) ? 1 : 0) // 4-byte


// An array where we can index the given answer
const char * const FILE_TYPES[] = {
    "data",                                //0
    "empty",                               //1
    "ASCII text",                          //2
    "ISO-8859 text",                       //3
    "UTF-8 Unicode text",                  //4
	"Little-endian UTF-16 Unicode text",   //5
	"Big-endian UTF-16 Unicode text",      //6
};


// The CheckForEmpty function check for, if the the flie is empty.
int CheckForEmpty(FILE* stream){

	fseek(stream, 0, SEEK_END);
	int size = ftell(stream);

	if(size == 0){
		return 1;
	}
	else
		return 0;
}


// The CheckForAscii function checks for non-ASCII data characters.
// If none is found, it will return 2, which means that the file is ASCII.
int CheckForAscii(FILE* stream){
	int c;

	// Assume ASCII
	int ret = 2;

	// reset file pointer
	fseek(stream, 0, SEEK_SET);

	while(!feof(stream)){
		c = fgetc(stream);

		if(c != EOF){
			// Test for non-ASCII text characters.
			// If c is not in the set described on p. 3 in assignment 1.
			if(!((c >= 0x07 && c <= 0x0D) || c == 0x1B || (c >= 0x20 && c <= 0x7E))){
				// prooved not ascii
				ret = 0;
				break;
			}	
		}
	}
	return ret;
}


// The CheckForISO function checks if the file contain non-ISO-8859-1-like bytes.
// If none has been found, it will return 3, which means the file is an ISO file. 
int CheckForISO(FILE* stream){
	int c;
	// Assume ISO
	int ret = 3;

	// Reset file pointer
	fseek(stream, 0, SEEK_SET);

	while(!feof(stream)){
		c = fgetc(stream);

		if(c != EOF) {
			// Test for non-ISO text characters.
			// If c is not in the set descriped on p. 4 in assignment 1.
			// This includes all ASCII-like bytes (as the function above), as well as
			// decimal value 160 - 255. Where the decimal 160 = 0xA0 in Hex and 
			// 255 = 0xFF in Hex.
			if(!((c >= 0x07 && c <= 0x0D) || c == 0x1B || (c >= 0x20 && c <= 0x7E) || 
				(c >= 0xA0 && c <= 0xFF))){
				// Prooved not ISO
				ret = 0;
				break;
			}	
		}
	}
	return ret;
}


// The CheckForUTF8 function checks for, if the file is encoded in UTF-8.
// The function will return 4 if the file is using UTF-8-like characters.
int CheckForUTF8(FILE* stream){
	int c;
	// Assume UTF8
	int ret = 0;

	// Reset file pointer
	fseek(stream, 0, SEEK_SET);

	// This will check for, if the file fits into any kind of number of bytes
	// by checking its designated bit-sequence.
	while(!feof(stream)){
		c = fgetc(stream);
		int c2 = c+1;
		int c3 = c+2;
		int c4 = c+3;

		// We have the following information from the table in the assignment at page 4,
		// that summarises the encoding:
		// 0xC0 base 16 = 11000000 base 2
		// 0x80 base 16 = 10000000 base 2
		// 0xE0 base 16 = 11100000 base 2
		// 0xF0 base 16 = 11110000 base 2
		//
		// So, according to the table, we have that:
		// 10xxxxxx  0x80..0xBF   Continuation bytes (1-3 continuation bytes)
		// 110xxxxx  0xC0..0xDF   First byte of a 2-byte character encoding
		// 1110xxxx  0xE0..0xEF   First byte of a 3-byte character encoding
		// 11110xxx  0xF0..0xF7   First byte of a 4-byte character encoding
		//
		// We use these informations to check for, if the file does fit 
		// into any of the number of bytes, where the possible representations 
		// for characters are:
		// 0xC0
		// 0xC0 0x80
        // 0xE0 0x80 0x80
        // 0xF0 0x80 0x80 0x80:
		if(c != EOF) {
			// Number of bytes = 1
			if((c & 0xC0) == 0xC0){
				ret = 4;
			}
			// Number of bytes = 2
			else if(((c & 0xC0) == 0xC0) && (((c2) & 0x80) == 0x80)){
				ret = 4;
			}
			// Number of bytes = 3
			else if(( (c & 0xE0) == 0xE0) && (((c2) & 0x80) == 0x80) && 
				(((c3) & 0x80) == 0x80)){
				ret = 4;
			}
			// Number of bytes = 4
			else if(((c & 0xF0) == 0xF0) && (((c2) & 0x80) == 0x80) && 
				(((c3) & 0x80) == 0x80) && (((c4) & 0x80) == 0x80)){
				ret = 4;
			}
		}
	}
	return ret;
}


// The CheckForUTF16 function checks if the file is encoded in UTF-16.
// The function will return 5, which means the file is using Big-endian UTF-16 Unicode text,
// while if the function returns 6, it means the file is using Little-endian UTF-16 Unicode text.
int CheckForUTF16(FILE* stream){
	// return value reflex index of answer array.
	int ret = 0;
	fseek(stream, 0, SEEK_SET);

	int fst = fgetc(stream);
	int snd = fgetc(stream);

	// Little-endian UTF-16 Unicode text:
	// This will check if the file begins with a byte-order-mark (BOM):
	// \0xFF \0xFE
  	if ((fst == 0xFF) && (snd == 0xFE)) {
    	ret = 5;
  	}
	// Big-endian UTF-16 Unicode text:
	// This will check if the file begins with a byte-order-mark (BOM):
	// \0xFE \0xFF
	else if ((fst == 0xFE) && (snd == 0xFF)) {
    	ret = 6;
  	}
  	return ret;
}


// The following function checks the file type and 
// will return the corresponding number. 
int CheckForType(FILE* stream){
	
	if (stream == NULL) {
		return -1;
	}
	// An empty file, is index 1.
	else if(CheckForEmpty(stream) == 1){
		return 1;
	}
	// An ASCII file type, is index 2.
	else if(CheckForAscii(stream) == 2){
		return 2;
	}
	// An ISO file type, is index 3.
	else if(CheckForISO(stream) == 3){
		return 3;
	}
	// A Little-endian UTF-16 file type, is index 5.
	else if(CheckForUTF16(stream) == 5){
		return 5;
	}
	// A Big-endian UTF-16 file type, is index 6.
	else if(CheckForUTF16(stream) == 6){
		return 6;
	}
	// An UTF-8 file type, is index 4.
	else if(CheckForUTF8(stream) == 4){
		return 4;
	}
	// A data file type, is index 0.
	else
		return 0;
}


int main(int argc, char *argv[]) {
	FILE *stream;
	
	if (argc >= 2){
		// (using the same method if a correct path to existing file is given)
		// Aligning output corresponding to the longest name.
    	unsigned long max_length = strlen(argv[1]);

    	for (int i = 1; i < argc; i++) {
    		if (max_length < strlen(argv[i-1])){
    			max_length = strlen(argv[i-1]);
    		}
    	}

    	int j = 1;
    	while(j < argc){
    		stream = fopen(argv[j],"r");

			// Using hint for code to make the following behaviour (given to us in the task from section 1.2, at page 3): 
			// If we provide a path to a non-existing file, or some other I/O error occurs:
			if(CheckForType(stream) == -1){
    			// If our program cannot find a file, it will give an error.
        		fprintf(stdout, "%s:%*scannot determine(%s)\n", argv[j], 
          			(int)((max_length+1) - strlen(argv[j])), " ", strerror(errno));
			}
			else{
				// Using code given to us at page 2 in the assignment, to get following behaviour:
				// If the given argument(s) is a path to a file that exists, and the type of that 
				// file can be determined, write one line of text to stdout.
				// Where it will consist of: the given path: spaces and a guess of the file type.
				// I.e. it guesses the file type, using the CheckForType function defined above.
				fprintf(stdout,"%s:%*s%s\n", argv[j], (int)((max_length+1) - strlen(argv[j])),
					" ", FILE_TYPES[CheckForType(stream)]);  
        	
				fclose(stream); // Closing stream (closing file)
    		}
      		j++;
    	}
    	return EXIT_SUCCESS; 
	}
	else{
    	// If our program have not chosen any file it will print a usage error:
    	fprintf(stderr, "Usage: %s path\n", argv[0]);
    	
		return EXIT_FAILURE;
  	}
}