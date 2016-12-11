#ifndef UTIL_H_
#define UTIL_H_
#include <string.h>
/*****

*****/

typedef enum vartypes
{
	ANY,
	INT,
	CHAIN,
	INDI,
	OP,
	UNDEF
}vartypes;

typedef struct node_content
{
	char value[20];
	vartypes type;
}node_content;

/*****

*****/



typedef struct d_linklist_node
{
	node_content *content;
	struct d_linklist_node *next;
	struct d_linklist_node *pre;
}D_listnode;

typedef struct d_linklist
{
	D_listnode *head;
	D_listnode *fence;
	D_listnode *tail;
	int length;
	int pos;
}D_linklist;

/*****

*****/

typedef struct tree_node
{
	node_content *content;
	struct tree_node *leftchild;
	struct tree_node *rightchild;
	
}tree_node;

#endif
