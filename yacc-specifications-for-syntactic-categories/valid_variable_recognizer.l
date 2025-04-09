%{
#include "y.tab.h"
%}

%%
[a-zA-Z][a-zA-Z0-9]*    { return ID; }
.                       { /* ignore or return error token if desired */ }

%%
