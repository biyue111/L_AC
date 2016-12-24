#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"
#include "util.c"
#include "analex.h"
#include "analex.c"
#ifndef _CALCULATE_H
#define _CALCULATE_H


int factor(tree_node *root,D_linklist *ana_lex,int *finish);
int term(tree_node *root,D_linklist *ana_lex_list,int *finish);

#endif
