#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include "myheader.h" //hand-implemented headerfile

#define FMAX 1000000
#define IMAX 4096
#define OMAX 10000
#define WMAX 1000 //word max
#define WMAXL 46 //word legnth max

char* fbuf; //file buffer
char* ibuf; //input buffer
char* obuf; //output buffer
char** words; //input words

int main(int argc, char* argv[]){
	char* inputError = "There should be only one argument.\n";
	int inputErrorNum = 35;

	size_t fmax = (size_t)FMAX;
	size_t imax = (size_t)IMAX;
	size_t omax = (size_t)OMAX;
	char* fbuf = (char*)malloc(FMAX*sizeof(char));
	char* ibuf = (char*)malloc(IMAX*sizeof(char));
	char* obuf = (char*)malloc(OMAX*sizeof(char));
	
	char** words = (char**)malloc(WMAX*sizeof(char*));
	for(int i=0; i<WMAX; i++){
		char* words[i] = (char*)malloc(sizeof(char)*WMAXL);

	int fd; //file descriptor
	ssize_t bytesRead;
	ssize_t bytesWrite;
	off_t pos; //current position
	
	int curline; //current line(file)
	int curidx; //current index(file)
	
	

	if(argc!=2){  // when the number of arguments is not one
		write(STDERR_FILENO, inputError, inputErrorNum); 
		exit(1);
	}

	free(fmax);
	free(imax);
	free(omax);
	return 0;
}
