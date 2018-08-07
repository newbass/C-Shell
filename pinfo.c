#include "pinfo.h"

void pinfo(char procid[])
{
	char new_dir[2000]={0};
	char exec[2000]={0};
	char exec_path[2000]={0};
	char stat_path[2000]={0};
	strcpy(new_dir,"/proc/");
	strcat(new_dir,procid);
	strcpy(exec_path,new_dir);
	strcpy(stat_path,new_dir);
	strcat(exec_path,"/exe");
	strcat(stat_path,"/status");
	//	printf("%s %s\n",exec_path,stat_path);
	int count=1;
	FILE *file =fopen(stat_path,"r");
	if(file==NULL)
		perror("PID doesn't exist");
	else
	{
		char line[2000];
		//read a line
		while(fgets(line, sizeof line,file)!=NULL){
			if(count==1||count==2||count==3||count== 6||count==7)
				printf("%s", line);
			count++;

		}
		struct rusage usage;
		readlink(exec_path,exec,sizeof(exec));
		getrusage(RUSAGE_SELF,&usage);

		printf("Executable Path: %s\n",exec);
		printf("Memory Usage: %ld\n",usage.ru_maxrss);
		fclose(file); 
	}
	return;
}
