#include "calculate.h"
/***************
from right to left
<term> ::= [+|-][<term>(+|-)]<factor>
<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)
***************/

vartypes verify_type(char *s) //return the type
{

}

int nature(tree_node *root,D_linklist *ana_lex_list)
{
	int flag = 1,moved;

}
int term(tree_node *root,D_linklist *ana_lex_list)
{
	int flag = 1,moved;
	node_content *con;
	tree_node *r_c = create_tree_node();
	root->rightchild = r_c;
	flag = factor(root->rightchild,ana_lex_list);
	if(flag && D_to_pre(ana_lex_list))//successfully move to pre
	{
		con = ana_lex_list->fence->content;
		//con->type = verify_type(con->value);
		//if(con->type==OP && (con->value[0]=='+'||con->value[0]=='-'))
		if((con->value[0]=='+'||con->value[0]=='-'))
		{
			memcpy(root->content,con,sizeof(node_content));
			
			if(flag && D_to_pre(ana_lex_list))
			{
				tree_node *l_c = create_tree_node();
				root->leftchild = l_c;
				flag = term(root->leftchild,ana_lex_list);
			}
			else if(flag)//finish
			{
				printf("Test(term):only have +- at begin\n");
			}
		}
		else //the next segment is illegal.
		{
			flag = 0;
			printf("[ERROR](term): illegal segment %s\n",con->value);
		}
	}
	else if(flag)//finish
	{
		printf("[ERROR](term): Wrong enter in term\n");
		flag = 0;
	}

	return flag;
}

//<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)

int factor(tree_node *root,D_linklist *ana_lex)
{
	int flag = 1,moved;
	node_content *con;
	con = ana_lex->fence->content;
	tree_node *r_c = create_tree_node();
	root->rightchild = r_c;
	if(strcmp(con->value,")")==0)
	{
		if(D_to_pre(ana_lex_list))//move to previous node
		{

			flag = term(tree_node *root->rightchild,D_linklist *ana_lex);
			if(flag == 0)
				printf("[ERROR]:Wrong term\n");
		}
		else
		{
			flag = 0;
			printf("[ERROR]:open parentese, only has close\n");
		}
		if(!(strcmp(con->value,"(")==0))
		{
			printf("[ERROR]:open parentese, don't have open\n");
			flag = 0;
		}
	}
	else if(nature(tree_node *root->rightchild,D_linklist *ana_lex))
	{

	}
	else
	{
		printf("[ERROR]:Wrong input in factor %s\n",con->value);
	}

	if(flag && D_to_pre(ana_lex))
	{
		if((con->value[0]=='x'||con->value[0]=='/'))
		{
			memcpy(root->content,con,sizeof(node_content));
			flag = factor(root->leftchild, ana_lex);
		}
		else
			return flag;
	}
	return flag;
}

int ana_syn()
{

}


//void analex(char *text, D_linklist* ana_lex )
int calculate(char *text)
{
	D_linklist *ana_lex = create_D_list();
	tree_node *root_node = create_tree_node();
	analex(text, ana_lex);
	
	printf("Test(calculate):reslut of analex");
	print_D_linklist(ana_lex);
	int tree_created_flag;
	tree_created_flag = term(root_node, ana_lex);
}


int main(int argc, char* argv[])
{
	
}
