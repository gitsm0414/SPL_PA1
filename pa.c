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
#define WMAX 1500 //word max
#define WMAXL 46 //word legnth max

char** words;


void cleanup(char** words){
	for(int i=0; i<WMAX; i++){
		free(words[i]);
		words[i] = NULL;
	}
	free(words);
	words = NULL;
}
void closefile(int fd){
	int ret;
	if((ret = close(fd)) < 0){
		write(STDERR_FILENO, "file closing error\n", 20);
	}
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
		words[i] = (char*)malloc(WMAXL * sizeof(char));
	}
	int wordnum; //the number of input words
	int oidx; //current index of obuf
	oidx = 0; //initialize
	
	int fd; //file descriptor	
	ssize_t bytesWrite;
	ssize_t bytesRead;
	
	int curline; //current line(file)
	int curidx; //current index(file)
	char sentence[300]; //used when case 2 & 4, representing one sentence

	int inputcase;//case 0~4
	int flag = 1; //if 0, then terminate 'while'

	if(argc!=2){  // when the number of arguments is not one, print error and exit
		write(STDERR_FILENO, inputError, inputErrorNum); 
		cleanup(words);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0){
		write(STDERR_FILENO, "file opening error\n", 20);
		cleanup(words);
		exit(1);
	}

	do{
		if((bytesRead = read(STDIN_FILENO, ibuf, imax))==-1){
			write(STDERR_FILENO, "input reading error\n", 21);
			cleanup(words);
			closefile(fd);
			exit(1);
		}
		//checking case
		inputcase = casereturn(ibuf, (int) bytesRead);
		if(inputcase==-1){
			write(STDERR_FILENO, "invald input\n", 14);
			cleanup(words);
			closefile(fd);
			exit(1);
		}

		preproc(ibuf, inputcase, (int) bytesRead, words,  &wordnum);

		switch(inputcase){
		case 0:
			flag = 0;
			break;			
		case 1:
			while((bytesRead = readBySentence(fd, fbuf, fmax)) > 0){		
				int maxidx = (int) bytesRead;
				curline = 1;
				curidx = 0;
				int length = len(words[0]);

				for(int i=0; i<(maxidx-length+1); i++){
					if(fbuf[i]=='\n'){
						curline++;
						curidx=0;
						continue;
					}
					if(isstart(fbuf, i)){
						if(eqword(fbuf, i, maxidx, words[0])){
							addlineidx(obuf, curline, curidx, &oidx);
						}
					}
					curidx++;
				}
			}
			if(bytesRead < 0){
					write(STDERR_FILENO, "file reading error\n", 20);
					cleanup(words);
					closefile(fd);
					exit(1);
			}
			break;
		case 2:
			while((bytesRead = readBySentence(fd, fbuf, fmax)) > 0){
				int maxidx = (int) bytesRead;
				curline = 1;
				curidx = 0;
				
				int sidx = 0; // sentence idx
				int bufidx = 0; //buffer idx
				
				while(bufidx < maxidx){
					if((fbuf[bufidx]=='\n') ||(bufidx+1 == maxidx)){
						sentence[sidx]='\n';

						int ret;
						ret = case2f(fbuf, words, &wordnum, sentence, sidx+1);
						
						if(ret == 1){
							addline(obuf, curline, &oidx);
						}
						bufidx++;
						sidx = 0;
						curline++;
					}
					else{
						sentence[sidx++] = fbuf[bufidx++];
					}
				}			
			}
			if(bytesRead < 0){
					write(STDERR_FILENO, "file reading error\n", 20);
					cleanup(words);
					closefile(fd);
					exit(1);
			}
			break;
		case 3:
			while((bytesRead = readBySentence(fd, fbuf, fmax)) > 0){
				int maxidx = (int) bytesRead;
				curline = 1;
				curidx	= 0;
				int length = len(words[0]);

				for(int i=0; i<(maxidx-length+1);i++){
					if(fbuf[i]=='\n'){
						curline++;
						curidx=0;
						continue;
					}
					if(fbuf[i+length-1]=='\n'){
						i = i + length - 1;
						curline++;
						curidx = 0;
						continue;
					}
					if(isstart(fbuf, i)){
						if(eqphrase(fbuf, i, maxidx, words[0])){
							addlineidx(obuf, curline, curidx, &oidx);
						}	
					}
					curidx++;
				}
			}
			if(bytesRead < 0){
					write(STDERR_FILENO, "file reading error\n", 20);
					cleanup(words);
					closefile(fd);
					exit(1);
			}	
			break;
		case 4:
			while((bytesRead = readBySentence(fd, fbuf, fmax)) > 0){
				
			}
			if(bytesRead < 0){
					write(STDERR_FILENO, "file reading error\n", 20);
					cleanup(words);
					closefile(fd);
					exit(1);
			}
			break;
		default:
		}
		if(flag!=0){
			//output buffer preprocessing
			if(oidx==0){
			       	obuf[oidx]='\n';
				oidx++;
			} 
			else obuf[oidx-1]='\n'; 

			//print output
			if((bytesWrite = write(STDOUT_FILENO, obuf, (size_t)oidx))==-1){
				write(STDERR_FILENO, "output error\n", 14);
				cleanup(words);
				closefile(fd);
				exit(1);
			}
			//variable reinitializing
			oidx=0;
			lseek(fd, 0, SEEK_SET);
		}

	}while(flag!=0);

	cleanup(words);
	return 0;
}
