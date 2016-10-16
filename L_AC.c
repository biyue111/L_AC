#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "analex.h"
#include "analex.c"

void cmd_input(char *input)
{	
	printf("Text:");
	scanf("%99[^0]",input);
	getchar();
}

int file_input(char *input,char *filepath)
{	
	FILE *fp = fopen(filepath, "r");
	int i = 0;
	int ch;
	ch=fgetc(fp);
	while(ch!=EOF) 
	{
		//printf("%c",ch);
		input[i++] = ch;
		ch=fgetc(fp);
	}
	input[i] = '\0';
	fclose(fp);
	return 0;
}

int main(int argc, char* argv[])
{
	char text[100];
	D_linklist* ana_lex=create_D_list(INT,"0");
	int i=0;

	printf("TEST: argc = %d\n",argc);

	if(argc==1)
	{
		printf("[Message] Input from Terminal\n");
		cmd_input(text);
	}
	else if(argc==2)
	{
		printf("TEST: two arg\n");
		printf("TEST: argv[1] = %s\n",argv[1]);
		if(argv[1][0]=='1')
		{
			printf("[Message] Input from Terminal\n");
			cmd_input(text);
		}
		else if(argv[1][0]=='2')
		{
			printf("[Message] Input from .//input//input.ac\n");
			file_input(text,"input//input.ac");
		}
		else
		{

		}
	}
	else
	{

	}

	
	analex(text,ana_lex);

	printf("TEST: result:%s\n\n",text);
	printf("TEST: result of analex:\n");

	i=0;
	D_print_list(ana_lex);
		
	return 0;
}
