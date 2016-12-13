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
	return D_list;
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
	return 0;
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
	return 0;
}

int add_rightchild()
{
	return 0;
}
/*************
*************/

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
	if(i_stack->length > 0)
	{
		*pnum = i_stack->top->val;
		return 1;
	}
	else return 0;
}

int stack_compare(stack *s1, stack *s2)
{

	printf("Test(stack_compare):length%d %d\n",s1->length,s2->length);

	if(s1->length != s2->length)
		return 0;
	stack_node *n1, *n2;
	n1 = s1->top;
	n2 = s2->top;
	while(n1 != NULL)
	{
		if(n1->val != n2->val)
		{
			printf("Test(stack_compare):%d %d\n",n1->val,n2->val);
			return 0;
		}
		n1 = n1->next;
		n2 = n2->next;
	}
	return 1;
}

void print_stack(stack* i_stack)
{
	stack_node *temp_n = i_stack->top;
	while(temp_n != NULL)
	{
		printf("%d ",temp_n->val);	
		temp_n = temp_n->next;
	}
	printf("\n");
}

int stack_clear(stack* i_stack)
{
	while(i_stack->length > 0)
	{
		stack_node *temp_n = i_stack->top;
		i_stack->top = i_stack->top->next;
		i_stack->length--;
		free(temp_n);
	}
	return 0;
}

/*************
* the stack for int array
*************/
typedef struct a_stack_node{
	int a[10];
	struct a_stack_node *next;
}a_stack_node;

typedef struct a_stack{
	a_stack_node *top;
	int length;
}a_stack;

a_stack_node* create_a_stack_node(int (*i_a)) 
{
	a_stack_node *sn = (a_stack_node*)malloc(sizeof(a_stack_node));
	int i;
	for(i=0;i<10;i++)
		sn->a[i] = i_a[i];
	sn->next = NULL;
	return sn;
}

a_stack* create_a_stack()
{
	a_stack *s = (a_stack*)malloc(sizeof(a_stack));
	s->top = NULL;
	s->length = 0;
	return s;
}

int a_push(int (*a) ,a_stack* i_stack)
{
	a_stack_node *temp_n = create_a_stack_node(a);
	temp_n->next = i_stack->top;
	i_stack->top = temp_n;
	(i_stack->length)++;
	return 1;
}

int a_pop(int (*p_a) ,a_stack* i_stack)
{
	if(i_stack->length>0)
	{
		int i = 0;
		for(i=0;i<10;i++)
			p_a[i] = i_stack->top->a[i];
		a_stack_node *torel = i_stack->top;
		i_stack->top = i_stack->top->next;
		free(torel);
		(i_stack->length)--;
		return 1;
	}
	else return 0;
}

int a_stack_get_top(int (*p_a) ,a_stack* i_stack)//get the top of stack
{
	if(i_stack->length > 0)
	{
		p_a = i_stack->top->a;
		return 1;
	}
	else return 0;
}

int a_stack_clear(a_stack* i_stack)
{
	while(i_stack->length > 0)
	{
		a_stack_node *temp_n = i_stack->top;
		i_stack->top = i_stack->top->next;
		i_stack->length--;
		free(temp_n);
	}
	return 0;
}
/*********
*********/
typedef struct s_stack_node{
	stack *s;
	struct s_stack_node *next;
}s_stack_node;

typedef struct s_stack{
	s_stack_node *top;
	int length;
}s_stack;

s_stack_node* create_s_stack_node(stack *i_s)
{
	s_stack_node *sn = (s_stack_node*)malloc(sizeof(s_stack_node));
	sn->s = create_stack();
	memcpy(sn->s,i_s,sizeof(stack));
	sn->next = NULL;
	return sn;
}

s_stack* create_s_stack()
{
	s_stack *s = (s_stack*)malloc(sizeof(s_stack));
	s->top = NULL;
	s->length = 0;
	return s;
}

int s_push(stack *i_s,s_stack* i_stack)
{
	s_stack_node *temp_n = create_s_stack_node(i_s);
	temp_n->next = i_stack->top;
	i_stack->top = temp_n;
	(i_stack->length)++;
	return 1;
}

int s_pop(stack *p_s,s_stack* i_stack)
{
	if(i_stack->length>0)
	{
		memcpy(p_s,i_stack->top->s,sizeof(stack));
		s_stack_node *torel = i_stack->top;
		i_stack->top = i_stack->top->next;
		free(torel);
		(i_stack->length)--;
		return 1;
	}
	else return 0;
}

int s_stack_get_top(stack *p_s,s_stack* i_stack)//get the top of stack
{
	if(i_stack->length > 0)
	{
		p_s = i_stack->top->s;
		return 1;
	}
	else return 0;
}

int s_stack_clear(s_stack* i_stack)
{
	while(i_stack->length > 0)
	{
		s_stack_node *temp_n = i_stack->top;
		i_stack->top = i_stack->top->next;
		i_stack->length--;
		free(temp_n);
	}
	return 0;
}
#endif

