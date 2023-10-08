#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include "myheader.h" //hand-implemented headerfile

#define FMAX 1000000 //file buf max
#define IMAX 4096 //input buf max
#define OMAX 50000 //output buf max
#define WMAX 1000 //word max
#define WMAXL 46 //word legnth max

char** words;

void cleanup(void){
	for(int i=0; i<WMAX; i++){
		free(words[i]);
	}
	free(words);
}

int main(int argc, char* argv[]){
	char* inputError = "There should be only one argument.\n";
	int inputErrorNum = 35;
	
	char fbuf[FMAX]; //file buffer
	char ibuf[IMAX]; //input buffer
	char obuf[OMAX]; //output buffer
	size_t fmax = (size_t)FMAX;
	size_t imax = (size_t)IMAX;
	size_t omax = (size_t)OMAX;
	
	//memory allocating, words
	words = (char**)malloc(WMAX * sizeof(char*));
	for(int i=0; i<WMAX; i++){
		words[i] = (char*)malloc(WMAXL * sizeof(char))
	}
	int wordnum; //the number of input words
	int oidx; //current index of obuf
	oidx = 0; //initialize
	
	int fd; //file descriptor
	ssize_t bytesWrite;
	ssize_t bytesRead;
	off_t pos; //current position of file
	
	int curline; //current line(file)
	int curidx; //current index(file)
	
	int inputcase;//case 0~4
	int flag = 1; //if 0, then terminate 'while'

	atexit(cleanup);

	if(argc!=2){  // when the number of arguments is not one, print error and exit
		write(STDERR_FILENO, inputError, inputErrorNum); 
		exit(1);
	}

	do{
		if((bytesRead = read(STDIN_FILENO, ibuf, imax))==-1){
			write(STDERR_FILENO, "input reading error\n", 21);
			exit(1);
		}
		//checking case
		inputcase = casereturn(ibuf, (int) bytesRead);
		if(inputcase==-1){
			write(STDERR_FILENO, "invald input\n", 14);
			exit(1);
		}

		preproc(ibuf, inputcase, (int) bytesRead, words,  &wordnum);

		switch(inputcase){
			case 0:
				flag = 0;
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			default:
		}
		if(flag!=0){
			//output buffer preprocessing
			if(oidx==0) obuf[oidx]='\n'; 
			else obuf[oidx-1]='\n'; 
		
			//print output
			if((bytesWrite = write(STDOUT_FILENO, obuf, (size_t)oidx))==-1){
				write(STDERR_FILENO, "output error\n", 14);
				exit(1);
			}
			//variable reinitializing
			oidx=0;
			//lseek(fd, 0, SEEK_SET);
		}

	}while(flag!=0);

	cleanup();
	return 0;
}
