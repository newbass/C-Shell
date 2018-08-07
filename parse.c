#include "parse.h"

#define BUFFERSIZE 2000
int  parse(char *line, char **argv)
{
	int count=0;
	while (*line != '\0') {       // if not the end of line .......
//		printf("*line:%c\n",*line);
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     // replace white spaces with 0
//		printf("line:%s\n",line);
		*argv++ = line;		// save the argument position     
		count++;
		
		while (*line != '\0' && *line != ' ' && 
				*line != '\t' && *line != '\n') 
			line++;             // skip the argument until ...
	}
	*argv = '\0';                 // mark the end of argument list 
	return count;
}
/*
int main()
{
	char line[1024]={0};
	char *argv[64];
	strcpy(line,"   cp   abc   xyz &  ");
	parse(line,argv);
	int i=0;
	for(i=0;argv[i]!=NULL;i++)puts(argv[i]);
	return 0;
}
*/
//http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
