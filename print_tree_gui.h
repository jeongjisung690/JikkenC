
/*
* ===================================================
*  Print Tree GUI  ver.1.1.0  (January 30 2024)
* ===================================================
*
*  For more info.
*  https://github.com/smpny7/print-tree-gui
*
*/

#include <stdio.h>

char *node_type_str[] = {
	"IDENT_AST",
	"NUM_AST",
	"ASSIGN_AST",
	"ADD_AST",
	"SUB_AST",
	"MUL_AST",
	"DIV_AST",
	"EQ_AST",
	"LT_AST",
	"GT_AST",
	"WHILE_AST",
	"IF_AST",
	"ELSE_AST",
	"STATS_AST",
	"PROG_AST",
	"ARRAY_AST",
	"DEF_AST",
	"LOOP_AST",
	"COND_AST"
};

/*
* Overview: Output the tree structure.
* @argument: {Node *} np - Parent node.
* @return: No return
*/
void printTreeGUI(Node *np)
{
    printf("{");

    printf("\"type\": \"%s\",", node_type_str[np->ntype]);

    printf("\"varName\": \"%s\",", np->varname ? np->varname : "null");

    printf("\"value\": ");
    np->ntype == NUM_AST  ? printf("\"%d\",", np->value) : printf("\"null\",");

    printf("\"child\": [");
    if (np->child != NULL)
        printTreeGUI((Node *)np->child);
    printf("],");

    printf("\"brother\": [");
    if (np->bro != NULL)
        printTreeGUI((Node *)np->bro);
    printf("]");

    printf("}");
}
