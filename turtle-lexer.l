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

-?{DIGIT}+              { yylval.value = strtod(yytext, NULL); return VALUE; }
forward|fw            { return KW_FORWARD; }
backward|bw			  { return KW_BACKWARD; }
position|pos          { return KW_POSITION; }
right|rt              { return KW_RIGHT; }
left|lt              { return KW_LEFT; }
heading|hd           { return KW_HEADING; }
up					  { return KW_UP; }
down				  { return KW_DOWN; }
print				  { return KW_PRINT; }
red|green|blue|black|gray|cyan|yellow|magenta    { printf("I know this is color: %s\n", yytext); yylval.name = yytext; return NAME; }
color                 { return KW_COLOR; }
[\n\t ]*              /* whitespace */
quit { return QUIT; }
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
