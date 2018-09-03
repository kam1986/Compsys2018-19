#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Wrong number of arguments\n");
		return EXIT_FAILURE;	
	}

	printf("%s\n%s\n",argv[1],argv[1]);

	return EXIT_SUCCESS;
}