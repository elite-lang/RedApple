/* 
* @Author: sxf
* @Date:   2015-09-23 22:55:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 12:26:13
* 
* 代码生成部分的引导类
*/

#ifndef RED_CODE_GEN_H
#define RED_CODE_GEN_H

#include "Model/Node.h"
#include "CodeGen.h"


class CodeGenContext;
class RedCodeGen : public CodeGen
{
public:
	virtual void Init(Node* node);
	virtual void PreScan(Node* node);
	virtual void Make(Node* node, const char* outfile_name, const char* module_name);
	virtual void MakeMeta(const char* outfile_name, const char* module_name);

	static RedCodeGen* Create();
	static RedCodeGen* Create(Node* node);
private:
	RedCodeGen();
	RedCodeGen(Node* node);
	~RedCodeGen();
	CodeGenContext* context = NULL;
};



#endif // RED_CODE_GEN_H
