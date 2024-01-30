#define MAXBUF 30
typedef enum{
 IDENT_AST,//変数
 NUM_AST,
 ASSIGN_AST,
 ADD_AST,
 SUB_AST,
 MUL_AST,
 DIV_AST,
 EQ_AST,
 LT_AST,
 GT_AST,
 WHILE_AST,
 IF_AST,
 ELSE_AST,
 STATS_AST,
 PROG_AST,
 ARRAY_AST,
 DEF_AST,
 LOOP_AST,
 COND_AST
}NType;


typedef struct node{
 NType ntype;
 char* varname;
 int value;//NUMBER,配列の添字用
 struct node *child;
 struct node *bro;
} Node;

typedef struct SymbolTable{
 char* varname;
 int size;//配列ならその大きさ、以外は1
 int addr;
} Table;

int getOffset(char*);
void codeGene_for_expression(Node*);
void codeGeneration(Node*);
void codeGene_for_cond(Node*);
void codeGene_for_while(Node*);
void codeInit();
void codeEnd();
void addTable(Node*);
void makeTable(Node*);
void initTable(Table*);

Node *buildNode2(NType t,Node *p1,Node *p2);
Node *buildNumNode(NType t,int n);
Node *buildIdentNode(NType t,char *s);
Node *buildArrayNode(NType t,char *s,int n);
Node *buildDefNode(NType t,char *s1,char *s2);


Node *newNode(NType ntype,char* varname,int value);
Node *addChildNode(Node *parent,Node *child);
Node *addBroNode(Node *child, Node *bro);
void printNodes(Node *node);

