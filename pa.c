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

char* fbuf; //file buffer
char* ibuf; //input buffer
char* obuf; //output buffer
char** words; //input words
int wordnum; //the number of input words
ssize_t bytesRead;
int oidx; //current index of obuf

void cleanup(void){
	if(fbuf!=NULL) free(fbuf);
	if(ibuf!=NULL) free(ibuf);
	if(obuf!=NULL) free(obuf);

	for(int i=0; i<WMAX; i++){
		if(words[i]!=NULL) free(words[i]);
	}
	if(words!=NULL) free(words);
}

int main(int argc, char* argv[]){
	char* inputError = "There should be only one argument.\n";
	int inputErrorNum = 35;

	size_t fmax = (size_t)FMAX;
	size_t imax = (size_t)IMAX;
	size_t omax = (size_t)OMAX;
	fbuf = (char*)malloc(FMAX*sizeof(char));
	ibuf = (char*)malloc(IMAX*sizeof(char));
	obuf = (char*)malloc(OMAX*sizeof(char));
	oidx = 0; //initialize

	//memory allocating of 'words'
	words = (char**)malloc(WMAX*sizeof(char*));
	for(int i=0; i<WMAX; i++){
		words[i] = (char*)malloc(sizeof(char)*WMAXL);
	}
	int fd; //file descriptor
	ssize_t bytesWrite;
	off_t pos; //current position of file
	
	int curline; //current line(file)
	int curidx; //current index(file)
	
	int inputcase;//case 0~4
	int flag = 1; //if 0, then terminate 'while'

	atexit(cleanup); //cleanup when exit

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
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				flag = 0;
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
	}

	}while(flag!=0);

	cleanup(); //free the memories

	return 0;
}
