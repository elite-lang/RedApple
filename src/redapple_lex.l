%{
#include <string>
#include "Model/nodes.h"
#include "redapple_parser.hpp"

#define SAVE_TOKEN yylval.str = new std::string(yytext, yyleng)

#define TOKEN(t) (yylval.token = t)

extern "C" int yywrap() { }

%}

 

%%

[ \t\n]                 ;

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;

[0-9]+\.[0-9]*          SAVE_TOKEN; return TDOUBLE;

[0-9]+                  SAVE_TOKEN; return TINTEGER;

"="                     SAVE_TOKEN; return TOKEN(TEQUAL);

"=="                    SAVE_TOKEN; return TOKEN(TCEQ);

"!="                    SAVE_TOKEN; return TOKEN(TCNE);

"<"                     SAVE_TOKEN; return TOKEN(TCLT);

"<="                    SAVE_TOKEN; return TOKEN(TCLE);

">"                     SAVE_TOKEN; return TOKEN(TCGT);

">="                    SAVE_TOKEN; return TOKEN(TCGE);

"("                     return TOKEN(TLPAREN);

")"                     return TOKEN(TRPAREN);

"{"                     return TOKEN(TLBRACE);

"}"                     return TOKEN(TRBRACE);

"."                     SAVE_TOKEN; return TOKEN(TDOT);

","                     return TOKEN(TCOMMA);

":"			return TOKEN(TCOLON);

"+"                     SAVE_TOKEN; return TOKEN(TPLUS);

"-"                     SAVE_TOKEN; return TOKEN(TMINUS);

"*"                     SAVE_TOKEN; return TOKEN(TMUL);

"/"                     SAVE_TOKEN; return TOKEN(TDIV);

"%"                     SAVE_TOKEN; return TOKEN(TMOD);

.                       printf("Unknown token!\n"); yyterminate();


%%

