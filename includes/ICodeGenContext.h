#ifndef ICODE_GEN_CONTEXT_H
#define ICODE_GEN_CONTEXT_H

#include "Model/Node.h"
#include <map>
#include <stack>
#include <string>
using namespace std;

#include "LLCG/llcg.h"
#include "MetaModel/StructModel.h"
#include "MetaModel/FunctionModel.h"
#include "MetaModel/MacroModel.h"
#include "CodeGenFunction.h"

class IDTable;
struct id;

class ICodeGenContext
{
public:

	/**
	 * @brief 预先扫描宏时的初始化，初始化第一趟扫描
	 */
	virtual void PreMacro() = 0;

	/**
	 * @brief 预先扫描类型名称时的初始化，初始化第二趟扫描
	 */
	virtual void PreInit() = 0;

	/**
	 * @brief 预先扫描类型和函数定义时的初始化，初始化第三趟扫描
	 */
	virtual void PreTypeInit() = 0;

	/**
	 * @brief 正式扫描前的初始化
	 */
	virtual void Init() = 0;

	/**
	 * @brief 这个函数是用来一条条翻译Node宏的
	 * @details 这个函数是用来一条条翻译Node宏的
	 *
	 * @param node 要翻译的语法节点
	 * @return 翻译后的后端变量，往往是一个表达式或一个语句返回的值
	 */
	virtual LValue MacroMake(Node* node) = 0;


	/**
	 * @brief 递归翻译该节点下的所有宏
	 * @param node 要翻译的节点
	 */
	virtual void MacroMakeAll(Node* node) = 0;

	/**
	 * @brief 查找一个名称是否为一个C宏
	 * @details 查找一个名称是否为一个C宏
	 *
	 * @param str 宏的名字
	 * @return 如果有，则返回函数指针，否则返回NULL
	 */
	virtual ICodeGenFunction* getMacro(const string& str) = 0;

	// C++注册宏
	// void AddMacros(const FuncReg* macro_funcs); // 为只添加不替换保留

	/**
	 * @brief 添加或替换一条宏
	 * @details 添加或替换一条宏，如果当前名称存在则替换，没有则添加
	 *
	 * @param macro_funcs 一组宏定义数组，类似Lua中函数的定义方式
	 */
	virtual void AddOrReplaceMacros(const FuncReg* macro_funcs) = 0;

	virtual void AddOrReplaceMacros(const string& name, ICodeGenFunction* func) = 0;


	/**
	 * @brief 移除全部宏指令
	 * @details 移除全部宏指令
	 */
	virtual void RemoveAllMacros() = 0;

	// 获取当前模块中已注册的函数
	virtual LValue getFunction(Node* node) = 0;
	virtual LValue getFunction(const std::string& name) = 0;


	// 用户宏的查找与声明设置
	virtual shared_ptr<MacroModel> getUserMacro(const std::string& name) = 0;
	virtual void  setUserMacro(const std::string& name, Node* node) = 0;

	virtual shared_ptr<FunctionModel> getFunctionModel(const std::string& name) = 0;
	virtual shared_ptr<StructModel> getStructModel(const std::string& name) = 0;

	virtual void ScanOther(Node* node) = 0;

	// 类型的定义和查找
	virtual void DefType(string name, LValue t) = 0;
	virtual LValue FindType(const string& name) = 0;
	virtual LValue FindType(Node*) = 0;
	virtual LValue FindSrcType(const string& name) = 0;
	virtual LValue FindSrcType(Node*) = 0;

	/**
	 * @brief 定义一个变量 有可能是全局变量
	 *
	 * @param name 变量名
	 * @param addr 存储地址
	 */
	virtual void DefVar(const string& name, LValue addr) = 0;

	/**
	 * @brief 在符号表中查找一个变量
	 *
	 * @param name 变量名
	 * @return 变量的存储地址
	 */
	virtual LValue FindVar(const string& name) = 0;


	virtual void SaveMacros() = 0;
	virtual void RecoverMacros() = 0;

	virtual bool isSave() = 0;
	virtual void setIsSave(bool save) = 0;

	virtual id* FindST(Node* node) const = 0;
	virtual id* FindST(const string& str) const = 0;

	/**
	 * @brief 获取低层次代码生成器，目前只有llvm
	 * @return 生成器指针
	 */
	virtual llcg* getLLCG() = 0;


};


#endif /* end of include guard: ICODE_GEN_CONTEXT_H */
