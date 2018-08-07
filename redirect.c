#include <termios.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include "redirect.h"
#include "parse.h"


#define BUFFERSIZE 2000

void spacemaker(char buffer[],char mainbuf[])
{
	int k=0,i=0;
	while(buffer[i]!='\n' && buffer[i]!='\0')
	{
		//printf("%c %d\n",buffer[i],i);
		if(buffer[i]!='|' && buffer[i]!='>' && buffer[i]!='<')mainbuf[k]=buffer[i];

		else {mainbuf[k]=' '; k++; mainbuf[k]=buffer[i]; k++; mainbuf[k]=' ';}

		k++;
		i++;
	}
	return;
}

void redirect(char command[])
{
	char mainbuf[BUFFERSIZE]={0};
	spacemaker(command,mainbuf);
	char *argv[10]={0};
	int num = parse(mainbuf,argv);
	int i=0;
	int inpidx=0,outidx=0;
	
	char *args[10]={0};

	while(strcmp(argv[i],">")!=0 && strcmp(argv[i],"<")!=0)
	{
		args[i]=argv[i];
		i++;
	}

	int flagapp = 0;

	for(i=0;i<num;i++)
	{
		if(strcmp(argv[i],">")==0)outidx=i;

		if(strcmp(argv[i],">")==0 && strcmp(argv[i+1],">")==0)flagapp=1;

		if(strcmp(argv[i],"<")==0)inpidx=i;
	}

	int savedin,savedout;

	if(inpidx>0)
	{
		if(open(argv[inpidx+1],O_RDONLY)==-1)
			{printf("Input file doesn't exist\n");return;}
		int in = open(argv[inpidx+1],O_RDONLY);
		savedin = dup(0);
		dup2(in,STDIN_FILENO);
		close(in);
	}

	if(outidx>0)
	{
		int out;
		if(flagapp==0)
			out = open(argv[outidx+1],O_WRONLY|O_TRUNC|O_CREAT,0644); // Should also be symbolic values for access rights
		else
			out = open(argv[outidx+1],O_WRONLY|O_APPEND|O_CREAT,0644); // Should also be symbolic values for access rights
		savedout = dup(1);
		dup2(out,STDOUT_FILENO);
		close(out);
	}

	//printf("%s\n",args[0]);
	pid_t pid;
	int status;

	if((pid=fork()) < 0)exit(1);

	else if(pid==0)
	{
		execvp(args[0],args);
	}

	else
	{
		while(wait(&status)!=pid);
		if(inpidx>0)
		{dup2(savedin,0);
		close(savedin);}
		if(outidx>0)
		{dup2(savedout,1);
		close(savedout);}
		return;
	}
	printf("Exiting\n");
	exit(0);
}
