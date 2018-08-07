#include "cd.h"
#define BUFFERSIZE 2000

int cd(char *pth,char home_dir[]){
	char path[BUFFERSIZE]={0};
	char tem[BUFFERSIZE]={0};
	strcpy(path,pth);
	int i=0;
	int flag=0;
	char cwd[BUFFERSIZE];
	if(pth[0] == '~')
	{
		strcpy(tem,home_dir);
		for(i=1;i<strlen(pth);i++)
		{	tem[strlen(home_dir)-1+i]=pth[i];
		}
		flag = chdir(tem);
	}

	else if(pth[0] != '/')
	{// true for the dir in cwd
		getcwd(cwd,sizeof(cwd));
		strcat(cwd,"/");
		strcat(cwd,path);
		flag = chdir(cwd);
	}else{//true for dir w.r.t. /
		flag = chdir(pth);
	}
	if(flag==-1)perror("incorrect format (cd <directory_name>) or no such directory exists\n");
	return 0;
}
