%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

DIGIT [A-Z0-9]

%%

"#".*                 {}
[A-Z][A-Z0-9]*        { yylval.name = strdup(yytext); return NAME; }
"set" 				  { return KW_SET; }
"+"					  { return ADD; }
"-"					  { return SUB; return OPPOSE; }
"*"					  { return MUL; }
"/"				  	  { return DIV; }
"^"			  		  { return PUI; }
"sin"                   { return SIN; }
"cos" 				  { return COS; }
"tan" 				  { return TAN; }
"sqrt" 				  { return SQRT; }
"random" 				  { return RANDOM; }
-?{DIGIT}+            { yylval.value = strtod(yytext, NULL); return VALUE; }
-?[0-9]*[[0-9]+e[0-9]*[[\.[0-9]+e[0-9]*]?|[\.[0-9]+]?]|[[\.[0-9]+e[0-9]*]?|[\.[0-9]+]?] { yylval.value = atof(yytext); return VALUE; }
forward|fw            { return KW_FORWARD; }
backward|bw			  { return KW_BACKWARD; }
position|pos          { return KW_POSITION; }
right|rt              { return KW_RIGHT; }
left|lt              { return KW_LEFT; }
heading|hd           { return KW_HEADING; }
"up"					  { return KW_UP; }
"down"				  { return KW_DOWN; }
"print"				  { return KW_PRINT; }
"color"                 { return KW_COLOR; }
"red"					  { return RED; }
"green"                 { return GREEN; }
"blue"				  { return BLUE; }
"black"                { return BLACK; }
"gray"				  { return GRAY; }
"cyan"				  { return CYAN; }
"yellow"                { return YELLOW; }
"magenta"				  { return MAGENTA; }
"{"			  		  { return LP; }
"}"					  { return RP; }
")"              	  { return LL; }
"("              	  { return RR; }
","					  { return COMMA; }

repeat                { return KW_REPEAT; }
[\n\t ]*              /* whitespace */
quit { return QUIT; }
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
