/* 
* @Author: sxf
* @Date:   2015-10-10 18:44:44
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:07:14
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

#include "LLCG/llcg.h"
#include "idtable.h"
#include "MetaModel/StructModel.h"
#include "MetaModel/FunctionModel.h"
#include "MetaModel/MacroModel.h"
#include "CodeGenFunction.h"


/**
 * @brief 代码生成上下文, 是代码生成中的关键信息存储类
 * @details 存储最关键的数据, 例如符号栈, LLVM生成器对象.
 *          在宏翻译过程中, 该上下文指针将会传入翻译函数中.\n
 *          该上下文在编译器中, 同一时刻应该仅存在一份, 
 *          生存周期为一个包的编译开始到包内全部代码翻译完毕.\n
 *          切换上下文时, 相当于清空符号栈, 释放资源, 重启LLVM后端
 *          会导致之前分析的符号全部消失
 */
class CodeGenContext
{
public:
	CodeGenContext(Node* node);
	~CodeGenContext();

	/**
	 * @brief 预先扫描宏时的初始化，初始化第一趟扫描
	 */
	void PreMacro();

	/**
	 * @brief 预先扫描类型名称时的初始化，初始化第二趟扫描
	 */
	void PreInit();

	/**
	 * @brief 预先扫描类型和函数定义时的初始化，初始化第三趟扫描
	 */
	void PreTypeInit();

	/**
	 * @brief 正式扫描前的初始化
	 */
	void Init();

	/**
	 * @brief 这个函数是用来一条条翻译Node宏的
	 * @details 这个函数是用来一条条翻译Node宏的
	 * 
	 * @param node 要翻译的语法节点
	 * @return 翻译后的后端变量，往往是一个表达式或一个语句返回的值
	 */ 
	LValue MacroMake(Node* node);


	/** 
	 * @brief 递归翻译该节点下的所有宏
	 * @param node 要翻译的节点
	 */
	void MacroMakeAll(Node* node) {
		MacroMake(node);
	}

	/**
	 * @brief 查找一个名称是否为一个C宏
	 * @details 查找一个名称是否为一个C宏
	 * 
	 * @param str 宏的名字
	 * @return 如果有，则返回函数指针，否则返回NULL
	 */
	ICodeGenFunction* getMacro(const string& str);

	// C++注册宏
	// void AddMacros(const FuncReg* macro_funcs); // 为只添加不替换保留
	
	/**
	 * @brief 添加或替换一条宏
	 * @details 添加或替换一条宏，如果当前名称存在则替换，没有则添加
	 * 
	 * @param macro_funcs 一组宏定义数组，类似Lua中函数的定义方式
	 */
	void AddOrReplaceMacros(const FuncReg* macro_funcs);

	/**
	 * @brief 移除全部宏指令
	 * @details 移除全部宏指令
	 */
	void RemoveAllMacros(); 

	// 获取当前模块中已注册的函数
	LValue getFunction(Node* node);
	LValue getFunction(const std::string& name);

	void nowFunction(LValue _nowFunc);

	// 用户宏的查找与声明设置
	shared_ptr<MacroModel> getUserMacro(const std::string& name);
	void  setUserMacro(const std::string& name, Node* node);

	shared_ptr<FunctionModel> getFunctionModel(const std::string& name);
	shared_ptr<StructModel> getStructModel(const std::string& name);

	void ScanOther(Node* node);

	// 类型的定义和查找
	void DefType(string name, LValue t);
	LValue FindType(const string& name);
	LValue FindType(Node*);
	LValue FindSrcType(const string& name);
	LValue FindSrcType(Node*);
	
	/**
	 * @brief 定义一个变量 有可能是全局变量
	 * 
	 * @param name 变量名
	 * @param addr 存储地址
	 */
	void DefVar(const string& name, LValue addr);
	
	/**
	 * @brief 在符号表中查找一个变量
	 * 
	 * @param name 变量名
	 * @return 变量的存储地址
	 */
	LValue FindVar(const string& name);


	void SaveMacros();
	void RecoverMacros();

	bool isSave() { return _save; }
	void setIsSave(bool save) { _save = save; }

	id* FindST(Node* node) const;

	id* FindST(const string& str) const {
		return st->find(str);
	}
	IDTable* st;

	/**
	 * @brief 获取低层次代码生成器，目前只有llvm
	 * @return 生成器指针
	 */
	llcg* getLLCG() { return codeGenerator; }

private:

	llcg* codeGenerator;

	/**
	 * @brief 语法树根节点
	 * @details 额外保存的语法树根节点，为方便树的遍历而记录
	 */ 
	Node* root;

	/**
	 * @brief 宏定义表，用来查找是否有该宏定义的
	 * @details 宏定义表，里面存放有对应名称的C语言宏处理函数
	 */
	map<string, ICodeGenFunction*> macro_map;

	/**
	 * @brief 这个栈是用来临时保存上面的查询表的
	 * @details 某些情况，可能会对宏处理的函数进行临时的变更，就需要保存和恢复全部的状态，这个栈是用来记录宏表的
	 */
	stack<map<string, ICodeGenFunction*> > macro_save_stack;

	void setNormalType();

	/**
	 * 用来记录当前是读取还是存入状态
	 */ 
	bool _save;
};


#endif // CODE_GEN_CONTENT_H
