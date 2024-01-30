%{
#include <stdio.h>
#include "ast.h"
#include "compiler.tab.h"
#include "print_tree_gui.h"


extern int yylex();
extern Node *top;
%}
%union{
 Node *np;
 int ival;
 char *sp;
}

%token DEFINE ARRAY WHILE IF ELSE SEMIC
%token L_BRACKET R_BRACKET L_PARAN R_PARAN 
%token L_BRACE R_BRACE ASSIGN MUL DIV
%token EQ LT GT 
%token <sp> IDENT 
%token <ival> NUMBER ADD SUB

%type <np> program expression statements statement term factor condition 
%type <np> var declarations decl_statement  assignment_stmt loop_stmt cond_stmt
%type <ival> add_op mul_op cond_op


%%
program : declarations statements {top=buildNode2(PROG_AST,$1,$2);}
;
declarations : decl_statement declarations {$$=buildNode2(DEF_AST,$1,$2);}| decl_statement {$$=buildNode2(DEF_AST,$1,NULL);}
;
decl_statement : DEFINE IDENT SEMIC {$$=buildIdentNode(IDENT_AST,$2);}
;
statements : statement statements {$$=buildNode2(STATS_AST,$1,$2);}
| statement {$$=buildNode2(STATS_AST,$1,NULL);}
;
statement : assignment_stmt {$$=$1;}
| loop_stmt {$$=$1;}
| cond_stmt {$$=$1;}
;
assignment_stmt : var ASSIGN expression SEMIC {$$=buildNode2(ASSIGN_AST,$1,$3);}
/*| IDENT L_BRACKET NUMBER R_BRACKET ASSIGN expression SEMIC {$$=buildNode2(ASSIGN_AST,buildArrayNode(ARRAY_AST,$1,$3),$6);}*/
;
expression : expression add_op term {$$ = buildNode2($2,$1,$3);}
| term {$$=$1;}
;
term : term mul_op factor {$$=buildNode2($2,$1,$3);}
| factor {$$=$1;}
;
factor : var {$$=$1;}
| L_PARAN expression R_PARAN {$$=$2;}
;
add_op : ADD {$$=ADD_AST;}| SUB {$$=SUB_AST;}
;
mul_op : MUL {$$=MUL_AST;}| DIV {$$=DIV_AST;}
;
var : IDENT {$$ = buildIdentNode(IDENT_AST,$1);}
| NUMBER {$$ = buildNumNode(NUM_AST,$1);}
/*| IDENT L_BRACKET NUMBER R_BRACKET {$$ = buildArrayNode(ARRAY_AST,$1,$3);}*/
;
loop_stmt : WHILE L_PARAN condition R_PARAN L_BRACE statements R_BRACE {$$=buildNode2(WHILE_AST,$3,$6);}
;
cond_stmt : IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE {$$=buildNode2(IF_AST,$3,$6);}
| IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE ELSE L_BRACE statements R_BRACE {buildNode2(IF_AST,$3,buildNode2(ELSE_AST,$6,$10));}
;
condition : expression cond_op expression {$$=buildNode2($2,$1,$3);}
;
cond_op : EQ {$$=EQ_AST;}|LT {$$=LT_AST;}|GT {$$=GT_AST;} 
;


%%
/*
int main(void){
Node *n0, *n1, *n2, *n3;
n2 = buildIdentNode(IDENT_AST, "a");
n3 = buildNumNode(NUM_AST, 5);
n1 = buildNode2(ASSIGN_AST, n2, n3);
n0 = buildNode2(PROG_AST, n1, n2);
printNodes(n2);
}*/

int main(void)
{ 
 if(yyparse()){
  fprintf(stderr,"Error\n");
  return 1;
 }

 codeInit();
 codeGeneration(top);
 codeEnd();
 
 //printTreeGUI(top);
 //printf("\n");
 //printNodes(parse_result);
}
