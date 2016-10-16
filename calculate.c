#include <stdio>
#include "util.h"
#include "util.c"

/***************
from right to left
<term> ::= [+|-][<term>(+|-)]<factor>
<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)
***************/

vartypes verify_type(char *s) //return the type
{

}


int term(tree_node *root,D_linklist *ana_lex_list)
{
	int flag = 1,moved;
	node_content *con;
	flag = factor(root->rightchild,ana_lex_list);
	if(flag && D_to_pre(ana_lex_list))//successfully move to pre
	{
		con = ana_lex_list->fence->content;
		con->type = verify_type(con->value);
		if(con->type==OP && (con->value[0]=='+'||con->value[0]=='-'))
		{
			root->content = con;
			
			if(flag && D_to_pre(ana_lex_list))
			{
				flag = term(root->leftchild,ana_lex_list);
			}
			else if(flag)
			{
			}
		}
		else //the next segment is illegal.
		{
			flag = 0;
			printf("[ERROR]");
		}
	}
	else if(flag)
	{

	}
	return flag;
}

//<factor> ::= [<factor>(x|/)](<nature>|”(”<term>”)”)

int facter(tree_node *root,D_linklist *ana_lex)
{
	
}

int ana_syn()
{

}




int main(int argc, char* argv[])
{
	
}
