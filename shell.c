#include "global.h"
#include "hasPrefix.h"
#include "cd.h"
#include "pinfo.h"
#include "parse.h"
#include "ls.h"
#include "echo.h"
#include "redirect.h"
#include "all.h"
#include "setenv.h"
#include "unsetenv.h"
#define BUFFERSIZE 2000
char home_dir[1111]={0};
char curr_dir[1111]={0};
char pr_dir[1111]={0};
int backcount = 1;

typedef struct background_process {
	int job_id;
	int job_state;
	char job_name[100];
	int p_id;
} global_back;

global_back allback[1000];

void cdhelp(char cmmnd[])
{
	char *tempTok = strchr(cmmnd,' '); 
	if(tempTok == NULL)
	{
		cd(home_dir,home_dir);			
	}

	else
	{
		char *temp = tempTok+1;
		char *locationOfNewLine = strchr(temp, '\n');

		if(locationOfNewLine) {
			*locationOfNewLine = '\0';
		}
		if(strcmp(temp,".")==0);
		else if(strcmp(temp,"..")==0)
		{
			int ex = strlen(curr_dir);
			while(ex>0 && curr_dir[ex-1]!='/')
			{
				ex--;
			}
			char parent[BUFFERSIZE]={0};
			if(ex>0)
			{
				strncpy(parent,curr_dir,ex);
				//	printf("%d %s %s\n",ex,parent,curr_dir);
				cd(parent,home_dir);
				strcpy(pr_dir,curr_dir);	
			}
		}
		else if(strcmp(temp,"-")==0)
		{
			cd(pr_dir,home_dir);
			strcpy(pr_dir,curr_dir);	
		}
		else
		{
			cd(temp,home_dir);
			strcpy(pr_dir,curr_dir);	
		}
	}

}

void lshelp(char cmmnd[])
{
	char *tempTok = strchr(cmmnd,' ');
	if(tempTok == NULL)
	{
		lssimp(curr_dir,0);
	}

	else
	{
		char *temp = tempTok+1;
		char *locationOfNewLine = strchr(temp, '\n');

		if(locationOfNewLine) {
			*locationOfNewLine = '\0';
		}
		char lsflag[11]={0};
		int idxls=0;
		int i=0;
		
		char *copy = NULL;
		copy=temp;

		char *directory = NULL;
		while(temp!=NULL)
		{
		//	printf("%s\n",temp);
			temp = strchr(temp,'-');
			if(temp==NULL)break;
			
			temp+=1;
			i=0;
			while(temp[i]!=' ' && temp[i]!='\0')
			{
				lsflag[idxls++]=temp[i++];
			}
		}
		
		if(idxls==0 && copy!=NULL)lssimp(copy,0);

		while(copy!=NULL)
		{
			copy = strchr(copy,' ');
			if(copy==NULL)break;

			copy+=1;
			if(copy[0]!='-' && copy[0]!='\0')
				directory = copy;
		}
		if(idxls==0 && directory!=NULL)lssimp(directory,0);

		if(strlen(lsflag)>2){printf("more flags than expected entered\n");return;}
		i=0;
		for(i=0;i<strlen(lsflag);i++){if(lsflag[i]!='a' && lsflag[i]!='l'){printf("invalid flag. only [al] are allowed\n");}}
		//
		//	char *directory = strchr()
		//
		if(directory==NULL)
		{
			if(strcmp(lsflag,"a")==0)lssimp(curr_dir,1);

			else if(strcmp(lsflag,"al")==0 || strcmp(lsflag,"la")==0)ls(curr_dir,1);

			else if(strcmp(lsflag,"l")==0)ls(curr_dir,0);
		}
		else
		{
			if(strcmp(lsflag,"a")==0)lssimp(directory,1);

			else if(strcmp(lsflag,"al")==0 || strcmp(lsflag,"la")==0)ls(directory,1);

			else if(strcmp(lsflag,"l")==0)ls(directory,0);
		}
	}

}

void pinfohelp(char cmmnd[])
{
	char *pinfoTok = strchr(cmmnd,' ');
	if(pinfoTok==NULL)
	{
		int this_uid = getpid();
		char procid[100]={0};
		sprintf(procid,"%d",this_uid);
		pinfo(procid);
		//printf("%s\n",new_dir);
	}
	else
	{
		char *tempPin = pinfoTok+1;
		char *locationOfNewLine = strchr(tempPin, '\n');

		if(locationOfNewLine) {
			*locationOfNewLine = '\0';
		}
		pinfo(tempPin);
	}

}

void echohelp(char cmmnd[])
{
	char *echoTok = strchr(cmmnd,' ');
	if(echoTok==NULL)
	{
		perror("no input after echo command");
	}
	else
	{
		echoTok+=1;
		char *locationOfNewLine = strchr(echoTok, '\n');

		if(locationOfNewLine) {
			*locationOfNewLine = '\0';
		}
		echo(echoTok);
	}
	printf("\n");

}

int getch (void)
{
	int ch;
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

void interrupthelp(char sleepcnt[])
{
	char intdir[10000]={0};
	strcpy(intdir,"/proc/interrupts");
	FILE *intfd;
	int time_sleep=0,i=0;
	for(i=0;i<strlen(sleepcnt);i++)time_sleep =time_sleep*10+ sleepcnt[i]-48;
	//printf("%d\n",time_sleep);
	int init=0;
	int ret = fork();

	if(ret>0)
	{
		while(1==1)
		{
			intfd = fopen(intdir,"r");
			char line[2000];
			//read a line
			while(fgets(line, sizeof line,intfd)!=NULL){
				if((strstr(line,"i8042")) || (strstr(line,"IRQ")) || init==0)
					{printf("%s", line);init=1;}
			}
			fclose(intfd);
			sleep(time_sleep);
			int status;
			//printf("my-id:%d\n",ret);
			pid_t return_pid = waitpid(ret, &status, WNOHANG);
			//printf("return-id:%d\n",return_pid);
			if(return_pid == ret)break;
			else if(return_pid == 0);
		}
	}
	
	else
	{
		while(1==1)
		{
			if(getch()=='q')
			exit(0);
		}
	}
}

void dirtyhelp(char cmd[])
{
//	time_t lasttime, thistime;
//	lasttime = time(NULL);
	int numsec = atoi(cmd);
	int ret = fork();
	if(ret>0)
	{	
		while(1){
			FILE * dirtyfd = fopen("/proc/meminfo","r");
			char line[2000]={0};
			while(fgets(line, sizeof line,dirtyfd)!=NULL){
				if(strstr(line,"Dirty"))
				{
					char *dirtok = strchr(line,' ');
					while(*dirtok == ' ')dirtok++;

					printf("%s",dirtok);
					break;
				}
			}

			fclose(dirtyfd);
			sleep(numsec);
			int status;
			//printf("my-id:%d\n",ret);
			pid_t return_pid = waitpid(ret, &status, WNOHANG);
			//printf("return-id:%d\n",return_pid);
			if(return_pid == ret)break;
			else if(return_pid == 0);
		}
	}
	else
	{
		while(1==1)
		{
			if(getch()=='q')
			exit(0);
		}		
	}		
		//lasttime += numsec;
		
	return;
}

void ampcdhelp(char cmmnd[])
{
	printf("[%d]          %d\n",backcount,getpid());
	cdhelp(cmmnd);
	return;
}

void amppinfohelp(char cmmnd[])
{
	printf("[%d]          %d\n",backcount,getpid());
	pinfohelp(cmmnd);
	return;
}

void amplshelp(char cmmnd[])
{
	printf("[%d]          %d\n",backcount,getpid());
	lshelp(cmmnd);
	return;
}


void ampechohelp(char cmmnd[])
{
	printf("[%d]          %d\n",backcount,getpid());
	echohelp(cmmnd);
	return;
}

int checkpipe(char command[])
{
	int i=0;
	for(i=0;i<strlen(command);i++)
	{
		if(command[i]=='|')return 1;
	}
	return 0;
}

int checkredirect(char command[])
{
	int i=0;
	for(i=0;i<strlen(command);i++)
	{
		if(command[i]=='<' || command[i]=='>')return 1;
	}
	return 0;
}

void sethelp(char comm[])
{
	char *arg[5];
	int num = parse(comm,arg);
//	printf("%d\n",num);
	setv(arg,num);
	return;
}

void unsethelp(char comm[])
{
	char *arg[5];
	int num = parse(comm,arg);
//	printf("%d\n",num);
	unsetv(arg,num);
	return;
}

void update_back(int pid,char comm[])
{
	printf("%s\n",comm);
	allback[backcount-1].job_id = backcount;
	strcpy(allback[backcount-1].job_name,comm);
	allback[backcount-1].job_state = 1;
	allback[backcount-1].p_id = pid;
	return;
}

void checkback()
{
	struct stat backcheck;
	int i=0;
	for(i=0;i<backcount-1;i++)
	{
		char process_name[500] = {0};
		char pid[6] = {0};
		sprintf(pid,"%d",allback[i].p_id);
		strcpy(process_name,"/proc/");
		strcat(process_name,pid);
	//	printf("%d",stat(process_name,&backcheck));
		if(allback[i].job_state>=0 && stat(process_name,&backcheck)==-1)
		{
			allback[i].job_state=-1;
			printf("[%d]+  		 Done(normally)        %s\n",allback[i].job_id,allback[i].job_name);
		}
	}
	return;
}

void printJobs()
{
	int i=0;
	for(i=0;i<backcount-1;i++)
	{
		printf("[%d]	",allback[i].job_id);
		if(allback[i].job_state>0)printf("Running	");
		else if(allback[i].job_state==-1)printf("Stopped	");

		printf("	%s [%d]\n",allback[i].job_name,allback[i].p_id);
	}
	return;
}

void k_job(char *args[])
{
	if(args[1]==NULL || args[2] ==NULL)
	{
		printf("Usage: kjob <job_id> <signal_num>\n");
		return;
	}

	int jid = atoi(args[1]);
	if(jid >= backcount || jid<=0)
	{
		printf("Error job number does not exist\n");
		return;
	}

	int sig=atoi(args[2]);
	
	kill(allback[jid-1].p_id,sig);
	allback[jid-1].job_state=-1;
	checkback();
	return;
}

void over_kill()
{
	int i=0;
	for(i=0;i<backcount-1;i++)
	{
		kill(allback[i].p_id,SIGSTOP);
		allback[i].job_state=-1;
	}
	checkback();
	return;
}

void bg(char *args[])
{
	int jid = atoi(args[1]);
	if((jid >= backcount || jid<=0) || allback[jid-1].job_state==1)
	{
		printf("Error: job number does not exist or is running\n");
		return;
	}

	char *arg[10];
	int num = parse(allback[jid-1].job_name,arg);
//	printf("%d\n",num);

	pid_t pid;
	int status;

	if((pid=fork()) < 0)exit(1);

	else if(pid==0)
	{
		arg[num]=NULL;
		if(execvp(*arg,arg)<0)exit(1);
	}

	else
	{
		//while(wait(&status) != pid);
		update_back(pid,allback[jid-1].job_name);
		printf("%s with PID[%d] created normally\n", allback[jid-1].job_name,pid);
		backcount++;
	}

	return;
}

void fg(char *args[])
{
	if(args[1]==NULL)
	{
		printf("Usage: fg <jobid>\n");
	}
	
	int jid=atoi(args[1]);
	
	if((jid<=0 || jid >= backcount) || allback[jid-1].job_state==0)
	{
		printf("Error: job number does not exist\n");
		return;
	}

	if(allback[jid-1].job_state==1)
	{
		kill(allback[jid-1].p_id,SIGSTOP);
		checkback();
	}

	allback[jid-1].job_state=0;
	char *argv[10];
	int num = parse(allback[jid-1].job_name,argv);
//	while(wait(&status) != allback[jid-1].p_id);
	pid_t pid;
	int status;

	if((pid=fork()) < 0)exit(1);

	else if(pid==0)
	{
		argv[num]=0;
		kill(allback[jid-1].p_id,SIGKILL);
		if(execvp(*argv,argv)<0)exit(1);
	}
					
	else
	{
		while(wait(&status) != pid);
	}

	return;
}

volatile int gTerminate = 0;
volatile int xTerminate = 0;

void sigkill(int sign)
{
	if(fork()==0)
	{
		gTerminate = 1;
		signal(SIGINT, &sigkill);		
	}
	else
	{
		gTerminate = 0;
	}
	return;
}

int sigpid=0;

void sigstop(int sign)
{
	kill(sigpid,SIGSTOP);	
	return;	
}

int main()
{
	//signal(SIGTSTP,sigstop);
	signal(SIGINT,sigkill);
	register struct passwd *pw;
	register uid_t uid;
	char buffer[BUFFERSIZE]={0};
	struct utsname unm;
	int i=0;

	getcwd(home_dir,sizeof(home_dir));
	getcwd(pr_dir,sizeof(pr_dir));

	while(gTerminate==0 && xTerminate==0)
	{
		checkback();
		uid = geteuid();
		pw = getpwuid(uid);
		uname(&unm);

		if(pw)
		{
			printf("<%s@%s:",pw->pw_name,unm.sysname);
		}
		else
		{
			printf("<Unknown@%s:",unm.sysname);
		}

		if(getcwd(curr_dir,sizeof(curr_dir)))
		{
			//			printf("cur:%s\n",curr_dir);
			if(strcmp(curr_dir,home_dir)==0)
			{
				printf("~>");
			}
			else if(strstr(curr_dir,home_dir)!=NULL)
			{
				printf("~");
				for(i=strlen(home_dir);i<strlen(curr_dir);i++)
					printf("%c",curr_dir[i]);
				printf(">");
			}
			else
			{
				printf("%s>",curr_dir);
			}
		}
		bzero(buffer, BUFFERSIZE);
		fgets(buffer, BUFFERSIZE, stdin);

		char *tok = strtok(buffer,";"); //use something more powerful
		char command[100][BUFFERSIZE]={0};
		int z=0;
		while(tok!=NULL)
		{
			strcpy(command[z++],tok);
			tok = strtok(NULL,";");
		}
		int y=0;
		int flagbg = 0;

		for(y=0;strcmp(command[y],"\n")!=0 && y<z;y++)
		{
			char *argv[10]={0};
			//	printf("%d 5%s5\n",y,command[y]);
			int numArg = parse(command[y],argv);
			numArg=0;
			for(numArg=0;argv[numArg]!=NULL && strcmp(argv[numArg],"")!=0;numArg++);


			//			printf("%d\n",numArg);

			if(y==z-1 && strcmp(argv[numArg-1],"&")==0)flagbg=1;

			int mer = 0;
			char compressed[BUFFERSIZE] = {0};


			//	printf("%s\n",compressed);
			if(flagbg==0)
			{
				int time_sleep=0;
				for(mer=0;mer<=numArg-1;mer++)
				{
					strcat(compressed,argv[mer]);	
					if(mer!=numArg-1)strcat(compressed," ");		
				}
				if(hasPrefix(compressed,"quit")==0)exit(0);
				else if(checkpipe(compressed)==1)pipehelp(compressed);
				else if(checkredirect(compressed)==1)redirect(compressed);
				else if(hasPrefix(compressed,"cd")==0)cdhelp(compressed);
				else if(hasPrefix(compressed,"ls")==0)lshelp(compressed);
				else if(hasPrefix(compressed,"pwd")==0)printf("%s\n",curr_dir);
				else if(hasPrefix(compressed,"pinfo")==0)pinfohelp(compressed);
				else if(hasPrefix(compressed,"echo")==0)echohelp(compressed);
				else if(hasPrefix(compressed,"setenv")==0)sethelp(compressed);
				else if(hasPrefix(compressed,"unsetenv")==0)unsethelp(compressed);
				else if(hasPrefix(compressed,"jobs")==0){checkback();printJobs();}
				else if(hasPrefix(compressed,"kjob")==0){k_job(argv);}
				else if(hasPrefix(compressed,"fg")==0){fg(argv);}
				else if(hasPrefix(compressed,"bg")==0){bg(argv);}
				else if(hasPrefix(compressed,"overkill")==0){over_kill();}
				else if(hasPrefix(compressed,"nightswatch")==0 && strcmp(argv[3],"interrupt")==0 && strcmp(argv[1],"-n")==0)interrupthelp(argv[2]);
				else if(hasPrefix(compressed,"nightswatch")==0 && strcmp(argv[3],"dirty")==0 && strcmp(argv[1],"-n")==0)dirtyhelp(argv[2]);
				else
				{
					pid_t pid;
					int status;

					if((pid=fork()) < 0)exit(1);

					else if(pid==0)
					{
						argv[numArg]=0;
						if(execvp(*argv,argv)<0)exit(1);
					}
					
					else
					{
						sigpid = pid;
						while(wait(&status) != pid);
					}
				}
			}
			else
			{
				int ind=0;
				char ncompressed[BUFFERSIZE] = {0};
jumpfin:
				strcpy(ncompressed,"");
				while(strcmp(argv[ind],"&")!=0)
				{	
					strcat(ncompressed,argv[ind]);	
					if(strcmp(argv[ind+1],"&")!=0)strcat(ncompressed," ");
					ind++;
				}
				ind++;
				int ret = fork();
				if(ret==0)
				{
					if(hasPrefix(ncompressed,"quit")==0)exit(0);
					else if(checkpipe(ncompressed)==1)pipehelp(ncompressed);
					else if(checkredirect(ncompressed)==1)redirect(ncompressed);
					else if(hasPrefix(ncompressed,"cd")==0)ampcdhelp(ncompressed);
					else if(hasPrefix(ncompressed,"ls")==0)amplshelp(ncompressed);
					else if(hasPrefix(ncompressed,"pwd")==0)printf("%s\n",curr_dir);
					else if(hasPrefix(ncompressed,"pinfo")==0)amppinfohelp(ncompressed);
					else if(hasPrefix(ncompressed,"echo")==0)ampechohelp(ncompressed);
					else if(hasPrefix(ncompressed,"setenv")==0)sethelp(ncompressed);
					else if(hasPrefix(ncompressed,"unsetenv")==0)unsethelp(ncompressed);
					else
					{
						pid_t pid;
						int status;

						if((pid=fork()) < 0)exit(1);

						else if(pid==0)
						{
							argv[numArg-1]=NULL;
							if(execvp(*argv,argv)<0)exit(1);
						}

						else
						{
						//	while(wait(&status) != pid);
						}
						exit(0);
					}
					exit(0);
				}
				else
				{
					wait(NULL);
					int status;
					//printf("my-id:%d\n",ret);
					pid_t return_pid = waitpid(ret, &status, WNOHANG);
					//printf("return-id:%d\n",return_pid);

					if ( WIFEXITED(status) ) {
						int es = WEXITSTATUS(status);
						if(es==0)
							{
								printf("%s with PID[%d] created normally\n", ncompressed,ret+1);
								update_back(ret+1,ncompressed);
							}
						else{printf("%s with PID[%d] exited abnormally\n", ncompressed,ret+1);}
					}
					backcount++;
				}

				if(ind<numArg-2)goto jumpfin;
			}
			getcwd(curr_dir,sizeof(curr_dir));
		}
	}
	return 0;
}
