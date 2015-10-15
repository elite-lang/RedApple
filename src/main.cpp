#include <stdio.h>
#include <fstream>
#include "Model/nodes.h"
#include "redapple_parser.hpp"
#include "CodeGen.h"
#include <cstring>
#define maxpath 1000
using namespace std;

const char help_message[] = "welcome for using Red Apple compiler v0.1!\nusage: red [options] src-files\n";

extern FILE* yyin;
extern Node *programBlock;

char* make_default_name(const char* filename) {
	int size = 0;
	for (const char* p = filename; *p != 0; ++p, ++size)
		if (*p == '.') break;
	char* str = new char[size+4];
	strncpy(str, filename, size+1);
	str[size+1] = 'b';
	str[size+2] = 'c';
	str[size+3] = 0;
	return str;
}

int main(int argc,const char *argv[])
{
	if (argc <= 1) printf(help_message);
	else {
		const char *file_in_name = argv[1];
		FILE* file_in;
		if ((file_in = fopen(file_in_name, "r")) == NULL) {
			printf(file_in_name);
			printf("找不到程序源文件");
			return 0;
		}
		
		yyin = file_in;
		yyparse();

		// 打印语法树
		programBlock->print(0);

		// 语法生成
		char* output_name = make_default_name(file_in_name);
		CodeGen* codegen = new CodeGen(programBlock);
		codegen->Make(output_name);
		delete codegen;

		/* you should close the file. */
		fclose(file_in);
	}
	return 0;
}
