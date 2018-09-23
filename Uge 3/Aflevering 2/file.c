#include<stdio.h> 	// fprintf, stdout, stderr.
#include<stdlib.h>	// exit, EXIT_FAILURE, EXIT_SUCCESS.
#include<string.h>	// strerror.
#include<errno.h>	// errno.

// an array were we can index the given answer
char* answers[7] = {
	"empty", 
	"data",
	"ASCII text",
	"ISO-8859",
	"UTF-8 Unicode",

	// endian order determenes te order in which bits are read.
	// e.i. left ot right or right to left.
	"Little-endian UTF-16 Unicode",
	"Big-endian UTF-16 Unicode"
};

enum Type {
	EMPTY = 0,
	DATA = 1,
	ASCII = 2,
	ISO = 3,
	UTF8 = 4,
	LITTLE_UTF16 = 5,
	BIG_UTF16 = 6,
};


// print correct error for handling corrupt and missing files.
int print_error(char *path, int errnum){
	return fprintf(stdout, "%s: cannot determine (%s)\n",
			// strerror takes a error code number and return a standard error string
		path, strerror(errnum));
}


// check for none ascci data charachters
int CheckForAscii(FILE* stream){
	char c;

	// assume ascii
	int ret = ASCII;

	while(1){
				
		c = fgetc(stream);

		// breaks loop if we get a eof char (End Of File).
		if(c==EOF) {
			break;
		}

		// test for none-ASCII text characters
		// if c in not in the set describet ad p. 4 assignment 0.
		if((c < 0x20 && (c < 0x07 || c > 0x0D) && c != 0x1B) || c > 0x7E){
			// prooved not ascii
			ret = DATA;
			break;			
		}
		
	}

	// reset file pointer
	fseek(stream,0,SEEK_SET);
	return ret;
}

int CheckForISO(FILE* stream){
	// return value reflex index of answer array.
	unsigned char c;

	// assume ascii
	int ret = ISO;

	while(1){
				
		c = fgetc(stream);

		// breaks loop if we get a eof char (End Of File).
		if(feof(stream)) {
			break;
		}

		// test for none-ISO text characters
		// if c in not in the set describet ad p. 4 assignment 0.
		if((c < 0x20 && (c < 0x07 || c > 0x0D) && c != 0x1B)
			|| (0x80 < c && c < 0xA0)){
			// prooved not ISO
			ret = DATA;
			break;			
		}
		
	}

	// reset file pointer
	fseek(stream,0,SEEK_SET);
	return ret;
}

int CheckForUTF8(FILE* stream){
	// return value.
	int ret = UTF8;

	// String for searching the file.
	// (4 char + 1 string terminator) 
	char str[5] = { 0 };

	while(ret == UTF8){
		// read the next 4 char of the string
		// increament the stream pointer by 4 bytes
		fgets(str, 4, stream);
		

		if(feof(stream)){
			// need to handle end of file
			break;
		}
		// set the last 3 bits to 0
		// e.i (0xf8 base 16 = 11111000 base 2)
		unsigned char fsttest = str[0] & 0xf8;
		// ex. 11101111 & 11111000 = 11101000
		// if any of the 5 left bit in str[0] are 0,
		// then the result is 0 for that bit
		// the 5 bit from left must be checked if 1
		
		// set the 6 most right bits to 0
		// and the 2 most left to either 1 or 0
		// 0xc0 base 16 = 11000000 base 2
		unsigned char sectest = str[1] & 0xc0;
	 	unsigned char thdtest = str[2] & 0xc0;
		unsigned char fthtest = str[3] & 0xc0;

		// 0x80 base 16 = 10000000 base 2
		// if any of the three above chars has the
		// bit sequence 1100000 the test will return a 
		// none-valid test with value 0xc0 where a valid sequence 
		// will have 10000000 with value 0x80
		

		switch(fsttest){

			// case 4
			// case 11110000 base 2
			case 0xf0:
				// if not all bytes to most left bits are 10 base 2
				if((sectest | thdtest | fthtest) != 0x80){
				 	ret = DATA;
				}
				break;
			
			// case 3
			// case 11101000 base 2 
			case 0xe8:
			// case 11100000 base 2
			case 0xe0:
				// if not all bytes to most left bits are 10 base 2
				if((sectest | thdtest ) != 0x80){
				 	ret = DATA;

					break;
				}
				// set the stream pointer 1 byte back.
				fseek(stream, -1,SEEK_CUR);
				break;

			// cases 2
			// case 11011xxx base 2
			case 0xd8:
			// case 11010xxx base 2
			case 0xd0:
			// case 11001xxx base 2
			case 0xc8:
			// case 11000xxx base 2
			case 0xc0:
				// if not 10xxxxxx base 2
				if(sectest != 0x80){
				 	ret = 1;

				 	ret = DATA;

					 break;
				}
				// set the stream pointer 2 bytes back.
				fseek(stream, -2,SEEK_CUR);
				break;

						
			default:
				// case 0xxxxxxx base 2
				if((unsigned char)str[0] < 0x80 && !0x00){
					fseek(stream, -3, SEEK_CUR);
					break;
				}
				// all other case are faults 
				ret = DATA;
				break;
		}
	}

	// reset stream pointer to start of the file
	fseek(stream,0,SEEK_SET);
	return ret;
}


int CheckForUTF16(FILE* stream){
	// return value reflex index of answer array.
	int ret = DATA;
	
	char c1 = fgetc(stream);
	char c2 = fgetc(stream);

	switch(c1){

		case '\xFF':
		if(c2 == '\xFE'){
			ret = LITTLE_UTF16;
		}
		break;

		case '\xFE':
		if(c2 == '\xFF'){
			ret = BIG_UTF16;
		}
		break;
	}
	
	
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
			// testing against UFT16 files, and setting index to
			// either to DATA or UFT16 (big or little endian)
			index = CheckForUTF16(stream);
			
			// assuming that the file are a data file
			// try to disprove it, trough testing.
			if(index == DATA){

				// uses function that check the stream for none ascii chars.
				index = CheckForAscii(stream);
			}

			if(index == DATA){
				index = CheckForUTF8(stream);
			}
			
			if(index == DATA){

				index = CheckForISO(stream);
			}
		}
		
		
		// print the correct answer
		fprintf(stdout, "%s: %s\n", argv[i], answers[index]);
		
		index = 0;
		
	}

	// return successfully
	return EXIT_SUCCESS;	
}