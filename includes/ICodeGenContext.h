#ifndef ICODE_GEN_CONTEXT_H
#define ICODE_GEN_CONTEXT_H

#include <map>
#include <stack>
#include <string>
using namespace std;

#include "LLCG/llcg.h"
#include "MetaModel/StructModel.h"
#include "MetaModel/FunctionModel.h"
#include "MetaModel/MacroModel.h"
#include "CodeGenFunction.h"

class Node;
class Pass;
class PassManager;
class IDTable;
struct id;

class ICodeGenContext
{
public:

	/**
	 * @brief 这个函数是用来一条条翻译Node宏的
	 * @details 这个函数是用来一条条翻译Node宏的
	 *
	 * @param node 要翻译的语法节点
	 * @return 翻译后的后端变量，往往是一个表达式或一个语句返回的值
	 */
	virtual LValue MacroMake(Node* node) = 0;


	// 获取当前模块中已注册的函数
	virtual LValue getFunction(Node* node) = 0;
	virtual LValue getFunction(const std::string& name) = 0;


	// 用户宏的查找与声明设置
	virtual shared_ptr<MacroModel> getUserMacro(const std::string& name) = 0;
	virtual void  setUserMacro(const std::string& name, Node* node) = 0;

	virtual shared_ptr<FunctionModel> getFunctionModel(const std::string& name) = 0;
	virtual shared_ptr<StructModel> getStructModel(const std::string& name) = 0;


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


	virtual bool isSave() = 0;
	virtual void setIsSave(bool save) = 0;

	virtual id* FindST(Node* node) const = 0;
	virtual id* FindST(const string& str) const = 0;

	/**
	 * @brief 获取低层次代码生成器，目前只有llvm
	 * @return 生成器指针
	 */
	virtual llcg* getLLCG() = 0;

	virtual void setNowPass(Pass* pass) = 0;

	virtual Pass* getNowPass() = 0;

	virtual PassManager* getPassManager() = 0;
};


#endif /* end of include guard: ICODE_GEN_CONTEXT_H */
