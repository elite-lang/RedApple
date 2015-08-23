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

    IDNode *id_node;

    ValueNode *value_node;

    BlockNode *block_node;

    FuncCallNode *funccall_node;

    CallArgsNode *callargs_node;

    IntNode *int_node;

    FloatNode *float_node;

    ByteNode *byte_node;

    TypeDefNode *typedef_node;

    FuncDefNode *funcdef_node;

    FuncArgsNode *funcargs_node;

    VarDefNode *vardef_node;

    ConstDefNode *constdef_node;

    string *str;

    int token;
}

 

/* Define our terminal symbols (tokens). This should

   match our tokens.l lex file. We also define the node type

   they represent.

 */

%token <str> TIDENTIFIER TINTEGER TDOUBLE

%token <str> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL

%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT TCOLON

%token <str> TPLUS TMINUS TMUL TDIV TMOD

 

/* Define the type of node our nonterminal symbols represent.

   The types refer to the %union declaration above. Ex: when

   we call an ident (defined by union type ident) we are really

   calling an (NIdentifier*). It makes the compiler happy.

 */

%type <node> program

%type <statement_node> def_statement

%type <block_node> def_statements

%type <statement_node> statement

%type <block_node> statements

%type <block_node> block

%type <vardef_node> var_def

%type <funcdef_node> func_def

%type <funcargs_node> func_def_args

%type <id_node> id

%type <value_node> numeric

%type <exp_node> expr

%type <callargs_node> call_arg 

%type <callargs_node> call_args

%type <str> operator


/* Operator precedence for mathematical operators */

%left TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL

%left TPLUS TMINUS

%left TMUL TDIV TMOD


%start program

%%

program : def_statements { programBlock = $1; }

        ;

def_statement : var_def | func_def ;

def_statements : def_statement { $$ = new BlockNode($1); }

	       | def_statements def_statement { $1->addChildren($2); }

 	       ;

statements : statement { $$ = new BlockNode($1); }

	   | statements statement { $1->addChildren($2); $$ = $1; }

	   ;

statement : def_statement | expr | block ;

block : TLBRACE { st->push(); } statements TRBRACE { $$ = $3; st->pop(); }

      | TLBRACE TRBRACE { $$ = new BlockNode(); }

      ;

 

var_def : id id { $$ = new VarDefNode($1, $2); }

        | id id TEQUAL expr { $$ = new VarDefNode($1, $2, $4); }

        ;

 

func_def : id id TLPAREN { st->push(); } func_def_args TRPAREN block

            { $$ = new FuncDefNode($1, $2, $5, $7); st->pop(); }

          ;

func_def_args : %empty  { $$ = NULL; }

              | var_def { $$ = new FuncArgsNode($1); }

              | func_def_args TCOMMA var_def { $1->addChildren($3); }

              ;

 

id : TIDENTIFIER { $$ = new IDNode($1); }

   ;

 

numeric : TINTEGER { $$ = new IntNode(atol($1->c_str())); delete $1; }

        | TDOUBLE { $$ = new FloatNode(atof($1->c_str())); delete $1; }

        ;

 

expr : id TEQUAL expr { $$ = new FuncCallNode($2, $1, $3); }

     | id TLPAREN call_args TRPAREN { $$ = new FuncCallNode($1, $3); }

     | id { $$ = $1; }

     | numeric { $$ = $1; }

     | expr operator expr { $$ = new FuncCallNode($2, $1, $3); }

     | TLPAREN expr TRPAREN  /* ( expr ) */  { $$ = $2; }

     ;


call_arg  :  expr { $$ = new CallArgsNode($1);  }
	
	  | TIDENTIFIER TCOLON expr { $$ = new CallArgsNode($3, $1); }

	  ;


call_args : %empty { $$ = NULL; }

          | call_arg

          | call_args TCOMMA call_arg  { $1->addBrother($3); $$ = $1; }

          ;

 

operator : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE

         | TPLUS | TMINUS | TMUL | TDIV | TMOD

         ;

 

%%
