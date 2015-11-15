/* 
* @Author: sxf
* @Date:   2015-09-23 22:55:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-13 12:30:09
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
	virtual void PreScan();
	virtual void ScanOther(Node* node);
	virtual void Make(const char* outfile_name);

	static RedCodeGen* Create();
	static RedCodeGen* Create(Node* node);
private:
	RedCodeGen();
	RedCodeGen(Node* node);
	~RedCodeGen();
	CodeGenContext* context;
};



#endif // RED_CODE_GEN_H
