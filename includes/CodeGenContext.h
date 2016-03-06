/*
* @Author: sxf
* @Date:   2015-10-10 18:44:44
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-26 15:10:36
*
* 代码生成的上下文类, 是C实现宏的最核心功能类
*/

#ifndef CODE_GEN_CONTENT_H
#define CODE_GEN_CONTENT_H

#include "ICodeGenContext.h"


/**
 * @brief 代码生成上下文, 是代码生成中的关键信息存储类
 * @details 存储最关键的数据, 例如符号栈, LLVM生成器对象.
 *          在宏翻译过程中, 该上下文指针将会传入翻译函数中.\n
 *          该上下文在编译器中, 同一时刻应该仅存在一份,
 *          生存周期为一个包的编译开始到包内全部代码翻译完毕.\n
 *          切换上下文时, 相当于清空符号栈, 释放资源, 重启LLVM后端
 *          会导致之前分析的符号全部消失
 */
class CodeGenContext : public ICodeGenContext
{
public:
	CodeGenContext();
	virtual ~CodeGenContext();

	void Init();

	/**
	 * @brief 这个函数是用来一条条翻译Node宏的
	 * @details 这个函数是用来一条条翻译Node宏的
	 *
	 * @param node 要翻译的语法节点
	 * @return 翻译后的后端变量，往往是一个表达式或一个语句返回的值
	 */
	virtual LValue MacroMake(Node* node);


	// 获取当前模块中已注册的函数
	virtual LValue getFunction(Node* node);
	virtual LValue getFunction(const std::string& name);


	// 用户宏的查找与声明设置
	virtual shared_ptr<MacroModel> getUserMacro(const std::string& name);
	virtual void  setUserMacro(const std::string& name, Node* node);

	virtual shared_ptr<FunctionModel> getFunctionModel(const std::string& name);
	virtual shared_ptr<StructModel> getStructModel(const std::string& name);

	// 类型的定义和查找
	virtual void DefType(string name, LValue t);
	virtual LValue FindType(const string& name);
	virtual LValue FindType(Node*);
	virtual LValue FindSrcType(const string& name);
	virtual LValue FindSrcType(Node*);

	/**
	 * @brief 定义一个变量 有可能是全局变量
	 *
	 * @param name 变量名
	 * @param addr 存储地址
	 */
	virtual void DefVar(const string& name, LValue addr);

	/**
	 * @brief 在符号表中查找一个变量
	 *
	 * @param name 变量名
	 * @return 变量的存储地址
	 */
	virtual LValue FindVar(const string& name);

	virtual bool isSave() { return _save; }
	virtual void setIsSave(bool save) { _save = save; }

	virtual id* FindST(Node* node) const;
	virtual id* FindST(const string& str) const;
	IDTable* st;

	/**
	 * @brief 获取低层次代码生成器，目前只有llvm
	 * @return 生成器指针
	 */
	virtual llcg* getLLCG() { return codeGenerator; }

	virtual void setNowPass(Pass* pass) {
		now_pass = pass;
	}

	virtual Pass* getNowPass() {
		return now_pass;
	}

	virtual PassManager* getPassManager() {
		return pm;
	}

	virtual void setPassManager(PassManager* pm) {
		this->pm = pm;
	}

private:

	llcg* codeGenerator;

	Pass* now_pass;
	PassManager* pm;

	void setNormalType();

	/**
	 * 用来记录当前是读取还是存入状态
	 */
	bool _save;
};


#endif // CODE_GEN_CONTENT_H
