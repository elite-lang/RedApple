/*
* @Author: sxf
* @Date:   2015-11-23 10:53:22
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 17:15:54
*/

#ifndef LLCG_H
#define LLCG_H

#include "LLCG/lvalue.h"

#include "MetaModel/FunctionModel.h"
#include "MetaModel/StructModel.h"

#include <string>
#include <vector>

using namespace std;


/**
 * @brief 核心接口类 底层代码生成器 Low-Level Code Generator
 * 整个底层的聚合接口
 */
class llcg
{
public:
	virtual LValue FuncType(FunctionModel* fmodel) = 0; // 返回FunctionType
	virtual LValue FuncType(LValue ret_type, vector<LValue>& types, bool isNotSure = false) = 0;
	virtual LValue GetOrInsertFunction(FunctionModel* fmodel) = 0; // 返回Function
	virtual LValue GetOrInsertFunction(const string& name, LValue func_type) = 0;
	virtual LValue GetOrInsertFunction(const string& name, LValue ret_type, vector<LValue>& types, bool isNotSure = false) = 0;
	virtual void   FunctionBodyBegin(LValue func, vector<string>& name_list) = 0; // 设置当前BasicBlock
	virtual void   FunctionBodyEnd() = 0; // 处理函数结束
	virtual LValue getFunction(const string& name) = 0; // 从当前模块中获取一个函数
	virtual LValue Call(FunctionModel* fmodel, vector<LValue>& args) = 0; // 返回CallInst
	virtual LValue Call(LValue func, vector<LValue>& args) = 0;
	virtual LValue Struct(StructModel* smodel) = 0; // 返回StructType
	virtual LValue Struct(LValue _struct, vector<LValue>& types) = 0;
	virtual LValue DeclareStruct(const string& name) = 0;
	virtual LValue DefVar(LValue var_type, const string& name) = 0; // 返回分配的地址
	virtual LValue DefVar(LValue var_type, const string& name, LValue init) = 0;
	virtual LValue DefGlobalVar(LValue var_type, const string& name) = 0;
	virtual LValue DefGlobalVar(LValue var_type, const string& name, LValue init) = 0;
	virtual LValue Load(LValue var_addr) = 0;
	virtual LValue Store(LValue var_addr, LValue value) = 0;
	virtual LValue Opt1(const string& opt, LValue value) = 0;
	virtual LValue Opt2(const string& opt, LValue value1, LValue value2) = 0;
	virtual LValue Cmp(const string& opt, LValue value1, LValue value2) = 0;
	virtual LValue Assignment(const string& opt, LValue value1, LValue value2) = 0;
	virtual LValue Dot(LValue value, int num) = 0;
	virtual LValue Select(LValue value, vector<LValue>& args) = 0;
	virtual void   If(LValue cond, LValue father, LValue true_block, LValue true_block_end,
					   LValue false_block, LValue false_block_end, bool isElseWork) = 0;
	virtual void   For(LValue cond, LValue init, LValue pd, LValue work, LValue statement, LValue statement_end, LValue end) = 0;
	virtual void   While(LValue cond, LValue father, LValue pd, LValue statement, LValue statement_end, LValue end) = 0;
	virtual void   DoWhile(LValue statement, LValue pd) = 0;
	virtual void   DoUntil(LValue statement, LValue pd) = 0;
	virtual void   Goto(LValue target) = 0;
	virtual LValue New(LValue var_type, vector<LValue>& args, const string& funcname = "") = 0;
	virtual LValue NewArray(LValue var_type, vector<LValue>& wd, const string& funcname = "") = 0;
	virtual LValue Delete(LValue pointer, const string& funcname = "") = 0;
	virtual LValue DeleteArray(LValue pointer, const string& funcname = "") = 0;
	virtual LValue Return() = 0;
	virtual LValue Return(LValue var) = 0;

	virtual LValue Int8() = 0;
	virtual LValue Int16() = 0;
	virtual LValue Int32() = 0;
	virtual LValue Int64() = 0;
	virtual LValue Float() = 0;
	virtual LValue Double() = 0;
	virtual LValue Void() = 0;

	virtual LValue ConstString(const string& str) = 0;
	virtual LValue ConstInt(int num) = 0;
	virtual LValue ConstDouble(double num) = 0;

	virtual void   BeginModule(const string& name) = 0;
	virtual void   VerifyAndWrite(const string& outfile_name) = 0;
	virtual void   MakeMetaModule(const string& outfile_name, const string& module_name) = 0;

	virtual LValue GetNowBasicBlock() = 0;
	virtual LValue CreateBasicBlock() = 0;
	virtual LValue CreateBasicBlock(LValue func) = 0;

	virtual void   MakeMetaList(vector<string>& list) = 0;
	virtual void   MakeMetaList(const string& name, vector<string>& list, LValue fp) = 0;
	virtual void   CloseTerminator(LValue basicblock, LValue target) = 0;
	virtual void   SetNowBasicBlock(LValue nowBlock) = 0;
	static llcg*   CreateLLVM();

};


#endif // LLCG_H
