#include "echo.h"

void echo(char echoTok[])
{
	int j=0,flags=0;
	if(echoTok[0]==34){flags = 1;j++;goto jump1;}
	else if(echoTok[0]==39){flags = 2;j++;goto jump2;}
	else goto jump0;

jump1:
	while(j<strlen(echoTok) && echoTok[j]!=34){printf("%c",echoTok[j]);j++;}
	j++;

	if(j<strlen(echoTok) && echoTok[j]==39){j++;goto jump2;}
	else if(j<strlen(echoTok) && echoTok[j]==34){j++;goto jump1;}
	else if(j<strlen(echoTok) && (echoTok[j]!=39 && echoTok[j]!=34))goto jump0;

jump2:						
	while(j<strlen(echoTok) && echoTok[j]!=39){printf("%c",echoTok[j]);j++;}
	j++;

	if(j<strlen(echoTok) && echoTok[j]==39){j++;goto jump2;}
	else if(j<strlen(echoTok) && echoTok[j]==34){j++;goto jump1;}
	else if(j<strlen(echoTok) && (echoTok[j]!=39 && echoTok[j]!=34))goto jump0;

jump0:						
	while(j<strlen(echoTok) && (echoTok[j]!=39 && echoTok[j]!=34))
	{
		if(echoTok[j]=='$')
		{
			char vari[1000]={0};
			int k=0;
			j++;
			while(j<strlen(echoTok) && echoTok[j]!=' ')vari[k++]=echoTok[j++];

			char *var = getenv(vari);
			printf("%s",var);
		}

		if(echoTok[j]!=92)
		{printf("%c",echoTok[j]);}

		j++;
	}

	if(j<strlen(echoTok) && echoTok[j]==39){j++;goto jump2;}
	else if(j<strlen(echoTok) && echoTok[j]==34){j++;goto jump1;}
	else if(j<strlen(echoTok) && (echoTok[j]!=39 && echoTok[j]!=34))goto jump0;

	return;
}
