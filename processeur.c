#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"
#include "util.c"
#include "analex.h"
#include "analex.c"

/*************************
* pile de donnees: data
* pile de type: type
* table de symbol: LAC
* Machine vertuel: VM
* Etat de systeme: state

* In when meet a funcion, the first parameter of the 
* function is the top value in data stack...
*************************/

#define FUNC_BASE_NUM 9
#define LAC_LENGTH 500
#define VM_LENGTH 100
#define PROCESSEUR_LENGTH 50

//#define NDEFTYPE 0
//#define CUS_INT 1

typedef struct stack_node{
	int val;
	struct stack_node *next;
}stack_node;

typedef struct stack{
	stack_node *top;
	int length;
}stack;

stack_node* create_stack_node(int num)
{
	stack_node *sn = (stack_node*)malloc(sizeof(stack_node));
	sn->val = num;
	sn->next = NULL;
	return sn;
}

stack* create_stack()
{
	stack *s = (stack*)malloc(sizeof(stack));
	s->top = NULL;
	s->length = 0;
	return s;
}

int push(int num,stack* i_stack)
{
	stack_node *temp_n = create_stack_node(num);
	temp_n->next = i_stack->top;
	i_stack->top = temp_n;
	(i_stack->length)++;
	return 1;
}

int pop(int* pnum,stack* i_stack)
{
	if(i_stack->length>0)
	{
		*pnum = i_stack->top->val;
		stack_node *torel = i_stack->top;
		i_stack->top = i_stack->top->next;
		free(torel);
		(i_stack->length)--;
		return 1;
	}
	else return 0;
}

int stack_get_top(int* pnum,stack* i_stack)//get the top of stack
{
	if(i_stack->length>0)
	{
		*pnum = i_stack->top->val;
		return 1;
	}
	else return 0;
}

typedef void (*pfunc_base)(void);

int LAC[LAC_LENGTH];
int LAC_length;
int VM[VM_LENGTH];
int VM_length;
pfunc_base processeur[PROCESSEUR_LENGTH];
int processeur_length;
int processeur_state;//0: Exucte functions; 1: define function;
int state;
int input_pos;//position read the result of analyse syn
stack* data,*type,*retour;
char *currtext;

int test_type(char *s);

//generate all base function to LAC[]
//functype:
//0: fonction de base
//1: user difined function
//
int func2LAC(char *func_base_str,int num_e,int *type_e,
		int num_s,int *type_s,pfunc_base f,int functype)
{
	int fun_len=0,begin=LAC_length;
	int j=0;
	//int flag=1;//whether LAC[] is long enought
	if(functype == 0)
	{
		LAC_length++;//skip first case
		while(func_base_str[fun_len]!='\0')
		{
			LAC[LAC_length++]=(int)(func_base_str[fun_len++]);
		}
		LAC[begin] = fun_len;

		LAC[LAC_length++] = num_e;
		for(j=0;j<num_e;j++) LAC[LAC_length++] = type_e[j];

		LAC[LAC_length++] = num_s;
		for(j=0;j<num_s;j++) LAC[LAC_length++] = type_s[j];


		LAC[LAC_length++] = VM_length;
		VM[VM_length++] = functype;
		VM[VM_length++] = processeur_length;
		processeur[processeur_length++] = f;
		LAC[LAC_length++] = begin;
	}
	return 1;
}


int complie()
{
	
}

void add()
{
	int process;
	int aug1,aug2,t_aug1,t_aug2;
	int res;
	int v1,v2;
	int type_verfi=0;

	pop(&process,retour);
	v1 = pop(&t_aug1,type);
	v2 = pop(&t_aug2,type);
	if(v1 && v2)
	{
		if(t_aug1 == INT && t_aug2 == INT)
		{
			push(INT,type);
			type_verfi = 1;
		}
		else
		{
			printf("[ERROR]: sementic fault, wrong type\n");
		}
	}
	else
	{
		printf("[ERROR]: sementic, lack varable");
	}

	if(type_verfi)
	{
		pop(&aug1,data);
		pop(&aug2,data);
		res = aug1 + aug2;
		push(res,data);
		push(process+1,retour);
	}
	
}

void point()
{
	int process;
	int aug1,t_aug1;
	int v1;
	int type_verfi=0;

	pop(&process,retour);
	v1 = pop(&t_aug1,type);
	if(v1)
	{
		if(t_aug1 == INT)
		{
			type_verfi = 1;
			printf("Test: right type\n");
		}
		else
		{
			printf("[ERROR]: sementic fault, wrong type\n");
		}
	}
	else
	{
		printf("[ERROR]: sementic, lack varable");
	}

	if(type_verfi)
	{
		pop(&aug1,data);
		printf("%d\n",aug1);
		push(process+1,retour);
	}
}

void fin()
{
	int process;
	pop(&process,retour);
}

void lit()//put a INT in stack
{
	int processer_pos;
	int var_type=0;

	pop(&processer_pos,retour);
	push(processer_pos+2,retour);
	
	var_type = test_type(VM[processer_pos+1]);
	if(var_type == INT)
	{
		push(INT,type);
		push(VM[processer_pos+1],data);
	}
	else printf("[ERROR]: sementic fault, %s not a INT type\n",currtext);
}

void two_point()
/*Let function processeur call v_prosseur 
* in the next loop*/
{
	processeur_state = 1;
}

int test_func(char* s,int *LAC_pos)
{
	int pos,s_length=0,k=0,flag=1;//pos in LAC
	
	k=0;
	while(s[k++]!='\0') s_length++;
	pos = LAC[LAC_length-1];
	while(pos>0)
	{
		pos = LAC[pos-1];
		flag = 1;
		if(s_length==LAC[pos])//length much
		{
			printf("Test: length much\n");
			for(k=0;k<s_length;k++)
			{
				if(s[k]!=LAC[pos+k+1])
				flag = 0;
				break;
			}
			if(flag)
			{
				printf("Test: find function %s\n",s);
				*LAC_pos = pos;
				return 1;
			}
		}
	}
	printf("Test: NOT find function %s\n",s);
	return 0;//not find the function
	
}

int test_type(char *s)
{
	int k=0;
	if(isdigit(s[0]))	return INT;
}

int v_processer(D_linklist* lex_list)
{
	int func_input_num=0,func_output_num=0;
	int func_input[10],func_output[10];
	stack *temp_data,*temp_type;
	temp_data = create_stack();
	temp_type = create_stack();
	int para_type;
	int flag=1;
	//if an Error detected, rewrite VM to initial length
	int VM_init_length = VM_length;

	char *func_name[20];
	int func_LAC_pos,para_LAC_pos,return_LAC_pos,func_VM_pos;

	int k;
	currtext = lex_list->fence->content->value;
	if(currtext[0]==':' && currtext[1]=='\0')
	{
		printf("[ERROR](v_processer): invaild function name ':'\n");
		VM_length = VM_init_length;
		return 1;//test un error
	}
	else if(currtext[0]==';' && currtext[1]=='\0')
	{
		printf("[ERROR](v_processer): No function name ':'\n");
		VM_length = VM_init_length;
		return 1;//test un error
	}
	else if(test_func(currtext,&func_LAC_pos))
	{
		printf("[ERROR](v_processer): redefine function %s",currtext);
		VM_length = VM_init_length;
		return 1;//test un error
	}

	//valide function name
	k=0;
	while(currtext[k]!='\0')
		func_name[k] = currtext[k++];
	func_name[k] = '\0';
	VM[VM_length++] = 1;//A function with right name

	if(!D_to_next(lex_list))
	{
		printf("[ERROR](v_processer):No ';'");
		VM_length = VM_init_length;
		return 4;//test un error
	}

do
{
	currtext = lex_list->fence->content->value;
	if(!flag)
	{
		printf("[ERROR](v_processer): sementic fault, wrong type\n");
		VM_length = VM_init_length;
		return 2;//test un error
	}
	else if(currtext[0]==':' && currtext[1]=='\0')
	{
		printf("[ERROR](v_processer): find ':' between :;\n");
		VM_length = VM_init_length;
		return 3;//test un error
	}
	else if(currtext[0]==';' && currtext[1]=='\0')
	{
		
		return 0;//no error
	}
	
	if(test_func(currtext,&func_LAC_pos))
	{
		para_LAC_pos = func_LAC_pos + 1 + LAC[func_LAC_pos];
		return_LAC_pos = para_LAC_pos+1+LAC[para_LAC_pos];

		printf("Test(v_processer): func_LAC_pos:%d, re:%d\n",func_LAC_pos, return_LAC_pos);
		func_VM_pos = LAC[return_LAC_pos+1+LAC[return_LAC_pos]];

		//test type of parameter
		for(k=0;k<LAC[para_LAC_pos];k++)
		{
			if(pop(temp_type,type))
			{
				if(para_type!=LAC[para_LAC_pos+k+1])
				//find wrong type
				{
					flag = 0;			
					break;
				}
			}
			else
			{
				func_input[func_input_num++] = LAC[para_LAC_pos+k+1];
			}
		}

		VM[VM_length++] = func_VM_pos;
	}
	else
	{
		var_type = test_type(currtext);
		if(var_type == INT)
		{
			push(INT,temp_type);
			VM[VM_length++] = 0;//function lit
			VM[VM_length++] = atoi(currtext);
			printf("Test(v_processer):add %s in VM\n",currtext);
		}
		else
		{
			printf("[ERROR](v_processer): , %s not a INT type\n",currtext);
		}
	}
	
}while(D_to_next(lex_list));
	printf("[ERROR](v_processer):No ';'");
	VM_length = VM_init_length;
	return 4;//test un error
	
}

void processer(D_linklist* lex_list)
{
int func_LAC_pos,para_LAC_pos,return_LAC_pos,para_type,func_VM_pos;
pfunc_base function;
int var_type;
int k=0;
int type_test_flag=1;
if(!D_to_begin(lex_list)) return;//list empty
do
{
	if(processeur_state == 1)
	{
		v_processer(lex_list);//define a function
		processeur_state = 0;
	}
	currtext = lex_list->fence->content->value;
	
	if(test_func(currtext,&func_LAC_pos))
	{
		para_LAC_pos = func_LAC_pos + 1 + LAC[func_LAC_pos];
		return_LAC_pos = para_LAC_pos + 1 + LAC[para_LAC_pos];

		printf("Test: func_LAC_pos:%d, re:%d\n",func_LAC_pos, return_LAC_pos);
		func_VM_pos = LAC[return_LAC_pos + 1 + LAC[return_LAC_pos]];
/*
		//test type of parameter

		
		for(k=0;k<LAC[para_LAC_pos];k++)
		{
			if(pop(para_type,type))
			{
				if(para_type!=LAC[para_LAC_pos+k+1])
				{
					flag = 0;
					printf("[ERROR]: sementic fault, wrong type\n");
					break;
				}
			}
			else
			{
				flag = 0;
				printf("[ERROR]: sementic fault, lack of parametter\n");
			}
		}
*/
		if(VM[func_VM_pos]==0)//fonction de base
		{
			printf("Test: VM pos %d\n",func_VM_pos);
			function = processeur[VM[func_VM_pos+1]];
			printf("execute function %s\n",currtext);
			function();
		}
		else if(VM[func_VM_pos]==1)//fonction defini en LAC
		{
			int VM_num;
			push(func_VM_pos+1,retour);
			while(stack_get_top(&VM_num,retour))
			{
				function = processeur[VM[VM_num]];
				function();
			}
		}

	}
	else
	{
		var_type = test_type(currtext);
		if(var_type == INT)
		{
			push(INT,type);
			push(atoi(currtext),data);
			printf("pushed %s\n",currtext);
		}
		else printf("[ERROR]: sementic fault, %s not a INT type\n",currtext);
	}
	
}while(D_to_next(lex_list));

}

void init()
{
	//int get_LAC(char *func_base_str,int num_e,int *type_e,int num_s,int *type_s,pfunc_base f)
	processeur_length = 0;
	VM_length = 0;
	LAC_length = 0;
	int inputval[10],outputval[10];

	LAC[LAC_length++] = 0;

	func2LAC("lit",0,inputval,0,outputval,lit,0);//Cfa = 0
	func2LAC("fin",0,inputval,0,outputval,fin,0);//Cfa = 2

	inputval[0]=INT;inputval[1]=INT;
	outputval[0]=INT;
	func2LAC("+",2,inputval,1,outputval,add,0);

	inputval[0]=INT;
	outputval[0]=INT;
	func2LAC(".",1,inputval,0,outputval,point,0);


}

int main(int argc, char* argv[])
{
	int i=0;
	char input_text[100];
	D_linklist* ana_lex = create_D_list(0,"");
	data = create_stack();
	type = create_stack();
	retour = create_stack();

	init();
	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");

	printf(">>");
	scanf("%99[^\n]",input_text);
	analex(input_text,ana_lex);
	processer(ana_lex);

	return 0;
}
