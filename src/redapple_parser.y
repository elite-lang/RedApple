%{
#include "Model/nodes.h"
#include <list>
using namespace std;

#define YYERROR_VERBOSE 1

Node *programBlock; /* the top level root node of our final AST */

extern int yylex();
extern int yylineno;
extern char* yytext;
extern int yyleng;

void yyerror(const char *s);

%}

 

/* Represents the many different ways we can access our data */

%union {
    Node *nodes;
    char *str;
    int token;
}

 

/* Define our terminal symbols (tokens). This should

   match our tokens.l lex file. We also define the node type

   they represent.

 */

%token <str> ID INTEGER DOUBLE
%token <token> CEQ CNE CGE CLE
%token <token> '<' '>' '=' '+' '-' '*' '/' '%' '^' '&' '|' '~'
%token <str> STRING CHAR
%token <token> IF ELSE WHILE DO GOTO FOR FOREACH  
%token <token> DELEGATE DEF DEFINE IMPORT USING NAMESPACE
%token <token> RETURN NEW THIS 
%token <str> KWS_EXIT KWS_ERROR KWS_TSZ KWS_STRUCT KWS_FWKZ KWS_FUNC_XS KWS_TYPE

/* 
   Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */

%type <nodes> program
%type <nodes> def_module_statement
%type <nodes> def_module_statements
%type <nodes> def_statement
%type <nodes> def_statements
%type <nodes> for_state
%type <nodes> if_state
%type <nodes> while_state
%type <nodes> statement
%type <nodes> statements
%type <nodes> block
%type <nodes> var_def
%type <nodes> func_def
%type <nodes> func_def_args
%type <nodes> func_def_xs 
%type <nodes> numeric
%type <nodes> expr
%type <nodes> call_arg 
%type <nodes> call_args 
%type <nodes> return_state

//%type <token> operator 这个设计容易引起规约冲突，舍弃
/* Operator precedence for mathematical operators */


%left '~'
%left '&' '|'
%left CEQ CNE CLE CGE '<' '>' '='
%left '+' '-'
%left '*' '/' '%' '^'
%left '.'

%start program

%%

program : def_statements { programBlock = Node::getList($1); }
        ;

def_module_statement : KWS_STRUCT ID '{' def_statements '}' { $$ = Node::make_list(3, new StringNode($1), new StringNode($2), $4); }
                     | KWS_STRUCT ID ';' { $$ = Node::make_list(3, new StringNode($1), new StringNode($2), new Node()); }
                     ;

def_module_statements  : def_module_statement { $$ = Node::getList($1); }
                       | def_module_statements def_module_statement { $$ = $1; $$->addBrother(Node::getList($2)); }
                       ;

func_def_xs : KWS_FUNC_XS { $$ = new StringNode($1); }
            | func_def_xs KWS_FUNC_XS {$$ = $1; $$->addBrother(new StringNode($2)); }
            ;

def_statement : var_def ';' { $$ = $1; }
              | func_def 
              | def_module_statement 
              | func_def_xs func_def { $$ = $2; $2->addBrother(Node::getList($1)); } 
              ;

def_statements : def_statement { $$ = Node::getList($1); }
               | def_statements def_statement { $$ = $1; $$->addBrother(Node::getList($2)); }
               ;

statements : statement { $$ = Node::getList($1); }
           | statements statement { $$ = $1; $$->addBrother(Node::getList($2)); }
           ;

statement : def_statement 
          | expr ';' { $$ = $1; } 
          | block 
          | if_state
          | while_state
          | for_state
          | return_state
          ;

if_state : IF '(' expr ')' statement { $$ = Node::make_list(3, new StringNode("if"), $3, $5); }
         | IF '(' expr ')' statement ELSE statement { $$ = Node::make_list(4, new StringNode("if"), $3, $5, $7); }
         ;

while_state : WHILE '(' expr ')' statement { $$ = Node::make_list(3, new StringNode("while"), $3, $5); }
            ;

for_state : FOR '(' expr ';' expr ';' expr ')' statement { $$ = Node::make_list(5, new StringNode("for"), $3, $5, $7, $9); }
          | FOR '(' var_def ';' expr ';' expr ')' statement { $$ = Node::make_list(5, new StringNode("for"), new Node($3), $5, $7, $9); }
          ;

return_state : RETURN ';' { $$ = new StringNode("return"); }
             | RETURN expr ';' { $$ = new StringNode("return"); $$->addBrother($2); }              

block : '{' statements '}' { $$ = new Node($2); }
      | '{' '}' { $$ = new Node(); }
      ; 

var_def : KWS_TYPE ID { $$ = Node::make_list(3, new StringNode("set"), new StringNode($1), new StringNode($2)); }
        | ID ID { $$ = Node::make_list(3, new StringNode("set"), new StringNode($1), new StringNode($2)); }
        | KWS_TYPE ID '=' expr { $$ = Node::make_list(4, new StringNode("set"), new StringNode($1), new StringNode($2), $4); }
        | ID ID '=' expr { $$ = Node::make_list(4, new StringNode("set"), new StringNode($1), new StringNode($2), $4); }
        ;

func_def : ID ID '(' func_def_args ')' block
            { $$ = Node::make_list(5, new StringNode("function"), new StringNode($1), new StringNode($2), $4, $6); }
         | KWS_TYPE ID '(' func_def_args ')' block
            { $$ = Node::make_list(5, new StringNode("function"), new StringNode($1), new StringNode($2), $4, $6); }
         | ID ID '(' func_def_args ')' ';'
            { $$ = Node::make_list(5, new StringNode("function"), new StringNode($1), new StringNode($2), $4); }
         | KWS_TYPE ID '(' func_def_args ')' ';'
            { $$ = Node::make_list(5, new StringNode("function"), new StringNode($1), new StringNode($2), $4); }
         ;

func_def_args : var_def { $$ = new Node(new Node($1)); }
              | func_def_args ',' var_def { $$ = $1; $$->addChildren(new Node($3)); }
              | %empty  { $$ = new Node(); }
              ;

numeric : INTEGER { $$ = new IntNode($1); }
        | DOUBLE { $$ = new FloatNode($1); }
        ;

expr : expr '=' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("="), $1, $3); }
     | ID '(' call_args ')' { $$ = Node::make_list(2, new StringNode("call"), new StringNode($1)); $$->addBrother($3); }
     | ID { $$ = new IDNode($1); }
     | numeric { $$ = $1; }
     | STRING { $$ = new StringNode($1); }
     | CHAR   { $$ = new CharNode($1); }
     | KWS_TSZ 
     | NEW ID '(' call_args ')' { $$ = Node::make_list(3, new StringNode("new"), new StringNode($2), $4); }
     | expr CEQ expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("=="), $1, $3); }
     | expr CNE expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("!="), $1, $3); }
     | expr CLE expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("<="), $1, $3); }
     | expr CGE expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode(">="), $1, $3); }
     | expr '<' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("<"), $1, $3); }
     | expr '>' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode(">"), $1, $3); }
     | expr '+' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("+"), $1, $3); }
     | expr '-' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("-"), $1, $3); }
     | expr '*' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("*"), $1, $3); }
     | expr '/' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("/"), $1, $3); }
     | expr '%' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("%"), $1, $3); }
     | expr '^' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("^"), $1, $3); }
     | expr '&' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("&"), $1, $3); }
     | expr '|' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("|"), $1, $3); }
     | expr '.' expr { $$ = Node::make_list(4, new StringNode("opt2"), new StringNode("."), $1, $3); }
     | '~' expr { $$ = Node::make_list(4, new StringNode("opt1"), new StringNode("~"), $2); }
     | '(' expr ')'  /* ( expr ) */  { $$ = $2; }
     ;


call_arg  :  expr { $$ = $1;  }
          |  ID ':' expr { $$ = Node::make_list(3, new StringNode(":"), $1, $3); }
          ;

call_args : %empty { $$ = new Node(); }
          | call_arg { $$ = Node::getList($1); }
          | call_args ',' call_arg  { $$ = $1; $$->addBrother(Node::getList($3)); }
          ;

%%

void yyerror(const char* s){
    fprintf(stderr, "%s \n", s);    
    fprintf(stderr, "line %d: ", yylineno);
    fprintf(stderr, "text %s \n", yytext);
    exit(1);
}