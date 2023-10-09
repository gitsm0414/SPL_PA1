//my functions
#include "myheader.h"
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

/*
 * function: readBySentence
 * This function read file by sentence.
 * First call read(), if the result of calling read() is interruption of the last sentence in the usrbuf, then undo reading one char by one until usrbuf is ended by complete sentence. And this function alse hadle errors and return -1. else the return value is the number of bytes read.
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
			while((usrbuf[i]!='\n') && (i > 1)){
				lseek(fd, -1, SEEK_CUR);
				nleft++;
				i--;
			}
		} //else is when reading process reached EOF
	}

	return (ssize_t) (n - nleft);
}

//return string length
int len(char* ptr){
	char* temp = ptr;
	int i=0;
	while(*temp != '\0'){
		i++;
		temp++;
	}
	return i;
}

//if buf[idx] is blank, return 1, else 0
int isblank_(char* buf, int idx){
	if(buf[idx]==' ' || buf[idx]=='\t' || buf[idx]=='\n') return 1;
	else return 0;
}

//if idx is start of the word, return 1, else 0
int isstart(char* buf, int idx){
	if(idx == 0){
		if(isblank_(buf, 0)) return 0;
		else return 1;
	} 
	if(isblank_(buf, idx-1)){
		if(isblank_(buf, idx)) return 0;
		else return 1;
	}
	return 0;
}

//if buf has equal word strating from idx, then return 1, else 0
int eqword(char* buf, int idx, int maxidx,  char* word){
	char* wordp = word;
	char buf_char;
	char word_char;
	while(*wordp!='\0' && (idx < maxidx)){
		//if uppercase, change to lowercase
		buf_char = buf[idx];
		word_char = *wordp;
		if(buf_char<='Z') buf_char += ('a' - 'A');
		if(word_char<='Z') word_char += ('a' - 'A');

		if(buf_char != word_char) return 0;
		idx++;
		wordp++;
	}
	if((*wordp=='\0') && (idx==maxidx)) return 1;
	if(idx==maxidx) return 0;

	if(isblank_(buf, idx)) return 1;
	return 0;
}
/*
 * function: casereturn
 * This function returns case number 1~4, if input was 'PA1EXIT', then return 0,
 * else, error, return -1
 */
int casereturn(char* buf, int maxidx){
	if(maxidx <= 0) return -1;

	if(buf[0]!='\"'){
		char* ex = "PA1EXIT\n";
		int eq;
		if((eq = eqword(buf, 0, maxidx, ex))==-1) return -1;
		if(eq==1) return 0;
	}
	if(buf[0]=='\"') return 3;
	
	for(int i=0; i<(maxidx-1); i++){
		if(buf[i]=='*') return 4;
	}
	for(int i=0; i<(maxidx-1); i++){
		if(isblank_(buf, i)) return 2;		
	}
	return 1;
}

// copy word(from buf to word)
void cp(char* buf, int idx, int maxidx, char* word){
	char* wordp = word;
	int blk;
	while(((blk = isblank_(buf, idx)) != 1) && (idx<maxidx)){
		*wordp = buf[idx];
		idx++;
		wordp++;
	}
	*wordp = '\0'; // all word ends '\0'
}
// copy phrase
void cpph(char* buf, int idx, int maxidx, char* word){
	char* wordp = word;
	
	while(((buf[idx]!='\"') && (idx<maxidx))){
		*wordp = buf[idx];
		idx++;
		wordp++;
	}
	*wordp = '\0'; // all word ends '\0'
}

/*
 * function: preproc
 * This function preprocess the input buffer according to case number. 
 */
void preproc(char* buf, int casenum, int maxidx,  char** word, int* wordn){
	switch(casenum){
		case 0: //pa1exit, do nothing
			break;
		case 1:
			cp(buf, 0, maxidx, word[0]);
			*wordn = 1;
			break;
		case 2:
			*wordn = 0;
			for(int i=0; i<maxidx; i++){
				if(isstart(buf, i)){
					cp(buf, i, maxidx, word[*wordn]);
					*wordn = *wordn + 1;
				}
			}
			break;
		case 3:
			cpph(buf, 1, maxidx, word[0]);
			*wordn = 1;
			break;
		case 4:
			int star;//index of *
			for(int i=0; i<maxidx; i++){
				if(buf[i]=='*') star = i;
			}

			cp(buf, 0, star, word[0]);
			cp(buf, star+1, maxidx, word[1]);
			*wordn =2;
			break;
		default:	
	}	
}

/*
 * function: eqphrase
 * This function returns 1 when buf(starting from index i) has same phrase with word[]
 * else, return 0
 */
int eqphrase(char* buf, int idx, int maxidx, char* word){
	char widx =0; //word idx
	char buf_char;
	char word_char;
	while((word[widx]!='\0')&&(idx<maxidx)){
		//both are not blank
		if((isblank_(buf, idx)==0)&&(isblank_(word, widx)==0)){
			//if uppercase, change to lower case
			buf_char = buf[idx];
			word_char = word[widx];
			if(buf_char<='Z') buf_char += ('a' - 'A');
			if(word_char<='Z') word_char += ('a' - 'A');

			if(buf_char != word_char) return 0;		
		}
		else if((isblank_(buf, idx))*(isblank_(word, widx))==1){ //both are blank
			if(buf[idx]=='\n' || word[widx]=='\n') return 0;
			if(buf[idx] != word[widx]) return 0;
		}
		else{
			return 0;	
		}
		widx++;
		idx++;
	}
	if((word[widx]=='\0') && (idx == maxidx)) return 1;
	if(idx == maxidx) return 0;

	if(isblank_(buf, idx)) return 1;
	return 0;
}

/*
 * function: wordexists
 * This function return 1 when there exist at least one word between the given idx
 * If there is no word between them, return 0
 * start idx is the next of the word1, end idx is one before the start of word2 
 */
int wordexists(char* buf, int start, int end){
	if((start - end) >= -1) return 0;
	if(isblank_(buf, start)==0 || isblank_(buf, end)==0) return 0;
	
	int i=start+1;
	while(i<end){
		if(isstart(buf, i)) return 1;
	}
	return 0;
}

//reverse string
void reverseStr(char* str, int length){
	int start = 0;
	int end = length-1;
	char temp;
	while(start < end){
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

//convert int to string, except '\0', and return the number's max place
int its(char* str, int num){
	int i = 0;
	if(num==0){
		str[0]='0';
		return 1;
	}

	while(num!=0){
		int digit = num % 10;
		str[i] = digit + '0';
		i++;
		num /=10;
	}

	reverseStr(str, i);
	return i;
}

/*
 * function: addline
 * This function add string [line number] to buf(output buf), and adjust oidx(representing the next position to add)
 * The last position is always blank space
*/
void addline(char* buf, int curline, int* oidx){
	char str[15];
	int max;
	max = its(str, curline);
	
	for(int i=0; i<max; i++){
		buf[*oidx] = str[i];
		*oidx = *oidx + 1;
	}
	buf[*oidx] = ' ';
	*oidx = *oidx + 1;
}
/*
 * function: addlineidx
 * This function add string [line number:index] to buf(output buf), and adjst oidx
 * The last position is alwyas blank space
 */
void addlineidx(char* buf, int curline, int curidx, int* oidx){
	addline(buf, curline, oidx);
	buf[*oidx-1]=':';
	addline(buf, curidx, oidx);
}


