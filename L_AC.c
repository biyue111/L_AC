#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "analex.h"
#include "analex.c"
#include "processeur.c"

#ifdef DEBUG
#define L_AC_DEBUG
#endif
void help()
{
	printf("./LAC.o [options]\n\
	Options:\n\
	--help \tDisplay this information\n\
	-i \tInterpretor mode\n\
	-f \tCompilor mode, use the file ./input.lac\n\
	-f [filepath] \tCompilor mode, use custom file\n\
	-l [inputfile] [outputfile] \tOnly run lexcal analyse\n\
	-r \truntime mode\n");
}

void cmd_input(char *input)
{	
	scanf("%99[^0]",input);
	getchar();
}

int file_input(char *input,char *filepath)
{	
	FILE *fp = fopen(filepath, "r");
	if(!fp)
	{
		printf("[ERROR]:Not find file %s, exit.\n", filepath);
		exit(0);
	}
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

void interpretor()
{
	char input_text[100];
	D_linklist* ana_lex = create_D_list(0,"");
	data = create_stack();
	type = create_stack();
	retour = create_stack();

	VM_LAC_init();

#ifdef L_AC_DEBUG
	int i=0;
	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");
#endif
	
	while(1)
	{
		printf(">>");
		scanf("%99[^\n]",input_text);
		getchar();
		analex(input_text,ana_lex);
		processer(ana_lex);
		//stack_clear(data);stack_clear(type);stack_clear(retour);
		printf("[MESSAGE]:current type stack(top-bottom)\n");
		print_stack(type);

		printf("[MESSAGE]:current data stack(top-bottom)\n");
		print_stack(data);

		Dlist_clear(ana_lex);

#ifdef L_AC_DEBUG
		printf("Test: LAC:");
		for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
		printf("\n");
		printf("Test: VM:");
		for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
		printf("\n");
#endif
	}	
}

void compilor(char *file)
{
	char input_text[1000];
	file_input(input_text,file);
#ifdef L_AC_DEBUG
	int i=0;
	while(input_text[i] != '\0')
	{
		printf("%c",input_text[i]);
		i++;
	}
#endif

	D_linklist* ana_lex = create_D_list(0,"");
	data = create_stack();
	type = create_stack();
	retour = create_stack();
	VM_LAC_init();

#ifdef L_AC_DEBUG
	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");
#endif
	analex(input_text,ana_lex);
	processer(ana_lex);
	//stack_clear(data);stack_clear(type);stack_clear(retour);
	Dlist_clear(ana_lex);

#ifdef L_AC_DEBUG
	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");
#endif
}

void analise_lexical(char *infile,char *outfile)
{
	char input_text[1000];
	file_input(input_text,infile);
	FILE *outfp;
	int i=0;
#ifdef L_AC_DEBUG
	while(input_text[i] != '\0')
	{
		printf("%c",input_text[i]);
		i++;
	}
#endif

	D_linklist* ana_lex = create_D_list(0,"");
	analex(input_text,ana_lex);
	D_to_begin(ana_lex);
	outfp = fopen(outfile,"w");
	for(i=0;i<ana_lex->length;i++)
	{
		fprintf(outfp,"%s\n",ana_lex->fence->content->value);
		D_to_next(ana_lex);
	}
	fclose(outfp);
}

int main(int argc, char* argv[])
{
	//D_linklist* ana_lex=create_D_list(INT,"0");
	//int i=0;
	int input_flag = 1;
#ifdef L_AC_DEBUG
	printf("TEST(L_AC): argc = %d\n",argc);
#endif
	if(argc == 1)
	{
		printf("[Message] Begin interpretor mode\n");
		interpretor();
	}
	else if(argc == 2)
	{
#ifdef L_AC_DEBUG
		printf("TEST(L_AC): two arg\n");
		printf("TEST(L_AC): argv[1] = %s\n",argv[1]);
#endif
		if(strcmp(argv[1], "-i") == 0)
		{
			printf("[Message] Begin interpretor mode\n");
			interpretor();
		}
		else if(strcmp(argv[1],"-f") == 0)
		{
			printf("[Message] Input from .//input//input.lac\n");
			compilor("input.lac");
		}
		else if(strcmp(argv[1],"--help") == 0)
			help();
		else
			input_flag = 0;
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1],"-f")==0)
		{
			printf("[Message] Input from %s\n",argv[2]);
			compilor(argv[2]);
		}
		else 
			input_flag = 0;
	}
	else if(argc == 4)
	{
		if(strcmp(argv[1],"-l")==0)//analise lexical
		{
			printf("[Message] Input from %s\n",argv[2]);
			analise_lexical(argv[2],argv[3]);
			printf("[Message] Input from %s\n",argv[3]);
		}
		else 
			input_flag = 0;
	}
	else
		input_flag = 0;

	if(input_flag == 0)
	{
		printf("Wrong input, please refer to help\n");
		help();
	}
	//analex(text,ana_lex);

	//printf("TEST: result:%s\n\n",text);
	//printf("TEST: result of analex:\n");

	//i=0;
	//D_print_list(ana_lex);
		
	return 0;
}
