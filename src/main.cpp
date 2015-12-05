#include <stdio.h>
#include <fstream>
#include "Model/nodes.h"
#include "redapple_parser.hpp"
#include "RedCodeGen.h"
#include <cstring>
#define maxpath 1000
using namespace std;

const char help_message[] = "welcome for using Red Apple compiler v0.6!\nusage: red [options] src-files\n";

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

Node* parseFile(const char* path) {
	FILE* file_in;
	if ((file_in = fopen(path, "r")) == NULL) {
		printf("找不到程序源文件: %s\n", path);
		return 0;
	}
	
	yyin = file_in;
	yyparse();
	fclose(file_in);

	// 打印语法树
	printf("源文件 - %s\n", path);
	programBlock->print(0);
	return programBlock;
}

int main(int argc,const char *argv[])
{
	if (argc <= 1) printf(help_message);
	else {
		const char *file_in_name = argv[1];
		Node* ans = parseFile(file_in_name);
		
		// 语法生成
		char* output_name = make_default_name(file_in_name);
		CodeGen* codegen = RedCodeGen::Create();
		codegen->Init();
		codegen->PreScan(ans);
		codegen->Make(ans, output_name);
		codegen->MakeMeta("meta.bc", "Meta");
		delete codegen;

		/* you should close the file. */
	}
	return 0;
}
