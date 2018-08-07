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
#include "unsetenv.h"

#define BUFFERSIZE 2000

void unsetv(char *arg[],int num)
{
	if(num!=2){printf("Wrong Format for unsetenv <varname>");return;}

	if((getenv(arg[1]))==NULL)
		printf("Variable '%s' doesn't exist\n",arg[1]);
	else if(unsetenv(arg[1])!=0)
		printf("Error while unsetting\n");
}
