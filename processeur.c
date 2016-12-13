#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"
#include "util.c"
#include "analex.h"
#include "analex.c"
#ifndef _PROCESSEUR_C
#define _PROCESSEUR_C
/*************************
* pile de donnees: data
* pile de type: type
* table de symbol: LAC
* Machine vertuel: VM
* Etat de systeme: state

* In when meet a funcion, the first parameter of the 
* function is the top value in data stack...
*************************/
#define PROCESSEUR_DEBUG
#define FUNC_BASE_NUM 9
#define LAC_LENGTH 500
#define VM_LENGTH 100
#define PROCESSEUR_LENGTH 50
#define CHAIN_MEMORY_SIZE 1000

//#define NDEFTYPE 0
//#define CUS_INT 1
//processeur_state
#define MODE_CALCULATER 0
#define MODE_COMPILER 1
#define CON_ELSE 2


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
char chain_memory[CHAIN_MEMORY_SIZE];
int chain_memory_length;
int processor_error_flag;//if error, exit processor

int test_type(char *s);

//generate all base function to LAC[]
//functype:
//0: fonction de base
//1: user difined function
//
int func2LAC(int funcptpos,pfunc_base f,int VM_pos,char *func_base_str,
		int num_e,int *type_e,int num_s,int *type_s)
{
	int fun_len=0,begin=LAC_length;
	int j=0;
	//int flag=1;//whether LAC[] is long enought
//	if(functype == 0)
//	{
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

	if(f != NULL)
	{
		LAC[LAC_length++] = VM_length;
		VM[VM_length++] = 0;
		if(processeur[funcptpos]==NULL)
		{
			VM[VM_length++] = funcptpos;
			processeur[funcptpos] = f;
		}
		else 
		{
			printf("[ERROR](func2LAC): Basic function postion error in array processer\n");
		}
	}
	else//function define in LAC 
	{
		LAC[LAC_length++] = VM_pos;
	}

	LAC[LAC_length++] = begin;
//	}
	return 0;
}


/*******************
* fonction de base
*******************/
void fin()
{
	int process;
	pop(&process,retour);
	//move the processer for user-defiend
	if(pop(&process,retour))
	{
		push(process + 1, retour);
#ifdef PROCESSEUR_DEBUG
		printf("Test(fin):pushed %d in retour\n",process+1);
#endif
	}
}
void str()
{
	int process_pos;
	int chain_len;
	pop(&process_pos,retour);
	chain_len = VM[process_pos+1];
	push(process_pos+2+chain_len,retour);
	push(CHAIN,type);
	push(chain_memory_length,data);//address of chain
	chain_memory[chain_memory_length++] = chain_len;
	int i=0;
	for(i=0;i<chain_len;i++)
	{
		chain_memory[chain_memory_length++] = (char)VM[process_pos+2+i];
	}
}

void lit()//put a INT in stack
{
	int processer_pos;
	//int var_type=0;

	pop(&processer_pos,retour);
	push(processer_pos+2,retour);
	
	//var_type = test_type(VM[processer_pos+1]);
	//if(var_type == INT)
	//{
		push(INT,type);
		push(VM[processer_pos+1],data);
		printf("Test(lit):pushed %d\n",VM[processer_pos+1]);
	//}
	//else printf("[ERROR]: sementic fault, %s not a INT type\n",currtext);
}

void dup()
{
	int processor_pos;
	int aug,aug_type;

	if(pop(&processor_pos,retour))
		push(processor_pos+1,retour);
	if(pop(&aug_type,type) && pop(&aug,data))
	{
		push(aug_type,type);
		push(aug_type,type);
		push(aug,data);
		push(aug,data);
	}	
	else
	{
		printf("[ERROR](dup):No input parameter\n");
		processor_error_flag = 1;
		return;
	}
}

void drop()
{
	int processor_pos;
	int aug,aug_type;

	if(pop(&processor_pos,retour))
		push(processor_pos+1,retour);
	if(pop(&aug_type,type) && pop(&aug,data))
	{ }	
	else
	{
		printf("[ERROR](drop):No input parameter\n");
		processor_error_flag = 1;
		return;
	}
}

void swap()
{
	int processor_pos;
	int aug1,aug2,aug_type1,aug_type2;

	if(pop(&processor_pos,retour))
		push(processor_pos+1,retour);
	if(pop(&aug_type1,type) && pop(&aug1,data) &&
		pop(&aug_type2,type) && pop(&aug2,data))
	{
		push(aug_type1,type);
		push(aug_type2,type);
		push(aug1,data);
		push(aug2,data);
	}	
	else
	{
		printf("[ERROR](swap):No input parameter\n");
		processor_error_flag = 1;
		return;
	}
}

void add()
{
	int process;
	int aug1,aug2;
	int aug_t;
	int res;
	//int v1,v2;
//	int type_verfi=0;

	if(pop(&process,retour))
		push(process+1,retour);

//	v1 = pop(&t_aug1,type);/*{{{*/
//	v2 = pop(&t_aug2,type);
//	if(v1 && v2)
//	{
//		if(t_aug1 == INT && t_aug2 == INT)
//		{
//		
//			push(INT,type);
//			type_verfi = 1;
//		}
//		else
//			printf("[ERROR]: sementic fault, wrong type\n");
//	}
//	else
//		printf("[ERROR]: sementic, lack varable");
//
//	if(type_verfi)/*}}}*/
		pop(&aug1,data);
		pop(&aug2,data);
		pop(&aug_t,type);
		pop(&aug_t,type);
		
		res = aug1 + aug2;
		push(res,data);
		push(INT,type);
		printf("Test(add):pushed %d\n",res);
//	}
	
}

void minus()
{
	int process;
	int aug1,aug2;
	int aug_t;
	int res;

	if(pop(&process,retour))
		push(process+1,retour); 
	pop(&aug1,data);
	pop(&aug2,data);
	pop(&aug_t,type);
	pop(&aug_t,type);

	res = aug2 - aug1;
	push(res,data);
	push(INT,type);
#ifdef PROCESSEUR_DEBUG
	printf("Test(minus):pushed %d\n",res);
#endif
}

void multiply()
{
	int process;
	int aug1,aug2;
	int aug_t;
	int res;

	if(pop(&process,retour))
		push(process+1,retour); 
	pop(&aug1,data);
	pop(&aug2,data);
	pop(&aug_t,type);
	pop(&aug_t,type);
	res = aug2 * aug1;
	push(res,data);
	push(INT,type);
#ifdef PROCESSEUR_DEBUG
	printf("Test(multiply):pushed %d\n",res);
#endif
}

void divide()
{
	int process;
	int aug1,aug2;
	int aug_t;
	int res;

	if(pop(&process,retour))
		push(process+1,retour); 
	pop(&aug1,data);
	pop(&aug2,data);
	pop(&aug_t,type);
	pop(&aug_t,type);
	res = aug2 / aug1;
	push(res,data);
	push(INT,type);
#ifdef PROCESSEUR_DEBUG
	printf("Test(minus):pushed %d\n",res);
#endif
}
void point()
{
	int process;
	int aug1;
	int aug_t;
	//int v1;
	//int type_verfi=0;

	if(pop(&process,retour))
		push (process+1,retour);
		
//	pop(&process,retour);/*{{{*/
//	v1 = pop(&t_aug1,type);
//	if(v1)
//	{
//		if(t_aug1 == INT)
//		{
//			type_verfi = 1;
//			printf("Test: right type\n");
//		}
//		else
//		{
//			printf("[ERROR]: sementic fault, wrong type\n");
//		}
//	}
//	else
//	{
//		printf("[ERROR]: sementic, lack varable");
//	}/*}}}*/

	pop(&aug1,data);
	pop(&aug_t,type);
	printf("%d\n",aug1);
}

void count()
{
	int chain_add;
	int chain_len;
	int process;
	int aug_t;
	if(pop(&process,retour))
		push (process+1,retour);
		
	pop(&chain_add,data);
	pop(&aug_t,type);
	chain_len = chain_memory[chain_add];
	chain_add++;
	push(chain_add,data);
	push(CHAIN,type);
	push(chain_len,data);
	push(INT,type);
}

void func_type()
{
	int chain_add;
	int chain_len;
	int process;
	int aug_t;
	if(pop(&process,retour))
		push (process+1,retour);
	pop(&chain_len,data);
	pop(&chain_add,data);
	pop(&aug_t,type);
	pop(&aug_t,type);
	int i=0;
	for(i=0;i<chain_len;i++)
		printf("%c",chain_memory[chain_add+i]);
	printf("\n");
}

void equal()
{
	int process;
	int aug1,aug2;
	//int v1;
	int aug_t;

	if(pop(&process,retour))
		push (process+1,retour);

	pop(&aug1,data);
	pop(&aug2,data);
	pop(&aug_t,type);
	pop(&aug_t,type);

	push(aug1 == aug2, data);
	push(INT,type);
}

void LAC_if(){
	int process;
	int aug1;
	int aug_t;

	pop(&aug1,data);
	pop(&aug_t,type);
	if(pop(&process,retour))
		if(aug1)//if true
			push(process+2,retour);
		else
			push(VM[process+1] + 1,retour);
	
	else
		printf("[ERROR](if): Try to use if in calculator mode");
}

void LAC_else(){
	int process;
	//only in condition <iftrue> we will meet else function
	if(pop(&process,retour))
		push(VM[process+1],retour);
	else
		printf("[ERROR](else): Try to use if in calculator mode");
}

void LAC_then(){
	int process;
	//only in condition <iftrue> we will meet else function
	if(pop(&process,retour))
		push(process+1,retour);
	
	else
		printf("[ERROR](then): Try to use if in calculator mode");

}

/*
void two_point()
*Let function processeur call v_prosseur 
* in the next loop
{
	processeur_state = 1;
}
*/
int test_func(char* s,int *LAC_pos)
{
	int pos,s_length=0,k=0,flag=1;//pos in LAC
	
	k=0;
	while(s[k++]!='\0') s_length++;
	pos = LAC_length;
	while(pos>0)
	{
		pos = LAC[pos-1];
		flag = 1;
		if(s_length==LAC[pos])//length much
		{
#ifdef PROCESSEUR_DEBUG
			printf("Test: length much at %d\n",pos);
#endif
			for(k=0;k<s_length;k++)
			{
				if(s[k]!=LAC[pos+k+1])
				{
					flag = 0;
					break;
				}
			}
			if(flag)
			{
#ifdef PROCESSEUR_DEBUG
				printf("Test: find function %s\n",s);
#endif
				*LAC_pos = pos;
				return 1;
			}
		}
	}
#ifdef PROCESSEUR_DEBUG
	printf("Test: NOT find function %s\n",s);
#endif
	return 0;//not find the function
	
}

int test_type(char *s)
{
	//int k=0;
	int s_length = 0;
	int type;
	while(s[s_length]!='\0')
		s_length++;
	if(isdigit(s[0]))	
		type = INT;
	else if(s[0] == '"' && s[s_length-1]=='"')
		type = CHAIN;
	else
		type = UNDEF;//other type
#ifdef PROCESSEUR_DEBUG
	printf("Test(test_type):find type:%d\n",type);
#endif
	return type;
}

int v_processer(D_linklist* lex_list)
{//mode de compilateur
	processeur_state = MODE_COMPILER;
	int func_input_num=0,func_output_num=0;
	int func_input[10],func_output[10];
	int m=0;
	for(m=0;m<10;m++)
	{
		func_input[m] = -1;
		func_output[m] = -1;
	}
	stack *temp_type_stack;
	stack *condition_stack;//record the level of condition
	//record the temps_data_stack 
	//when enter if
	s_stack *if_type_s_stack;	
	a_stack *func_input_a_stack;
	stack *func_input_num_stack;
	//stack *condition_type_stack;
	//temp_data_stack = create_stack();
	temp_type_stack = create_stack();
	condition_stack = create_stack();
	if_type_s_stack = create_s_stack();
	func_input_a_stack = create_a_stack();
	func_input_num_stack = create_stack();
	//if_stack = create_array_stack();
	//else_stack = create_array_stack();
	int para_type,var_type;
	int type_flag=1;
	int condition_flag=1;
	//if an Error detected, rewrite VM to initial length
	int VM_init_length = VM_length;

	char func_name[20];
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
	{
		func_name[k] = currtext[k];
		k++;
	}
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
	if(!type_flag)
	{
		printf("[ERROR](v_processer): sementic fault, wrong type\n");
		VM_length = VM_init_length;
		return 2;//test un error
	}
	if(!condition_flag)
	{
		printf("[ERROR](v_processer): sementic fault, condition branchs not much\n");
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
	{//right defined user function
		VM[VM_length++] = 4;//add fin function
		func_output_num = temp_type_stack->length;
		for(k=func_output_num-1;k>=0;k--)
			pop(&func_output[k],temp_type_stack);
		printf("Test(v_processer):function name is %s\n",func_name);
		//add user function to LAC and VM
		//funcpos is -1, which will not be used in func2LAC becasue of NULL finction pt
		func2LAC(-1,NULL,VM_init_length,func_name,func_input_num,func_input,func_output_num,func_output);

		return 0;//no error
	}
	
	if(test_func(currtext,&func_LAC_pos))
	{
		para_LAC_pos = func_LAC_pos + 1 + LAC[func_LAC_pos];
		return_LAC_pos = para_LAC_pos + 1 + LAC[para_LAC_pos];
		func_VM_pos = LAC[return_LAC_pos + 1 + LAC[return_LAC_pos]];//Cfa of function

		printf("Test(v_processer): func_LAC_pos:%d, re:%d\n",func_LAC_pos, return_LAC_pos);
		//test type of parameter
		for(k=0;k<LAC[para_LAC_pos];k++)
		{
			//if(processeur_state == CON_ELSE)
			//{
			//	printf("Test(v_processeur):ignore input, In CON_ELSE\n");
			//	break;//no nned to when in else branch
			//}
			if(pop(&para_type,temp_type_stack))
			{
				int t_type;
				pop(&t_type, temp_type_stack);
				if(para_type!=LAC[para_LAC_pos+k+1])
				//find wrong type
				{
					type_flag = 0;			
					break;
				}
			}
			else
			{
				func_input[func_input_num++] = LAC[para_LAC_pos+k+1];
			}
		}
		//add output
		for(k=0;k<LAC[return_LAC_pos];k++)	
		{
			//if(processeur_state == CON_ELSE)
			//{
			//	printf("Test(v_processeur):ignore output In CON_ELSE\n");
			//	break;//no nned to when in else branch
			//}
			push(LAC[return_LAC_pos+k+1],temp_type_stack);
			//push(LAC[return_LAC_pos+k+1],condition_type_stack);
		}
		VM[VM_length++] = func_VM_pos;
		//if condition statement
		if(strcmp(currtext,"if") == 0)
		{
			printf("Test(v_processeur):find if\n");
			push(VM_length,condition_stack);
			VM_length++;//leave the space in VM for condition jump
			s_push(temp_type_stack, if_type_s_stack);//push the before-if type state
			a_push(func_input, func_input_a_stack);
			push(func_input_num, func_input_num_stack);
			
		}
		else if(strcmp(currtext,"else") == 0)
		{
			processeur_state = CON_ELSE;
			int con_pos;
			//find the postion of "if"
			pop(&con_pos,condition_stack);
			VM[con_pos] = VM_length;
			printf("Test(v_processeur):find else and write else's postion %d to if'position %d\n",
				VM_length,con_pos);
			push(VM_length,condition_stack);
			VM_length++;
			stack *temp_s = create_stack();
			int temp_func_input[10];
			int temp_func_num;
			s_pop(temp_s, if_type_s_stack);
			a_pop(temp_func_input, func_input_a_stack);
			pop(&temp_func_num,func_input_num_stack);
			s_push(temp_type_stack, if_type_s_stack);
			a_push(func_input, func_input_a_stack);
			push(func_input_num, func_input_num_stack);
			temp_type_stack = temp_s;//reuse the state just before if
			func_input_num = temp_func_num;
			int n;
			for(n=0;n<10;n++)
			{
				func_input[n] = temp_func_input[n];
				printf("%d ",func_input[n]);
			}
			printf("\n");
			printf("%d\n", func_input_num);

		}
		else if(strcmp(currtext,"then") == 0)
		{
			processeur_state = MODE_COMPILER;
			int con_pos;
			pop(&con_pos,condition_stack);
			VM[con_pos] = VM_length;
			//there is must a function after then
			//e.g. fin
			stack *temp_aif_stack = create_stack();//the correspond state of after-if
			int temp_func_input[10];
			int temp_func_num;
			s_pop(temp_aif_stack,if_type_s_stack);
			a_pop(temp_func_input, func_input_a_stack);
			pop(&temp_func_num,func_input_num_stack);
			if(!stack_compare(temp_aif_stack,temp_type_stack))
			{
#ifdef PROCESSEUR_DEBUG
				printf("Test(v_processeur):two branch diff stack\n");
#endif
				condition_flag = 0;//can match
			}	
			int i;
			for(i=0;i<func_input_num;i++)
			{
				if(func_input[i] != temp_func_input[i])
					condition_flag = 0;
#ifdef PROCESSEUR_DEBUG
				printf("Test(v_processeur):two branch:%d %d\n",
					func_input[i], temp_func_input[i]);
#endif
			}
		}

	
	}
	else
	{
		var_type = test_type(currtext);
		if(var_type == INT)
		{
			//if(processeur_state != CON_ELSE)
			push(INT,temp_type_stack);
			VM[VM_length++] = 0;//function lit
			VM[VM_length++] = atoi(currtext);
#ifdef PROCESSEUR_DEBUG
			printf("Test(v_processer):add %s in VM\n",currtext);
#endif
		}
		else if(var_type == CHAIN)
		{
			push(CHAIN,temp_type_stack);
			VM[VM_length++] = 2;//function str
			int chain_len_pos = VM_length;
			VM_length++;//keep the space for chain length
			int chain_len = 0;
			int l = 2;//skip "_
			while(currtext[l] != '\0')//put string in VM
			{
				chain_len++;
				VM[VM_length++]=currtext[l++];
			}
			chain_len--;
			VM_length--;//elimine the "
			VM[chain_len_pos] = chain_len;
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

void exit_processer()
{
	
}

int runtime()
{
	return 0;
}

void processer(D_linklist* lex_list)
{//mode de calculatrice
int func_LAC_pos,para_LAC_pos,return_LAC_pos,para_type,func_VM_pos;
pfunc_base function;
int var_type;
int k=0;
int type_test_flag=1;
int error_rep;

if(!D_to_begin(lex_list)) return;//list empty
processeur_state = MODE_CALCULATER;
processor_error_flag = 0;
do
{
	currtext = lex_list->fence->content->value;
	if(currtext[0]==':' && currtext[1]=='\0')
	{
		D_to_next(lex_list);//skip ':'
		printf("Test(processer):enter v_processer:\n");
		error_rep = v_processer(lex_list);//define a function
		processeur_state = MODE_CALCULATER;
		if(!error_rep)
		{
			printf("Test(processer):Defined the function\n");
			if(!D_to_next(lex_list))//
				break;
		}
	}

	currtext = lex_list->fence->content->value;
	if(test_func(currtext,&func_LAC_pos))
	{
		para_LAC_pos = func_LAC_pos + 1 + LAC[func_LAC_pos];
		return_LAC_pos = para_LAC_pos + 1 + LAC[para_LAC_pos];
		func_VM_pos = LAC[return_LAC_pos + 1 + LAC[return_LAC_pos]];
#ifdef PROCESSEUR_DEBUG
		printf("Test(processer): func_LAC_pos:%d, parameter:%d, return:%d, VM_pos:%d\n",
#endif
			func_LAC_pos, para_LAC_pos, return_LAC_pos, func_VM_pos);

		int para_temp_array[10];//to store the type to reput in stack
		for(k=0;k<LAC[para_LAC_pos];k++)
		{
			if(pop(&para_type,type))
			{
				para_temp_array[k] = para_type;
				if(para_type!=LAC[para_LAC_pos+k+1] && LAC[para_LAC_pos+k+1]!=ANY)
				{
					type_test_flag = 0;
					printf("[ERROR]: sementic fault, wrong type for function %s\n",currtext);
					break;
				}
			}
			else
			{
				k--;
				type_test_flag = 0;
				printf("[ERROR]: sementic fault, lack of parametter for function %s\n",currtext);
				break;
			}
		}
		for(;k>0;k--)//re-put types in stack
		{
			push(para_temp_array[k],type);
		}
		if(type_test_flag)//if function has right input type
		{
//			for(k=0;k<LAC[return_LAC_pos];k++)
//			{
//				push(LAC[return_LAC_pos+1+k],type);
//			}
			if(VM[func_VM_pos]==0)//fonction de base
			{
#ifdef PROCESSEUR_DEBUG
				printf("Test: VM pos %d\n",func_VM_pos);
#endif
				function = processeur[VM[func_VM_pos+1]];
				printf("execute function %s\n",currtext);
				function();
				if(processor_error_flag)//have error
					return;
			}
			else if(VM[func_VM_pos]==1)//fonction defini en LAC
			{//begin runtime mode
				int VM_processer;//the position of the processer in VM
				push(func_VM_pos + 1,retour);
				while(stack_get_top(&VM_processer,retour))
				{
					if(VM[VM[VM_processer]] == 0) // if basic function
					{
						function = processeur[VM[VM[VM_processer] + 1]];
#ifdef PROCESSEUR_DEBUG
						printf("Test(processer): execute VM pos %d\n",VM[VM_processer]);
#endif
						function();
					}
					else if(VM[VM[VM_processer]] == 1)
					{
						//int temp;
						//pop(&temp,retour);
						//push(temp + 1,retour);
						push(VM[VM_processer] + 1,retour);
					}
					else
					{
						printf("[ERROR](processeur):wrong VM_number %d.\n", VM_processer);
						exit(0);
					}
				}
			}
		}

	}
	else//if currtext is a variable
	{
		var_type = test_type(currtext);
		if(var_type == INT)
		{
			push(INT,type);
			push(atoi(currtext),data);
			printf("pushed %s\n",currtext);
		}
		else if(var_type == CHAIN)
		{
			push(CHAIN,type);
			int m = 2;//skip the "_
			int chain_add = chain_memory_length;
			int chain_len = 0;
			chain_memory_length++;//keep space for the length of chain
			while(currtext[m] != '\0')
			{
				chain_len++;
				chain_memory[chain_memory_length++] = currtext[m++];
			}
			chain_len--;chain_memory_length--;//elimine " at the end
			chain_memory[chain_add] = chain_len;
			push(chain_add,data);
#ifdef PROCESSEUR_DEBUG
			printf("Test(processer):pushed %d char: %s at address %d\n",
				chain_len, currtext, chain_add);
#endif
		}
		else printf("[ERROR]: sementic fault, %s is not a legal type\n",currtext);
	}
#ifdef PROCESSEUR_DEBUG
	printf("Test(processer):current type stack\n");
	print_stack(type);

	printf("Test(processer):current data stack\n");
	print_stack(data);
#endif
	
}while(D_to_next(lex_list));

}

void VM_LAC_init()
{
	//int get_LAC(char *func_base_str,int num_e,int *type_e,int num_s,int *type_s,pfunc_base f)
	processeur_length = 0;
	VM_length = 0;
	LAC_length = 0;
	int inputval[10],outputval[10];
	int k=0;

	LAC[LAC_length++] = 0;
	for(k=0;k<PROCESSEUR_LENGTH-1;k++)
		processeur[k] = NULL;

//int func2LAC(int funcptpos,pfunc_base f,int VM_pos,char *func_base_str,
//		int num_e,int *type_e,int num_s,int *type_s)
	func2LAC(0,lit,-1,"lit",0,inputval,0,outputval);//order = 0
	func2LAC(1,str,-1,"str",0,inputval,0,outputval);//order = 1
	func2LAC(2,fin,-1,"fin",0,inputval,0,outputval);//order = 2 

	inputval[0]=INT;
	outputval[0]=INT;
	func2LAC(3,point,-1,".",1,inputval,0,outputval);
	
	inputval[0]=INT;inputval[1]=INT;
	outputval[0]=INT;
	func2LAC(4,add,-1,"+",2,inputval,1,outputval);
	func2LAC(5,minus,-1,"-",2,inputval,1,outputval);
	func2LAC(6,multiply,-1,"*",2,inputval,1,outputval);
	func2LAC(16,divide,-1,"/",2,inputval,1,outputval);
	
	inputval[0]=INT;inputval[1]=INT;
	outputval[0]=INT;
	func2LAC(7,equal,-1,"=",2,inputval,1,outputval);
	
	func2LAC(8,dup,-1,"dup",0,inputval,0,outputval);
	func2LAC(9,drop,-1,"drop",0,inputval,0,outputval);
	func2LAC(10,swap,-1,"swap",0,inputval,0,outputval);

	inputval[0]=CHAIN;
	outputval[0]=CHAIN;outputval[1]=INT;
	func2LAC(11,count,-1,"count",1,inputval,2,outputval);
	
	inputval[0]=INT;inputval[1]=CHAIN;
	func2LAC(12,func_type,-1,"type",2,inputval,0,outputval);
	inputval[0]=INT;
	func2LAC(13,LAC_if,-1,"if",1,inputval,0,outputval);

	func2LAC(14,LAC_else,-1,"else",0,inputval,0,outputval);

	func2LAC(15,LAC_then,-1,"then",0,inputval,0,outputval);
}
#endif
/*
int main(int argc, char* argv[])
{
	int i=0;
	char input_text[100];
	D_linklist* ana_lex = create_D_list(0,"");
	data = create_stack();
	type = create_stack();
	retour = create_stack();

	VM_LAC_init();

	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");
	
	while(1)
	{
		printf(">>");
		scanf("%99[^\n]",input_text);
		getchar();
		analex(input_text,ana_lex);
		processer(ana_lex);
		stack_clear(data);stack_clear(type);stack_clear(retour);
		Dlist_clear(ana_lex);


	printf("Test: LAC:");
	for(i=0;i<LAC_length;i++) printf("%d ",LAC[i]);
	printf("\n");
	printf("Test: VM:");
	for(i=0;i<VM_length;i++) printf("%d ",VM[i]);
	printf("\n");
	}	
	return 0;
}
*/
