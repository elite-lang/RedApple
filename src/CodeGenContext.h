/* 
* @Author: sxf
* @Date:   2015-10-10 18:44:44
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-11 14:22:56
* 
* 代码生成的上下文类, 是C实现宏的最核心功能类
*/

#ifndef CODE_GEN_CONTENT_H
#define CODE_GEN_CONTENT_H

#include "Node.h"
#include <map>
#include <string>
using namespace std;

#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

class CodeGenContext;
typedef Value* (*CodeGenFunction)(CodeGenContext*, Node*);
typedef struct _funcReg
{
	const char*     name;
	CodeGenFunction func;
} FuncReg;

class CodeGenContext
{
public:
	CodeGenContext(Node* node);
	~CodeGenContext();

	Value* MakeBegin() { return MacroMake(root); }

	// 这个函数是用来一条条翻译Node宏的
	Value* MacroMake(Node* node);

	CodeGenFunction getMacro(string& str);

	// C++注册宏
	// void AddMacros(const FuncReg* macro_funcs); // 为只添加不替换保留
	void AddOrReplaceMacros(const FuncReg* macro_funcs);

	void setModule(Module* pM) { M = pM; }
	Module* getModule() { return M; }
	void setContext(LLVMContext* pC) { Context = pC; }
	LLVMContext* getContext() { return Context; }
private:
	// 语法树根节点
	Node* root;

	// 当前的LLVM Module
	Module* M;
	LLVMContext* Context;

	// 这是用来查找是否有该宏定义的
	map<string, CodeGenFunction> macro_map;
};


#endif // CODE_GEN_CONTENT_H
