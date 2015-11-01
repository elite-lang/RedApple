/* 
* @Author: sxf
* @Date:   2015-10-10 18:44:44
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-31 10:02:21
* 
* 代码生成的上下文类, 是C实现宏的最核心功能类
*/

#ifndef CODE_GEN_CONTENT_H
#define CODE_GEN_CONTENT_H

#include "Node.h"
#include <map>
#include <stack>
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
#include "llvm/IR/ValueSymbolTable.h"
#include "idtable.h"

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

	// 必要的初始化方法
	void PreInit();
	void PreTypeInit();
	void Init();

	void MakeBegin() { 
		MacroMake(root); 
	}

	// 这个函数是用来一条条翻译Node宏的
	Value* MacroMake(Node* node);
	// 递归翻译该节点下的所有宏
	void MacroMakeAll(Node* node); 
	CodeGenFunction getMacro(string& str);

	// C++注册宏
	// void AddMacros(const FuncReg* macro_funcs); // 为只添加不替换保留
	void AddOrReplaceMacros(const FuncReg* macro_funcs);

	// 代码块栈的相关操作
	BasicBlock* getNowBlock();
	BasicBlock* createBlock();
	BasicBlock* createBlock(Function* f);

	// 获取当前模块中已注册的函数
	Function* getFunction(Node* node);
	Function* getFunction(std::string& name);
	void nowFunction(Function* _nowFunc);

	// 以下两函数已废弃
	Type* getNormalType(Node*);
	Type* getNormalType(std::string& str);

	void setModule(Module* pM) { M = pM; }
	Module* getModule() { return M; }
	void setContext(LLVMContext* pC) { Context = pC; }
	LLVMContext* getContext() { return Context; }

	// 类型的定义和查找
	void DefType(string name, Type* t);
	Type* FindType(string& name);
	Type* FindType(Node*);

	void SaveMacros();
	void RecoverMacros();

	bool isSave() { return _save; }
	void setIsSave(bool save) { _save = save; }

	id* FindST(Node* node) const;

	id* FindST(string& str) const {
		return st->find(str);
	}
	IDTable* st;
private:
	// 语法树根节点
	Node* root;

	// 当前的LLVM Module
	Module* M;
	LLVMContext* Context;
	Function* nowFunc;
	BasicBlock* nowBlock;
	
	// 这是用来查找是否有该宏定义的
	map<string, CodeGenFunction> macro_map;

	// 这个栈是用来临时保存上面的查询表的
	stack<map<string, CodeGenFunction> > macro_save_stack;

	// 这个表是用来查询自定义类型的
	// map<string, Type*> type_map;

	void setNormalType();

	// 用来记录当前是读取还是存入状态
	bool _save;
};


#endif // CODE_GEN_CONTENT_H
