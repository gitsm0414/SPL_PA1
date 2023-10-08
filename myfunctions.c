//my functions
#include "my.h"
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
/*
 * function: readBySentence
 * This function read file by sentence.
 * First call read(), if the result of calling read() is inturruption of the last sentence in the usrbuf, then undo reading one char by one until usrbuf is ended by complete sentence. And this function alse hadle errors and return -1. else the return value is the number of bytes read.
 */
ssize_t readBySentence(int fd, char* usrbuf, size_t n){
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;
	int idx = 0;

	while(nleft > 0){
		if((nread = read(fd, bufp, nleft)) < 0){
			if( errno == EINTR)
				nread = 0;
			else
				return -1;
		}
		else if(nread == 0) break;
		nleft -= nread;
		bufp += nread;
		idx += nread;
	}
	if(nleft == n) return 0; //if there is no bytes read, then return 0

	if(usrbuf[idx-1]!='\n'){ //if last char of usrbuf is not '\n'
		if(idx == n){ //last sentence of usrbuf was interrupted by buffer limit
			int i = idx-1;
			while(usrbuf[i]!='\n'){
				lseek(fd, -1, SEEK_CUR)
				nleft++;
				i--;
			}
		} //else is when reading process reached EOF
	}

	return (ssize_t) (n - nleft);
}
