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
right|rt              { return KW_RIGHT; }
[\n\t ]*              /* whitespace */
quit { return QUIT; }
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
