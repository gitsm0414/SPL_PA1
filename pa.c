#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
// include hand-implemented header file later

#define FMAX 1000000
#define IMAX 4096
#define OMAX 10000

int main(int argc, char* argv[]){
	char* inputError = "There should be only one argument.\n";
	int inputErrorNum = 35;

	size_t fmax = (size_t)FMAX;
	size_t imax = (size_t)IMAX;
	size_t omax = (size_t)OMAX;
	char* fbuf = (char*)malloc(FMAX); //file buffer
	char* ibuf = (char*)malloc(IMAX); //input buffer
	char* obuf = (char*)malloc(OMAX); //output buffer

	int fd; //file descriptor
	ssize_t bytesRead;
	ssize_t bytesWrite;
	off_t pos; //current position

	//struct stat st; //for reading file size
	//ex) meta = fstat(fd, &st);
		
	

	if(argc!=2){  // when the number of arguments is not one
		write(STDERR_FILENO, inputError, inputErrorNum); 
		exit(1);
	}

	free(fmax);
	free(imax);
	free(omax);
	return 0;
}
