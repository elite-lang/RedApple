/*
* @Author: sxf
* @Date:   2015-12-19 10:15:48
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 17:16:05
*/
#ifndef LLVG_L_H
#define LLVG_L_H


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
class llcg_l
{
public:
	virtual lvalue* l_FuncType(FunctionModel* fmodel) = 0; // 返回FunctionType
	virtual lvalue* l_FuncType(lvalue* ret_type, vector<lvalue*>& types, bool isNotSure = false) = 0;
	virtual lvalue* l_GetOrInsertFunction(FunctionModel* fmodel) = 0; // 返回Function
	virtual lvalue* l_GetOrInsertFunction(const string& name, lvalue* func_type) = 0;
	virtual lvalue* l_GetOrInsertFunction(const string& name, lvalue* ret_type, vector<lvalue*>& types, bool isNotSure = false) = 0;
	virtual void   l_FunctionBodyBegin(lvalue* func, vector<string>& name_list) = 0; // 设置当前BasicBlock
	virtual void   l_FunctionBodyEnd() = 0; // 处理函数结束
	virtual lvalue* l_getFunction(const string& name) = 0; // 从当前模块中获取一个函数
	virtual lvalue* l_Call(FunctionModel* fmodel, vector<lvalue*>& args) = 0; // 返回CallInst
	virtual lvalue* l_Call(lvalue* func, vector<lvalue*>& args) = 0;
	virtual lvalue* l_Struct(StructModel* smodel) = 0; // 返回StructType
	virtual lvalue* l_Struct(lvalue* _struct, vector<lvalue*>& types) = 0;
	virtual lvalue* l_DeclareStruct(const string& name) = 0;
	virtual lvalue* l_DefVar(lvalue* var_type, const string& name) = 0; // 返回分配的地址
	virtual lvalue* l_DefVar(lvalue* var_type, const string& name, lvalue* init) = 0;
	virtual lvalue* l_DefGlobalVar(lvalue* var_type, const string& name) = 0;
	virtual lvalue* l_DefGlobalVar(lvalue* var_type, const string& name, lvalue* init) = 0;
	virtual lvalue* l_Load(lvalue* var_addr) = 0;
	virtual lvalue* l_Store(lvalue* var_addr, lvalue* value) = 0;
	virtual lvalue* l_Opt1(const string& opt, lvalue* value) = 0;
	virtual lvalue* l_Opt2(const string& opt, lvalue* value1, lvalue* value2) = 0;
	virtual lvalue* l_Cmp(const string& opt, lvalue* value1, lvalue* value2) = 0;
	virtual lvalue* l_Assignment(const string& opt, lvalue* value1, lvalue* value2) = 0;
	virtual lvalue* l_Dot(lvalue* value, int num) = 0;
	virtual lvalue* l_Select(lvalue* value, vector<lvalue*>& args) = 0;
	virtual void   l_If(lvalue* cond, lvalue* father, lvalue* true_block, lvalue* true_block_end,
					   lvalue* false_block, lvalue* false_block_end, bool isElseWork) = 0;
	virtual void   l_For(lvalue* cond, lvalue* init, lvalue* pd, lvalue* work, lvalue* statement, lvalue* statement_end, lvalue* end) = 0;
	virtual void   l_While(lvalue* cond, lvalue* father, lvalue* pd, lvalue* statement, lvalue* statement_end, lvalue* end) = 0;
	virtual void   l_DoWhile(lvalue* statement, lvalue* pd) = 0;
	virtual void   l_DoUntil(lvalue* statement, lvalue* pd) = 0;
	virtual lvalue* l_New(lvalue* var_type, vector<lvalue*>& args, const string& funcname = "") = 0;
	virtual lvalue* l_NewArray(lvalue* var_type, vector<lvalue*>& wd, const string& funcname = "") = 0;
	virtual lvalue* l_Delete(lvalue* pointer, const string& funcname = "") = 0;
	virtual lvalue* l_DeleteArray(lvalue* pointer, const string& funcname = "") = 0;
	virtual lvalue* l_Return() = 0;
	virtual lvalue* l_Return(lvalue* var) = 0;

	virtual lvalue* l_Int8() = 0;
	virtual lvalue* l_Int16() = 0;
	virtual lvalue* l_Int32() = 0;
	virtual lvalue* l_Int64() = 0;
	virtual lvalue* l_Float() = 0;
	virtual lvalue* l_Double() = 0;
	virtual lvalue* l_Void() = 0;

	virtual lvalue* l_ConstString(const string& str) = 0;
	virtual lvalue* l_ConstInt(int num) = 0;
	virtual lvalue* l_ConstDouble(double num) = 0;

	virtual void   l_BeginModule(const string& name) = 0;
	virtual void   l_VerifyAndWrite(const string& outfile_name) = 0;
	virtual void   l_MakeMetaModule(const string& outfile_name, const string& module_name) = 0;

	virtual lvalue* l_GetNowBasicBlock() = 0;
	virtual lvalue* l_CreateBasicBlock() = 0;
	virtual lvalue* l_CreateBasicBlock(lvalue* func) = 0;

	virtual void   l_MakeMetaList(vector<string>& list) = 0;
	virtual void   l_MakeMetaList(const string& name, vector<string>& list, lvalue* fp) = 0;
	virtual void   l_CloseTerminator(lvalue* basicblock, lvalue* target) = 0;

};





#endif // LLVG_L_H
