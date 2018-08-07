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
#include "ls.h"
#define BUFFERSIZE 2000
void lssimp(char *pth,int mode)
{
	DIR *dir;
	struct dirent *file;
	dir = opendir(pth);
	while((file = readdir(dir)) != NULL)
	{
		if(mode==0 && ((strcmp(file->d_name,"..")!=0 && strcmp(file->d_name,".")!=0) && file->d_name[0]!='.'))
			printf("%s ",file->d_name);

		else if(mode==1)
			printf("%s ",file->d_name);
	}
	printf("\n");
	closedir(dir);
}

void ls(char *pth,int mode)
{
	DIR *dir;
	struct dirent *file;
	struct stat mstat;
	struct stat st;
	register struct passwd *pw;
	register struct group *aw;
	char buf[BUFFERSIZE]={0};
	dir = opendir(pth);

	int size=0;
	while((file = readdir(dir)) != NULL)
	{
		stat(file->d_name,&st);
		if((strcmp(file->d_name,"..")!=0) && (strcmp(file->d_name,".")!=0) && (file->d_name[0]!='.')){size+=st.st_blocks;}
	}
	closedir(dir);
	printf("total %d\n",size/2);
	dir = opendir(pth);
	while((file = readdir(dir)) != NULL)
	{
		char modtime[BUFFERSIZE]={0};
		sprintf(buf, "%s/%s", pth, file->d_name);
		stat(buf, &mstat);

		if(mode==0 && ((strcmp(file->d_name,"..")==0 || strcmp(file->d_name,".")==0) || file->d_name[0]=='.'))continue;

		pw = getpwuid(mstat.st_uid);
		aw = getgrgid(mstat.st_gid);
		stat(file->d_name,&st);

		if(S_ISDIR(st.st_mode))printf("d");
		else printf("-");

		if(st.st_mode & S_IRUSR)printf("r");
		else printf("-");

		if(st.st_mode & S_IWUSR)printf("w");
		else printf("-");

		if(st.st_mode & S_IXUSR)printf("x");
		else printf("-");

		if(st.st_mode & S_IRGRP)printf("r");
		else printf("-");

		if(st.st_mode & S_IWGRP)printf("w");
		else printf("-");

		if(st.st_mode & S_IXGRP)printf("x");
		else printf("-");

		if(st.st_mode & S_IROTH)printf("r");
		else printf("-");

		if(st.st_mode & S_IWOTH)printf("w");
		else printf("-");

		if(st.st_mode & S_IXOTH)printf("x ");
		else printf("- ");

		strcpy(modtime,ctime(&mstat.st_mtime));
		char *to = strtok(modtime," "); 
		int i=0;
		printf("%3ld %s %s ",mstat.st_nlink,pw->pw_name,aw->gr_name);
		printf("%10zu ",mstat.st_size);
		while(i<4)
		{
			if(i==2)
				printf("%2s ",to);
			else
				printf("%s ",to);

			to = strtok(NULL," ");
			i++;
		}
		printf("%s\n",file->d_name);
	}
	closedir(dir);
}

