#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
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
#include "setenv.h"

#define BUFFERSIZE 2000

void setv(char *arg[],int num)
{
	// arg[0] is the command name
	// arg[1] is the variable name
	// arg[2] is the value in case it is there

//	printf("%d %s\n",num,arg[2]);

	if(num<=1 || num>3){printf("Wrong Format for setenv <varname> <value>/NULL\n");return;}

	if(getenv(arg[1])==NULL)
		setenv(arg[1],"",1);

	if(num==2)
		setenv(arg[1],"",1);
	else if(num==3)
		setenv(arg[1],arg[2],1);

	return;
}
