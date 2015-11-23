%{
#include <string>
#include "Model/nodes.h"
#include <list>
using namespace std;

#include "redapple_parser.hpp"
#include "StringEscape.h"

#define SAVE_TOKEN     yylval.str = maketoken(yytext, yyleng)
#define SAVE_STRING    yylval.str = makestring(yytext, yyleng, 2)
#define SAVE_STRING_NC yylval.str = makestring(yytext, yyleng, 3)
extern "C" int yywrap() { return 1; }
char* maketoken(const char* data, int len);
char* makestring(const char* data, int len, int s);

%}

%option yylineno

%%

"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" ; /* 就是这种注释 */ 

#[^\n]*\n               ; /* 井号注释 */ 
"//"[^\n]*\n            ; /* 双线注释 */ 

[ \t\v\n\f]             ; /* 过滤空白字符 */

    /* 一些双元运算符 */
"++"                    return PP;
"--"                    return SS;
"<-"                    return LF;
"->"                    return RF;
"&&"                    return AND;
"||"                    return OR;
"!"                     return '!';
"::"                    return NSP;


"+="                    return PE;
"-="                    return SE;
"*="                    return ME;
"/="                    return DE;
"&="                    return AE;
"|="                    return OE;
"^="                    return XE;
"%="                    return MODE;
"<<="                   return FLE;
">>="                   return FRE;


    /* 比较运算符 */
"=="                    return CEQ;
"<="                    return CLE;
">="                    return CGE;
"!="                    return CNE;
"<"                     return '<';
">"                     return '>';


    /* 界符 */
"("                     return '(';
")"                     return ')';
"["                     return '[';
"]"                     return ']';
"{"                     return '{';
"}"                     return '}';
"."                     return '.';
","                     return ',';
"?"                     return '?';
":"                     return ':';
";"                     return ';';
"[]"                    return SZ;


    /* 基本运算符 */
"="                     return '=';
"+"                     return '+';
"-"                     return '-';
"*"                     return '*';
"/"                     return '/';
"%"                     return '%';
"^"                     return '^';
"&"                     return '&';
"|"                     return '|';
"~"                     return '~';

    /* 宏运算符 */
"@"                     return '@';
",@"                    return MBK;
"`"                     return '`';
    /* 下面声明要用到的关键字 */

    /* 控制流 */
"if"                    return IF;
"else"                  return ELSE;
"while"                 return WHILE;
"do"                    return DO;
"until"                 return UNTIL;
"goto"                  return GOTO;
"for"                   return FOR;
"foreach"               return FOREACH;

    /* 退出控制 */
"break"|"continue"|"exit"   SAVE_TOKEN; return KWS_EXIT;

"return"                return RETURN;

    /* 特殊运算符 */
"new"                   return NEW;
"this"                  return THIS;
    
    /* 特殊定义 */
"delegate"              return DELEGATE;
"def"                   return DEF;
"define"                return DEFINE;
"defmacro"              return DEFMACRO;
"import"                return IMPORT;
"using"                 return USING;
"namespace"             return NAMESPACE;
"const"                 return CONST;
"packed"                return PACKED;
"volatile"              return VOLATILE;
"woven"                 return WOVEN;

"try"|"catch"|"finally"|"throw"  SAVE_TOKEN; return KWS_ERROR; /* 异常控制 */

"null"|"true"|"false"               SAVE_TOKEN; return KWS_TSZ; /* 特殊值 */

"struct"|"enum"|"union"|"module"|"interface"|"class"     SAVE_TOKEN; return KWS_STRUCT; /* 结构声明 */

"public"|"private"|"protected"  SAVE_TOKEN; return KWS_FWKZ; /* 访问控制 */

"static"|"extern"|"virtual"|"abstract"|"in"|"out"        SAVE_TOKEN; return KWS_FUNC_XS; /* 函数修饰符 */

"var"|"auto"            SAVE_TOKEN; return KWS_TYPE; /* 基本类型 */

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return ID; /* 标识符 */

-?[0-9]*\.[0-9]*        SAVE_TOKEN; return DOUBLE;
-?[0-9]+                SAVE_TOKEN; return INTEGER;
0x[0-9A-Fa-f]+          SAVE_TOKEN; return INTEGER;

\"(\\.|[^\\"])*\"       SAVE_STRING; return STRING; /* 字符串 */
@\"(\\.|[^\\"])*\"      SAVE_STRING_NC; return STRING; /* 无转义字符串 */
\'(\\.|.)\'             SAVE_STRING; return CHAR;   /* 字符 */

.                       printf("Unknown Token!\n"); yyterminate();

%%


char* maketoken(const char* data, int len) {
    char* str = new char[len+1];
    strncpy(str, data, len);
    str[len] = 0;
    return str;
}

char* makestring(const char* data, int len, int s) {
    char* str = new char[len-s+1];
    strncpy(str, data+s-1, len-s);
    str[len-s] = 0;
    if (s == 3) return str;
    char* ans = CharEscape(str);
    delete[] str;
    return ans; 
}