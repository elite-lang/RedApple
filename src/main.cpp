#include <stdio.h>
#include <fstream>
#include "Model/nodes.h"
#include "redapple_parser.hpp"

#define maxpath 1000
using namespace std;

const char help_message[] = "welcome for using Red Apple compiler v0.1!\nusage: red [options] src-files\n";

extern FILE* yyin;
extern Node *programBlock;

char* fileReader(const char* path, int& flen) {
	fstream file;
	locale::global(locale("zh_CN.UTF-8"));
	file.open(path);//打开文件
	if(!file.is_open())
	{
		printf("can not open BNF file!\n");
		return NULL;
	}
	file.seekg(0,ios::end);
	flen = file.tellg();
	file.seekg(0,ios::beg);
	char* data = new char[flen+1];
	file.read(data,flen);
	file.close();
	data[flen] = 0;
	return data;
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
			return NULL;
		}
		yyin = file_in;
		yyparse();
		
		// 打印语法树
		programBlock->print(0);

		/* you should close the file. */
		fclose(file_in);
	}
	return 0;
}
