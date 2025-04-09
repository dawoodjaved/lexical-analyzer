%{
#include "y.tab.h"
%}

%%
[0-9]+       { yylval = atoi(yytext); return NUMBER; }
[ \t\r]+     ;
"("          { return '('; }
")"          { return ')'; }
"+"          { return '+'; }
"-"          { return '-'; }
"*"          { return '*'; }
"/"          { return '/'; }
\n           { return 0; }
.            { /* ignore everything else */ }

%%
