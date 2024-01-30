%{
#include <string.h>
#include "ast.h"
#include "compiler.tab.h"
%}
extern yylval;

%%
"define"	return DEFINE;
"array"		return ARRAY;
"while" 	return WHILE;
"if"		return IF;
"else"  	return ELSE;
";" 		return SEMIC;
"["		return L_BRACKET;
"]"		return R_BRACKET;
"("		return L_PARAN;
")"		return R_PARAN;
"{"		return L_BRACE;
"}"		return R_BRACE;
"=" 		return ASSIGN;
"+"		return ADD;
"-"		return SUB;
"*"		return MUL;
"/"		return DIV;
"=="		return EQ;
"<"		return LT;
">"		return GT;
[a-zA-Z]([a-zA-Z]|[0-9])*	{yylval.sp = strdup(yytext) ;return IDENT;}
[0-9]+ {yylval.ival = atoi(yytext) ;return NUMBER;}
[ \t\n]	        ;
"$" {return 0;}
%%
