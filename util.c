#ifndef UTIL_C_
#define UTIL_C_
#include "util.h"

/**************

**************/
D_listnode* create_Dlist_node(vartypes i_type,char* i_s)
{
	node_content *con = (node_content*)malloc(sizeof(node_content));
	D_listnode *n = (D_listnode*)malloc(sizeof(D_listnode));
	int k=0;
	while(i_s[k]!='\0')
	{
		con->value[k]=i_s[k];
		k++;
	}
	con->value[k]='\0';
	con->type = i_type;
	n->content = con;
	n->next = NULL;
	n->pre = NULL;
	return n;
}

D_linklist* create_D_list(vartypes i_type,char* i_s)
{
	D_listnode *h = create_Dlist_node(i_type,i_s);
	D_linklist *D_list = (D_linklist*)malloc(sizeof(D_linklist));

	D_list->head = h;
	D_list->fence = h;
	D_list->tail = h;
	D_list->length = 0;
	D_list->pos = 0;
}

int append_Dlist_node(D_linklist* list,vartypes i_type,char* i_s)
{
	D_listnode* new_node =  create_Dlist_node(i_type,i_s);
	list->tail->next = new_node;
	new_node->pre = list->tail;
	list->tail = new_node;
	(list->length)++;
	return 0;
}

int D_to_next(D_linklist* list)
{
	if(list->pos>=(list->length-1) || list->length==0) return 0;
	else
	{
		list->fence = list->fence->next;
		(list->pos)++;
		return 1;
	}
}

int D_to_pre(D_linklist* list)
{
	if(list->pos<=0 || list->length==0) return 0;
	else
	{
		list->fence = list->fence->pre;
		(list->pos)--;
		return 1;
	}
}

int D_to_begin(D_linklist* list)
{
	if(list->length==0) return 0;
	else
	{
		list->fence = list->head->next;
		list->pos = 0;
		return 1;
	}
}

int D_to_end(D_linklist* list)
{
	if(list->length==0) return 0;
	else
	{
		list->fence = list->tail;
		list->pos = list->length-1;
		return 1;
	}
}

int D_print_list(D_linklist* list)
{
	int i;
	D_to_begin(list);
	printf("Print List:\n");
	for(i=0;i<(list->length);i++)
	{
		printf("%s\n",list->fence->content->value);
		D_to_next(list);
	}
	return 1;
}

int Dlist_clear(D_linklist* list)
{
	list->fence = list->head;
	list->tail = list->head;
	list->pos = 0;
	while(list->length > 0)
	{
		D_listnode *temp_n =list->head->next;
		list->head->next = list->head->next->next;
		list->length--;
		free(temp_n);
	}
}

/***********

***********/
tree_node* create_tree_node_helper(node_content *con)
{
	tree_node *n = (tree_node*)malloc(sizeof(tree_node));
	n->content = con;
	n->leftchild = NULL;
	n->rightchild = NULL;
	//n->father = NULL;
	return n;
}

tree_node* create_tree_node(vartypes i_type,char* i_s)
{
	node_content *con = (node_content*)malloc(sizeof(node_content));
	int k=0;
	while(i_s[k]!='\0')
	{
		con->value[k]=i_s[k];
		k++;
	}
	con->value[k]='\0';
	con->type = i_type;
	return create_tree_node_helper(con);
}

int add_leftchild(tree_node *root,tree_node *child)
{
	
}

int add_rightchild()
{

}

#endif

