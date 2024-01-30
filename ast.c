#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"ast.h"

/*Note
 *
 * whileのラベル名をかぶらないアルゴリズム考える
 * 現状は、じかにラベル名を与えているため、ラベル名が重複する可能性あり
 *
 *
 */


Node *top = NULL;//astのルートノード用
Table table[100];
int tp=0;


void addTable(Node *p){
  table[tp].varname = p->varname;
  table[tp].size = 1;
  table[tp].addr = 10004000 + 4*tp;
  tp++;
}
/*
void initTable(Table *head){
 head->Ctable = NULL;
}

void makeTable(Node *p){
 if(p != NULL){
   makeTable(p->child);
   makeTable(p->child->bro);
   if(p->ntype == IDENT_AST) addTable(p);
 }
}

void addTable(Node *p){
 int count=0;
 Table *table = (Table *)malloc(sizeof(Table));
 Table *nexttable = headtable;

 while(nexttable->Ctable != NULL){
   count++; nexttable = nexttable->Ctable;
 }
 table->varname = p->varname;
 table->size = 1;//not array = 1, else the size of array
 table->addr = 10004000 + 4*count;
 table->Ctable = NULL;
 nexttable->Ctable = table; 
}
*/

void codeInit(){
/*
 $v0 : 計算結果格納
 $t0~ : 変数格納
 
*/
  printf("\tINITIAL_GP = 0x10008000\n");
  printf("\tINITIAL_SP = 0x7ffffffc\n");
  printf("\tstop_service = 99\n");
  printf("\tla $gp, INITIAL_GP\n");//
  printf("\tla $sp, INITIAL_SP\n");
  printf("\tla $t0, _%s\n",table[1].varname);
  printf("\t.text\n");
}
void codeEnd(){
 printf("\tli $v0, stop_service\n");
 printf("\tsyscall\n");
 printf("\tnop\n");
 printf("STOP:\n");
 printf("\tj STOP\n");
 printf("\tnop\n");
 printf("\t.data 0x10004000\n");
 printf("_%s:\t.word 0xffffffff\n",table[1].varname);
 printf("_%s:\t.word 10\n",table[0].varname);

}

void codeGeneration(Node* p){
// printf("codeGene start\n");
 int i; 
 if(p!=NULL){
   switch(p->ntype){
    case STATS_AST:
     codeGeneration(p->child);
     codeGeneration(p->child->bro);
     break;
    case ASSIGN_AST:
     codeGene_for_expression(p->child->bro);
     printf("\tsw $v0, %d($t0)\n",getOffset(p->child->varname));
     break;
    case WHILE_AST:
     codeGene_for_while(p);
     printf("%s\n","label2: ");
     break;
    case PROG_AST:
     codeGeneration(p->child);
     codeGeneration(p->child->bro);
     break;
    default:
     break;
   }
 }
}

void codeGene_for_cond(Node* p){
 switch(p->ntype){
  case EQ_AST:
   printf("\tlw $v0, %d($t0)\n",getOffset(p->child->varname));//load p->child
   if(p->child->bro->ntype == IDENT_AST){
    printf("\tlw $v1, %d($t0)\n",getOffset(p->child->bro->varname));//load p->child-bro 
   }else{
    printf("\tli $v1, %d\n",p->child->bro->value);
   }
   printf("\tseq $v0, $v0, $v1\n"); 
   break;
 /* case NEQ_AST:
   printf("\tlw $v0, %d($t0)\n",getOffset(p->child->varname));//load p->child
   if(p->child->bro->ntype == IDENT_AST){
    printf("\tlw $v1, %d($t0)\n",getOffset(p->child->bro->varname));//load p->child-bro 
   }else{
    printf("\tli $v1, %d\n",p->child->bro->value);
   }
   printf("\tsne $v0, $v0, $v1\n"); 
   break;*/
  case LT_AST:
    printf("\tlw $v0, %d($t0)\n",getOffset(p->child->varname));//load p->child
   if(p->child->bro->ntype == IDENT_AST){
    printf("\tlw $v2, %d($t0)\n",getOffset(p->child->bro->varname));//load p->child-bro 
    printf("\tslt $v0, $v0, $v1\n");
   }else{//p->child->bro->ntype == NUM_AST
    printf("\tslti $v0, $v0, %d\n",p->child->bro->value);
   }
   break;
  case GT_AST:
    printf("\tlw $v0, %d($t0)\n",getOffset(p->child->varname));//load p->child
   if(p->child->bro->ntype == IDENT_AST){
    printf("\tlw $v1, %d($t0)\n",getOffset(p->child->bro->varname));//load p->child-bro 
    printf("\tslt $v0, $v1, $v0\n");
   }else{//p->child->bro->ntype == NUM_AST
    printf("\tli $v1, %d\n",p->child->bro->value);
    printf("\tslt $v0, $v1, $v0\n");
   }
   break;
 }
}

void codeGene_for_while(Node* p){
 printf("%s\n","label1: ");
 codeGene_for_cond(p->child);//条件式
 printf("\tbeq $v0, $zero, %s\n","label2");
 printf("\tnop\n");
 codeGeneration(p->child->bro);//文集合
 printf("\tj %s\n","label1");
 printf("\tnop\n");
}

void codeGene_for_expression(Node* p){
 /*
 if(p->child != NULL){
   codeGene_for_expression(p->child);
   //push
   printf("addi $sp, $sp, -4\n");
   printf("sw $v0, $sp\n");
   if(p->child->bro != NULL){
     codeGene_for_expression(p->child->bro);
     printf("move $a0, $v0\n");//計算結果$v0を$a0に逃す
   }
   //pop
   printf("lw $v0, $sp\n");
   printf("addi $sp, $sp, 4\n");
 }*/
 if(p == NULL) return;
 switch(p->ntype){
  case ADD_AST:
        codeGene_for_expression(p->child);
       	printf("\taddi $sp, $sp, -4\n");
	printf("\tsw $v0, 0($sp)\n");
 	codeGene_for_expression(p->child->bro);
	printf("\tadd $a0, $v0, $zero\n");
	printf("\tlw $v0, 0($sp)\n");
	printf("\taddi $sp, $sp, 4\n");
	printf("\tadd $v0, $a0, $v0\n");//left + right	
        break;
        /*
	if(p->child->ntype == IDENT_AST && p->child->bro->ntype == IDENT_AST){
 	  printf("lw $v1, %d($t0)\n",getOffset(p->child->varname));
	  printf("lw $v2, %d($t0)\n",getOffset(p->child->bro->varname));		        
          printf("add $v0, $v1, $v2\n");
      	}else if(p->child->ntype == IDENT_AST && p->child->bro->ntype == NUM_AST){
	  printf("lw $v1,%d($t0)\n",getOffset(p->child->varname));
          printf("addi $v0, $v1, %d\n",p->child->bro->value);
  	}else if(p->child->ntype==NUM_AST && p->child->bro->ntype==IDENT_AST){
	  printf("lw $v1, %d($t0)\n",getOffset(p->child->bro->varname));
          printf("addi $v0, $v1, %d\n",p->child->value);
	}break;*/
  case SUB_AST:
        codeGene_for_expression(p->child);
       	printf("\taddi $sp, $sp, -4\n");
	printf("\tsw $v0, 0($sp)\n");
 	codeGene_for_expression(p->child->bro);
	printf("\tadd $a0, $v0, $zero\n");
	printf("\tlw $v0, 0($sp)\n");
	printf("\taddi $sp, $sp, 4\n");
        printf("\tneg $v0, $v0\n");//$v0 = -$v0
	printf("\tadd $v0, $a0, $v0\n");//left - right	
     	break;
	/*
	if(p->child->ntype == IDENT_AST && p->child->bro->ntype == IDENT_AST){
 	  printf("lw $v1, %d($t0)\n",getOffset(p->child->varname));
	  printf("lw $v2, %d($t0)\n",getOffset(p->child->bro->varname));		        
          printf("sub $v0, $v1, $v2\n");
      	}else if(p->child->ntype == IDENT_AST && p->child->bro->ntype == NUM_AST){
	  printf("lw $v1,%d($t0)\n",getOffset(p->child->varname));
          printf("addi $v0, $v1, -%d\n",p->child->bro->value);
  	}else if(p->child->ntype==NUM_AST && p->child->bro->ntype==IDENT_AST){
	  printf("lw $v1, %d($t0)\n",getOffset(p->child->bro->varname));
          printf("addi $v0, $v1, -%d\n",p->child->value);
	}break;*/
  case MUL_AST:
        codeGene_for_expression(p->child);
       	printf("\taddi $sp, $sp, -4\n");
	printf("\tsw $v0, 0($sp)\n");
 	codeGene_for_expression(p->child->bro);
	printf("\tadd $a0, $v0, $zero\n");
	printf("\tlw $v0, 0($sp)\n");
	printf("\taddi $sp, $sp, 4\n");
	printf("\tmul $v0, $a0, $v0\n");//left * right	
	break;
 	/*
	if(p->child->ntype == IDENT_AST && p->child->bro->ntype == IDENT_AST){
 	  printf("lw $v1, %d($t0)\n",getOffset(p->child->varname));
	  printf("lw $v2, %d($t0)\n",getOffset(p->child->bro->varname));		        
      	}else if(p->child->ntype == IDENT_AST && p->child->bro->ntype == NUM_AST){
	  printf("lw $v1,%d($t0)\n",getOffset(p->child->varname));
          printf("li $v2, %d\n",getOffset(p->child->bro->varname));
  	}else if(p->child->ntype==NUM_AST && p->child->bro->ntype==IDENT_AST){
	  printf("lw $v1, %d($t0)\n",getOffset(p->child->bro->varname));
          printf("li $v2, %d\n",getOffset(p->child->varname));
	}
        printf("mul $v0, $v1, $v2\n");
	break;*/
  case DIV_AST:
        codeGene_for_expression(p->child);
       	printf("\taddi $sp, $sp, -4\n");
	printf("\tsw $v0, 0($sp)\n");
 	codeGene_for_expression(p->child->bro);
	printf("\tadd $a0, $v0, $zero\n");
	printf("\tlw $v0, 0($sp)\n");
	printf("\taddi $sp, $sp, 4\n");
	printf("\tdiv $v0, $a0, $v0\n");//left / right	
	break;
	/*
	if(p->child->ntype == IDENT_AST && p->child->bro->ntype == IDENT_AST){
 	  printf("lw $v1, %d($t0)\n",getOffset(p->child->varname));
	  printf("lw $v2, %d($t0)\n",getOffset(p->child->bro->varname));		        
      	}else if(p->child->ntype == IDENT_AST && p->child->bro->ntype == NUM_AST){
	  printf("lw $v1,%d($t0)\n",getOffset(p->child->varname));
          printf("li $v2, %d\n",getOffset(p->child->bro->varname));
  	}else if(p->child->ntype==NUM_AST && p->child->bro->ntype==IDENT_AST){
	  printf("lw $v1, %d($t0)\n",getOffset(p->child->bro->varname));
          printf("li $v2, %d\n",getOffset(p->child->varname));
	}
        printf("div $v0, $v1, $v2\n");
	break;*/
  case IDENT_AST://ex)a = 0;
        //printf("skip IDENT_AST\n");
        if(p->varname == NULL) break;
        printf("\tlw $v0, %d($t0)\n",getOffset(p->varname));
        printf("\tnop\n");
 	break;
  case NUM_AST:
 	printf("\tori $v0, $zero, %d\n",p->value);
        break; 
  }

}

int getOffset(char *varname){
 int i;
 
 for(i=0;i<tp;i++){
   if(strcmp(varname,table[i].varname)==0){
     return table[i].addr - 10004000;
   }
 }
 return -1;//error: not found
}


Node *buildNode2(NType t,Node *p1,Node *p2){
 Node *p;
 if((p=(Node *)malloc(sizeof(Node))) == NULL){
   printf("error: out of memory\n");
 }
 p->ntype = t;
 p = addChildNode(p,p1);
 p1 = addBroNode(p1,p2);
 p->bro = NULL; 

 if(t == DEF_AST){
   addTable(p1); 
 }

 return p;
}

Node *buildNumNode(NType t,int n){
 Node *p;
 if((p=(Node *)malloc(sizeof(Node))) == NULL){
   printf("error: out of memory\n");
 }
 p->ntype = t;
 p->value = n;
 p->child = NULL;
 p->bro = NULL;

 return p;
} 

Node *buildIdentNode(NType t,char *s){
 Node *p;
 if((p=(Node *)malloc(sizeof(Node))) == NULL){
   printf("error: out of memory\n");
 }
 p->ntype = t;
 if((p->varname = (char*)malloc(sizeof(MAXBUF))) == NULL){
   printf("error: out of memory\n");
 }
 strncpy(p->varname,s,MAXBUF);
 p->child = NULL;
 p->bro = NULL;

 return p;
}

Node *buildArrayNode(NType t,char *s,int n){
 Node *p;
 char *name;
 if((p=(Node *)malloc(sizeof(Node))) == NULL){
   printf("error: out of memory\n");
 }
 p->ntype = t;
 if((p->varname = (char*)malloc(sizeof(MAXBUF))) == NULL){
   printf("error: out of memory\n");
 }
 snprintf(name,sizeof(MAXBUF),"%s[%d]",s,n);
 strncpy(p->varname,name,MAXBUF);
 p->child = NULL;
 p->bro = NULL;

 return p;
}

Node *buildDefNode(NType t,char *s1,char *s2){
/*
 Node *p,*q;
 p = newNode(t,s1,-1);
 q = newNode(t,s2,-1);

 if((p->varname = (char*)malloc(sizeof(MAXBUF))) == NULL){
   printf("error: out of memory\n");
 }
 addChildNode(p,q);
 
*/

// return p;
}


Node *newNode(NType ntype,char* varname,int value){
 Node *node;
 node = (Node *)malloc(sizeof(Node));
 node->child = NULL;
 node->bro = NULL;
 //node->varname = varname;
 //node->value = value;
 node->ntype = ntype;

 return node;
}

Node *addChildNode(Node *parent,Node *child){
 if (parent == NULL){
   fprintf(stderr,"node is not created\n");
   return NULL;
 }
 parent->child = (Node*)malloc(sizeof(Node));
 parent->child = child;

 return parent;
}

Node *addBroNode(Node *child, Node *bro){
 if (child == NULL){
   fprintf(stderr,"node is not created\n");
   return NULL;
 }
 child->bro = (Node*)malloc(sizeof(Node));
 child->bro = bro;

 return child;
}
/*
void printNodes(Node *node){
 char x_ast[19][20] = {"IDENT_AST","NUM_AST","ASSIGN_AST","ADD_AST","SUB_AST","MUL_AST",
 "DIV_AST","EQ_AST","LT_AST","GT_AST","WHILE_AST","IF_AST","ELSE_AST","STATS_AST",
 "PROG_AST","ARRAY_AST","DEF_AST","LOOP_AST","COND_AST"};
 
 if(node != NULL){
   if(node->child != NULL){
     printNodes(node->child);
   }
   if(node->ntype == NUM_AST) printf("(%d) ",node->value);
   else printf("(%s) ",node->varname);
   printf("->%s ",x_ast[node->ntype]);
   if(node->bro != NULL){
     printf("|\n");
     printNodes(node->bro);
   }
 }
}*/

