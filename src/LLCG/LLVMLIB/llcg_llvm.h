/*
* @Author: sxf
* @Date:   2015-11-23 21:37:15
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 17:16:39
*/


#ifndef LLCG_LLVM_H
#define LLCG_LLVM_H

#include "LLCG/llcg.h"
#include "LLCG/llcg_l.h"

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

using namespace llvm;

typedef void (*LibFunc)(Module*);

class llcg_llvm : public llcg, public llcg_l
{
public:
	llcg_llvm();
	~llcg_llvm();

	virtual LValue FuncType(FunctionModel* fmodel); // 返回FunctionType
	virtual LValue FuncType(LValue ret_type, vector<LValue>& types, bool isNotSure = false);
	virtual LValue GetOrInsertFunction(FunctionModel* fmodel); // 返回Function
	virtual LValue GetOrInsertFunction(const string& name, LValue func_type);
	virtual LValue GetOrInsertFunction(const string& name, LValue ret_type, vector<LValue>& types, bool isNotSure = false);
	virtual void   FunctionBodyBegin(LValue func, vector<string>& name_list); // 设置当前BasicBlock
	virtual void   FunctionBodyEnd(); // 处理函数结束
	virtual LValue getFunction(const string& name); // 从当前模块中获取一个函数
	virtual LValue Call(FunctionModel* fmodel, vector<LValue>& args); // 返回CallInst
	virtual LValue Call(LValue func, vector<LValue>& args);
	virtual LValue Struct(StructModel* smodel); // 返回StructType
	virtual LValue Struct(LValue _struct, vector<LValue>& types);
	virtual LValue DeclareStruct(const string& name);
	virtual LValue DefVar(LValue var_type, const string& name); // 返回分配的地址
	virtual LValue DefVar(LValue var_type, const string& name, LValue init);
	virtual LValue DefGlobalVar(LValue var_type, const string& name);
	virtual LValue DefGlobalVar(LValue var_type, const string& name, LValue init);
	virtual LValue Load(LValue var_addr);
	virtual LValue Store(LValue var_addr, LValue value);
	virtual LValue Opt1(const string& opt, LValue value);
	virtual LValue Opt2(const string& opt, LValue value1, LValue value2);
	virtual LValue Cmp(const string& opt, LValue value1, LValue value2);
	virtual LValue Assignment(const string& opt, LValue value1, LValue value2);
	virtual LValue Dot(LValue value, int num);
	virtual LValue Select(LValue value, vector<LValue>& args);
	virtual void   If(LValue cond, LValue father, LValue true_block, LValue true_block_end,
					   LValue false_block, LValue false_block_end, bool isElseWork);
	virtual void   For(LValue cond, LValue init, LValue pd, LValue work, LValue statement, LValue statement_end, LValue end);
	virtual void   While(LValue cond, LValue father, LValue pd, LValue statement, LValue statement_end, LValue end);
	virtual void   DoWhile(LValue statement, LValue pd);
	virtual void   DoUntil(LValue statement, LValue pd);
	virtual void   Goto(LValue target);
	virtual LValue New(LValue var_type, vector<LValue>& args, const string& funcname = "");
	virtual LValue NewArray(LValue var_type, vector<LValue>& wd, const string& funcname = "");
	virtual LValue Delete(LValue pointer, const string& funcname = "");
	virtual LValue DeleteArray(LValue pointer, const string& funcname = "");
	virtual LValue Return();
	virtual LValue Return(LValue var);

	virtual LValue Int8();
	virtual LValue Int16();
	virtual LValue Int32();
	virtual LValue Int64();
	virtual LValue Float();
	virtual LValue Double();
	virtual LValue Void();

	virtual LValue ConstString(const string& str);
	virtual LValue ConstInt(int num);
	virtual LValue ConstDouble(double num);

	virtual void   BeginModule(const string& name);
	virtual void   VerifyAndWrite(const string& outfile_name);
	virtual void   MakeMetaModule(const string& outfile_name, const string& module_name);

	virtual LValue GetNowBasicBlock();
	virtual LValue CreateBasicBlock();
	virtual LValue CreateBasicBlock(LValue func);

	virtual void MakeMetaList(vector<string>& list);
	virtual void MakeMetaList(const string& name, vector<string>& list, LValue fp);
	virtual void CloseTerminator(LValue basicblock, LValue target);
	virtual void SetNowBasicBlock(LValue nowBlock);

	/* 下面是llcg_l的继承 */

	virtual lvalue* l_FuncType(FunctionModel* fmodel); // 返回FunctionType
	virtual lvalue* l_FuncType(lvalue* ret_type, vector<lvalue*>& types, bool isNotSure = false);
	virtual lvalue* l_GetOrInsertFunction(FunctionModel* fmodel); // 返回Function
	virtual lvalue* l_GetOrInsertFunction(const string& name, lvalue* func_type);
	virtual lvalue* l_GetOrInsertFunction(const string& name, lvalue* ret_type, vector<lvalue*>& types, bool isNotSure = false);
	virtual void   l_FunctionBodyBegin(lvalue* func, vector<string>& name_list); // 设置当前BasicBlock
	virtual void   l_FunctionBodyEnd(); // 处理函数结束
	virtual lvalue* l_getFunction(const string& name); // 从当前模块中获取一个函数
	virtual lvalue* l_Call(FunctionModel* fmodel, vector<lvalue*>& args); // 返回CallInst
	virtual lvalue* l_Call(lvalue* func, vector<lvalue*>& args);
	virtual lvalue* l_Struct(StructModel* smodel); // 返回StructType
	virtual lvalue* l_Struct(lvalue* _struct, vector<lvalue*>& types);
	virtual lvalue* l_DeclareStruct(const string& name);
	virtual lvalue* l_DefVar(lvalue* var_type, const string& name); // 返回分配的地址
	virtual lvalue* l_DefVar(lvalue* var_type, const string& name, lvalue* init);
	virtual lvalue* l_DefGlobalVar(lvalue* var_type, const string& name);
	virtual lvalue* l_DefGlobalVar(lvalue* var_type, const string& name, lvalue* init);
	virtual lvalue* l_Load(lvalue* var_addr);
	virtual lvalue* l_Store(lvalue* var_addr, lvalue* value);
	virtual lvalue* l_Opt1(const string& opt, lvalue* value);
	virtual lvalue* l_Opt2(const string& opt, lvalue* value1, lvalue* value2);
	virtual lvalue* l_Cmp(const string& opt, lvalue* value1, lvalue* value2);
	virtual lvalue* l_Assignment(const string& opt, lvalue* value1, lvalue* value2);
	virtual lvalue* l_Dot(lvalue* value, int num);
	virtual lvalue* l_Select(lvalue* value, vector<lvalue*>& args);
	virtual void   l_If(lvalue* cond, lvalue* father, lvalue* true_block, lvalue* true_block_end,
					   lvalue* false_block, lvalue* false_block_end, bool isElseWork);
	virtual void   l_For(lvalue* cond, lvalue* init, lvalue* pd, lvalue* work, lvalue* statement, lvalue* statement_end, lvalue* end);
	virtual void   l_While(lvalue* cond, lvalue* father, lvalue* pd, lvalue* statement, lvalue* statement_end, lvalue* end);
	virtual void   l_DoWhile(lvalue* statement, lvalue* pd);
	virtual void   l_DoUntil(lvalue* statement, lvalue* pd);
	virtual lvalue* l_New(lvalue* var_type, vector<lvalue*>& args, const string& funcname = "");
	virtual lvalue* l_NewArray(lvalue* var_type, vector<lvalue*>& wd, const string& funcname = "");
	virtual lvalue* l_Delete(lvalue* pointer, const string& funcname = "");
	virtual lvalue* l_DeleteArray(lvalue* pointer, const string& funcname = "");
	virtual lvalue* l_Return();
	virtual lvalue* l_Return(lvalue* var);

	virtual lvalue* l_Int8();
	virtual lvalue* l_Int16();
	virtual lvalue* l_Int32();
	virtual lvalue* l_Int64();
	virtual lvalue* l_Float();
	virtual lvalue* l_Double();
	virtual lvalue* l_Void();

	virtual lvalue* l_ConstString(const string& str);
	virtual lvalue* l_ConstInt(int num);
	virtual lvalue* l_ConstDouble(double num);

	virtual void   l_BeginModule(const string& name);
	virtual void   l_VerifyAndWrite(const string& outfile_name);
	virtual void   l_MakeMetaModule(const string& outfile_name, const string& module_name);

	virtual lvalue* l_GetNowBasicBlock();
	virtual lvalue* l_CreateBasicBlock();
	virtual lvalue* l_CreateBasicBlock(lvalue* func);

	virtual void   l_MakeMetaList(vector<string>& list);
	virtual void   l_MakeMetaList(const string& name, vector<string>& list, lvalue* fp);
	virtual void   l_CloseTerminator(lvalue* basicblock, lvalue* target);

protected:

	// 当前的LLVM Module
	unique_ptr<Module> M;
	// 模块元数据 Module
	unique_ptr<Module> meta_M;

	LLVMContext context;
	Function* nowFunc;
	BasicBlock* nowBlock;

	void register_stdlib(Module* M, const LibFunc*);
	void register_metalib();
	void verifyModuleAndWrite(llvm::Module* M, const string& outfile_name);
	static Constant* geti8StrVal(Module& M, char const* str, Twine const& name);
	static Constant* getPtrArray(Module& M, vector<Constant*>& args_list);
	GetElementPtrInst* ptrMove(Value* v, int n);
	BasicBlock* createBlock();
	BasicBlock* createBlock(Function* f);

	FunctionType* getMallocType();
	FunctionType* getMallocArrayType();
	static FunctionType* malloc_type;
	static FunctionType* malloc_array_type;

};




#endif // LLCG_LLVM_H
