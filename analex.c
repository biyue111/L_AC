#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "analex.h"
#include "util.h"
#include "util.c"

#ifndef _ANALEX_C
#define _ANALEX_C
int test_quotation(const char *p, int *begin,const int pre,const int pos);
void TestFun(char *p,int *start,int *end,D_linklist* arr);
void DelCommentaryFun(char *p,int *start,int *end,D_linklist* arr);
typedef void (*HandleFun)(char*,int*,int*,D_linklist*);
void FindHandleRegec(HandleFun H_fun,char *c_regexp,char *p,D_linklist* arr);

//int test_quotation(const char *p, int *begin,const int pre,const int pos)/*{{{*/
//{//return 0, within the "", return 1 out ""
//	int end = *begin,flag = 1;
//	int i;
//	//no quo after
//	for(i=pos;p[i]!='\0';i++)
//	{
//		if (p[i]=='"') break;
//	}
//	if(p[i]=='\0') return 0;
//
//	//find pair " before ( or inverse slash 
//	for(i=*begin;i<=pre;i++)
//	{
//		if (p[i]=='"')
//		{
//			if(flag)
//				flag = 0;
//			else
//			{
//				flag = 1;
//				*begin = i+1;
//			}
//		}
//	}
//	return flag;
//}/*}}}*/

void TestFun(char *p,int *start,int *end,D_linklist* arr)
{
	char substr[100];
	int s_p = *start,e_p = *end;
	substr[0] = '\0';
	strncpy(substr,p + s_p,e_p - s_p);
	substr[e_p - s_p]= '\0';
	printf("%s : start %d, End %d\n",substr,s_p,e_p);
}

void del_substr(char *p,int *start,int *end)
{
	int s_p = *start,e_p = *end;
	char *l = p + s_p, *r = p + e_p;
	printf("TEXT: delet: %d--%d\n",s_p,e_p);
	while(*r != '\0')
	{
		*l = *r;
		l++;r++;
	}
	*l = '\0';
	*end = *start;
}

void DelCommentaryFun(char *p,int *start,int *end,D_linklist* arr)
{
	//printf("[TEST]frist is '%c'\n",p[*start]);
	if(p[*start]!='"')
	{
		if(p[*start]=='\n' || p[*start]==' ') (*start)++;
		del_substr(p,start,end);
	}
}

void FindIndiFun(char *p,int *start,int *end,D_linklist* arr)
{
	int j=0,k=0;
	char tempstr[20];
	//while(arr[i][0]!='\0') i++;
	for(k=*start;k<*end;k++)
		tempstr[j++] = p[k];
	tempstr[j]='\0';
	if(isdigit(tempstr[0]))
	{
		append_Dlist_node(arr,INT,tempstr);
	}
	else if(isalpha(tempstr[0]) || tempstr[0]=='_')
	{
		append_Dlist_node(arr,INDI,tempstr);
	}
	else if(tempstr[0]=='\"')
	{
		append_Dlist_node(arr,CHAIN,tempstr);
	}
	else
	{
		append_Dlist_node(arr,OP,tempstr);
	}
	
}


void FindHandleRegec(HandleFun H_fun,char *c_regexp,char *p,D_linklist *arr) //if test_quo == 0 not use test_quotation function
{
	
	int res = 1,start = 0,end = 0;
	//int test_quo_pos = 0;
	regex_t regexp;
	regmatch_t pmatch[100];

	if(regcomp(&regexp,c_regexp,REG_EXTENDED))
	//return 0 if Success
	{
		printf("Wrong!!\n");
		regfree(&regexp);
	}
	res = regexec(&regexp,p+end,1,pmatch,0);
	while (res==0)
	{
		printf("TEXT: find one\n");
		start = end + pmatch[0].rm_so;
		end += pmatch[0].rm_eo;

		H_fun(p,&start,&end,arr);

		res = regexec(&regexp,p+end,1,pmatch,0);
	}
	regfree(&regexp);
}

void analex(char *text, D_linklist* ana_lex )
{
	//int i;
	//for(i=0;i<99;i++) ana_lex[i][0] = '\0';
	//D_linklist* ana_lex = create_D_list(INT,"0");
	//FindHandleRegec(TestFun,REG_COMMENTARY_MULTI,p);
	FindHandleRegec(DelCommentaryFun,REG_COMMENTARY,text,NULL);
	FindHandleRegec(DelCommentaryFun,REG_COMMENTARY_MULTI,text,NULL);
	FindHandleRegec(FindIndiFun,REG_FIND_INDI,text,ana_lex);
}
#endif
