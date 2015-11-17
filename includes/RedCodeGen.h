/* 
* @Author: sxf
* @Date:   2015-09-23 22:55:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-17 11:19:15
* 
* 代码生成部分的引导类
*/

#ifndef RED_CODE_GEN_H
#define RED_CODE_GEN_H

#include "Model/Node.h"
#include "CodeGen.h"

namespace llvm {
	class Module;
	class Function;
} // llvm

class CodeGenContext;
class RedCodeGen : public CodeGen
{
public:
	virtual void Init(Node* node);
	virtual void PreScan(Node* node);
	virtual void Make(Node* node, const char* outfile_name, const char* module_name);
	virtual void MakeMeta(const char* outfile_name, const char* module_name);

	void VerifyAndWrite(llvm::Module* M, const char* outfile_name);

	static RedCodeGen* Create();
	static RedCodeGen* Create(Node* node);
private:
	RedCodeGen();
	RedCodeGen(Node* node);
	~RedCodeGen();
	CodeGenContext* context = NULL;


	void register_printf(llvm::Module *module);
	void register_malloc(llvm::Module *module);
	void register_echo(CodeGenContext* context, llvm::Function* printfFn);

	// 处理Meta数据相关函数
	void register_elite_meta_function(llvm::Module *module);
	void register_elite_meta_struct(llvm::Module *module);
	void register_elite_meta_init(llvm::Module *module);
	void register_functioncall(llvm::Module *module);
};



#endif // RED_CODE_GEN_H
