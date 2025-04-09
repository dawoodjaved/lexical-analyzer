%{
#include "y.tab.h"
%}

%%
[0-9]+                { yylval = atoi(yytext); return NUMBER; }
[\t \r]+             ;
"("                  { return '('; }
")"                  { return ')'; }
"+"                  { return '+'; }
"-"                  { return '-'; }
"*"                  { return '*'; }
"/"                  { return '/'; }
\n                   { /* handle end of input gracefully */ return 0; }
.                    { /* ignore other chars */ }

%%
