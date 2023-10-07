#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
// include hand-implemented header file later

#define FMAX 1000000
#define IMAX 4096

int main(int argc, char* argv[]){
	char* inputError = "There should be only one argument.";
	int inputErrorNum = 35;

	size_t fmax = (size_t)FMAX;
	size_t imax = (size_t)IMAX;
	char fbuf[FMAX]; //file buffer
	char ibuf[IMAX]; //input buffer
		
	int fd; //file descriptor
	ssize_t bytesRead;
	ssize_t bytesWrite;

	struct stat st; //for reading file size
	//meta = fstat(fd, &st);
	
	

	if(argc!=2){  // when the number of arguments is not one
		write(STDOUT_FILENO, inputError, inputErrorNum); 
		exit(1);
	}


	return 0;
}
