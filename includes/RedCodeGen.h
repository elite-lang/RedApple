/*
* @Author: sxf
* @Date:   2015-09-23 22:55:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-26 14:28:25
*
* 代码生成部分的引导类
*/

#ifndef RED_CODE_GEN_H
#define RED_CODE_GEN_H

#include "Model/Node.h"
#include "CodeGen.h"
#include "PassManager.h"


class CodeGenContext;
class RedCodeGen : public CodeGen
{
public:
	virtual void Init();
	virtual void PreScan(Node* node);
	virtual void PreScan(std::set<Node*>& nodes);
	virtual void Make(Node* node, const char* outfile_name, const char* module_name);
	virtual void MakeMeta(const char* outfile_name, const char* module_name);

	static RedCodeGen* Create();

	CodeGenContext* getContext() { return context; }
private:
	RedCodeGen();
	~RedCodeGen();
	CodeGenContext* context = NULL;
	PassManager pm;
};



#endif // RED_CODE_GEN_H
