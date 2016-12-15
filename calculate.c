#include "calculate.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include "util.h"
//#include "util.c"
/***************
from right to left
<term> ::= [+|-][<term>(+|-)]<factor>
<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)
***************/

vartypes verify_type(char *s) //return the type
{

}

int nature(tree_node *root,D_linklist *ana_lex_list,int *finish)
{
	int flag = 1,moved;
	node_content *con;
	//if(flag && D_to_pre(ana_lex_list))//successfully move to pre
	{
		con = ana_lex_list->fence->content;
		printf("Test(nature):get %s\n",con->value);
		if(isdigit(con->value[0]) && con->value[0]!='0')
		{
			printf("Test(nature):right nature %s\n",con->value);
			memcpy(root->content,con,sizeof(node_content));
			if(!D_to_pre(ana_lex_list))
			{
				*finish = 1;
				return 1;
			}
		}
		else
		{
			flag = 0;
			printf("[ERROR](nature): not a nature %s\n",con->value);
		}
	}
	return flag;
}
int term(tree_node *root,D_linklist *ana_lex_list,int *finish)
{
	int flag = 1,moved;
	node_content *con;
	tree_node *r_c = create_tree_node(0, "");
	root->rightchild = r_c;
	flag = factor(root->rightchild,ana_lex_list,finish);
	if((*finish)==1)//finish in factor
		return 1;
	//if(flag && D_to_pre(ana_lex_list))//successfully move to pre
	{
		con = ana_lex_list->fence->content;
		printf("Test(term):get %s\n",con->value);
		//con->type = verify_type(con->value);
		//if(con->type==OP && (con->value[0]=='+'||con->value[0]=='-'))
		if((con->value[0]=='+'||con->value[0]=='-'))
		{
			memcpy(root->content,con,sizeof(node_content));
			
			if(flag && D_to_pre(ana_lex_list))
			{
				tree_node *l_c = create_tree_node(0,"");
				root->leftchild = l_c;
				flag = term(root->leftchild,ana_lex_list,finish);
				if((*finish)==1)
					return 1;
			}
			else if(flag)//finish
			{
				*finish = 1;
				printf("Test(term):only have +- at begin\n");
				return flag;
			}
		}
		else if(strcmp(con->value,"(")==0)
		{//keep the position on (
		 //function factor will test it
			return flag;
		}
		else //the next segment is illegal.
		{
			flag = 0;
			printf("[ERROR](term): illegal segment %s\n",con->value);
		}
	}
	//else if(flag)//finish
	//{
	//	printf("[ERROR](term): Wrong enter in term\n");
	//	flag = 0;
	//}

	return flag;
}

//<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)

int factor(tree_node *root,D_linklist *ana_lex,int *finish)
{
	int flag = 1,moved;
	node_content *con;
	//if(!(flag && D_to_pre(ana_lex)))//successfully move to pre
	//{
	//	return 0;
	//}
	con = ana_lex->fence->content;
	tree_node *r_c = create_tree_node(0,"");
	root->rightchild = r_c;
	printf("Test(factor):get %s\n",con->value);
	if(strcmp(con->value,")")==0)
	{
		if(D_to_pre(ana_lex))//move to previous node
		{

			flag = term(root->rightchild,ana_lex,finish);
			if(flag == 0)
				printf("[ERROR]:Wrong term\n");
		}
		else
		{
			flag = 0;
			printf("[ERROR]:open parentese, only has close\n");
		}
		con = ana_lex->fence->content;
		if(strcmp(con->value,"(")==0)
		{
			if(!D_to_pre(ana_lex))
			{
				*finish = 1;
				return 1;
			}
		}
		else
		{
			printf("[ERROR]:open parentese, don't have open\n");
			flag = 0;
		}
	}
	else if(nature(root->rightchild,ana_lex,finish))
	{ 
		if((*finish)==1)
			return 1;
	
	}
	else
	{
		printf("[ERROR]:Wrong input in factor %s\n",con->value);
	}

	if(flag)
	{
		con = ana_lex->fence->content;
		printf("Test(factor):After nature, get %s\n",con->value);
		if((con->value[0]=='*'||con->value[0]=='/'))
		{
			memcpy(root->content,con,sizeof(node_content));
			if(!D_to_pre(ana_lex))
			{
				*finish = 1;
				printf("[ERROR]: not much %s\n",con->value);
				return 0;
			}
			tree_node *l_c = create_tree_node(0,"");
			root->leftchild = l_c;
			flag = factor(root->leftchild,ana_lex,finish);
		}
		else
		{
			return flag;
		}
	}
	return flag;
}

int ana_syn()
{

}

void post_view_tree(tree_node *root)
{
	if(root != NULL)
	{
		post_view_tree(root->leftchild);
		post_view_tree(root->rightchild);
		if(root->content->value[0]!='\0')
			printf("%s ", root->content->value);
	}
}


//void analex(char *text, D_linklist* ana_lex )
int calculate(char *text)
{
	D_linklist *ana_lex = create_D_list(0,"");
	tree_node *root_node = create_tree_node(0,"");
	analex(text, ana_lex);
	
	//printf("Test(calculate):reslut of analex\n");
	//print_D_linklist(ana_lex);
	int tree_created_flag;
	int finish_flag=0;
	D_to_end(ana_lex);
	tree_created_flag = term(root_node, ana_lex,&finish_flag);
	post_view_tree(root_node);
	printf("\n");
}


int main(int argc, char* argv[])
{
	char inputstr[100];
	scanf("%99[^\n]",inputstr);
	getchar();
	calculate(inputstr);
}
