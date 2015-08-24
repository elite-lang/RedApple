%{
#include "Model/nodes.h"
#include "idtable.h"

using namespace std;

ASTNode *programBlock; /* the top level root node of our final AST */

IDTable *st;

extern int yylex();

void yyerror(const char *s) { printf("ERROR: %s\n", s); }

%}

 

/* Represents the many different ways we can access our data */

%union {
    ASTNode *node;
    ExpressionNode *exp_node;

    StatementNode *statement_node;
    list<StatementNode*> *statement_node_list;

    ModuleDefNode *module_def_node;
    list<ModuleDefNode*> *module_def_node_list;

    IDNode *id_node;
    ValueNode *value_node;
    BlockNode *block_node;
    FuncCallNode *funccall_node;
    
    CallArgsNode *callargs_node;
    list<CallArgsNode*> *callargs_node_list;

    IntNode *int_node;
    FloatNode *float_node;
    ByteNode *byte_node;
    FuncDefNode *funcdef_node;
    //FuncArgsNode *funcargs_node;
    VarDefNode *vardef_node;
    list<VarDefNode*> *vardef_node_list;
    //ConstDefNode *constdef_node;
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
%token <str> KWS_EXIT RETURN NEW THIS KWS_ERROR KWS_TSZ KWS_STRUCT KWS_FWKZ KWS_FUNC_XS KWS_TYPE

/* 
   Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */

%type <node> program
%type <module_def_node> def_module_statement
%type <module_def_node_list> def_module_statements
%type <statement_node> def_statement
%type <block_node> def_statements
%type <statement_node> statement
%type <block_node> statements
%type <block_node> block
%type <vardef_node> var_def
%type <funcdef_node> func_def
%type <vardef_node_list> func_def_args
// %type <id_node> id
%type <value_node> numeric
%type <exp_node> expr
%type <callargs_node> call_arg 
%type <callargs_node_list> call_args
//%type <token> operator 这个设计容易引起规约冲突，舍弃
/* Operator precedence for mathematical operators */

%left '~'
%left '&' '|'
%left CEQ CNE CLE CGE '<' '>' '='
%left '+' '-'
%left '*' '/' '%' '^'

%start program

%%

program : def_module_statements { programBlock = $1; }
        ;

def_module_statement : KWS_STRUCT ID '{' def_statements '}' { $$ = new ModuleDefNode($1, $2, $4); } ;

def_module_statements  : def_module_statement { $$ = new list<ModuleDefNode*>(); 
                                                $$->push_back($1); }
                       | def_module_statements def_module_statement { $$ = $1; $$->push_back($2); }
                       ;

def_statement : var_def ';' { $$ = $1; } | func_def ;

def_statements : def_statement { $$ = new list<StatementNode*>(); 
                                 $$->push_back($1); }
               | def_statements def_statement { $$ = $1; $$->push_back($2); }
               ;

statements : statement { $$ = new list<StatementNode*>(); 
                         $$->push_back($1);  }
           | statements statement { $$ = $1; $$->push_back($2); }
           ;

statement : def_statement | expr ';' { $$ = $1; } | block ;

block : '{' statements '}' { $$ = new BlockNode($2); }
      | '{' '}' { $$ = new BlockNode(); }
      ; 

var_def : KWS_TYPE ID { $$ = new VarDefNode($1, $2); }
        | ID ID { $$ = new VarDefNode($1, $2); }
        | KWS_TYPE ID '=' expr { $$ = new VarDefNode($1, $2, $4); }
        | ID ID '=' expr { $$ = new VarDefNode($1, $2, $4); }
        ;

func_def : ID ID '(' func_def_args ')' block
            { $$ = new FuncDefNode($1, $2, $4, $6); }
         | KWS_TYPE ID '(' func_def_args ')' block
            { $$ = new FuncDefNode($1, $2, $4, $6); }
         ;

func_def_args : %empty  { $$ = NULL; }
              | var_def { $$ = new list<VarDefNode*>(); $$->push_back($1); }
              | func_def_args ',' var_def { $$ = $1; $$->push_back($3); }
              ;

numeric : INTEGER { $$ = new IntNode($1->str); }
        | DOUBLE { $$ = new FloatNode($1->str); }
        ;

expr : ID '=' expr { $$ = new FuncCallNode($2, $1, $3); }
     | ID '(' call_args ')' { $$ = new FuncCallNode($1, $3); }
     | ID { $$ = $1; }
     | numeric { $$ = $1; }
     | expr CEQ expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr CNE expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr CLE expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr CGE expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '<' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '>' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '+' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '-' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '*' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '/' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '%' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '^' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '&' expr { $$ = new FuncCallNode($2, $1, $3); }
     | expr '|' expr { $$ = new FuncCallNode($2, $1, $3); }
     | '~' expr { $$ = new FuncCallNode($2, $1); }
     | '(' expr ')'  /* ( expr ) */  { $$ = $2; }
     ;


call_arg  :  expr { $$ = new CallArgsNode($1);  }
          |  ID ':' expr { $$ = new CallArgsNode($1, $3); }
          ;

call_args : %empty { $$ = NULL; }
          | call_arg { $$ = new list<CallArgsNode*>(); $$->push_back($1); }
          | call_args ',' call_arg  { $$ = $1; $$->push_back($3); }
          ;

%%
