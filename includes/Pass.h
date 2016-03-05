#ifndef PASS_H
#define PASS_H


#include <string>
#include <map>
#include <stack>
using namespace std;

#include "CodeGenFunction.h"


class Pass {
public:
    Pass (const FuncReg* macro_funcs);
    Pass (const FuncReg* macro_funcs, const FuncReg* macro_funcs2);
    virtual ~Pass ();

    /**
	 * @brief 正式扫描前的初始化
	 */
	virtual void Init();


    /**
	 * @brief 查找一个名称是否为一个C宏
	 * @details 查找一个名称是否为一个C宏
	 *
	 * @param str 宏的名字
	 * @return 如果有，则返回函数指针，否则返回NULL
	 */
	virtual ICodeGenFunction* getMacro(const string& str);

	// C++注册宏
	// void AddMacros(const FuncReg* macro_funcs); // 为只添加不替换保留

	/**
	 * @brief 添加或替换一条宏
	 * @details 添加或替换一条宏，如果当前名称存在则替换，没有则添加
	 *
	 * @param macro_funcs 一组宏定义数组，类似Lua中函数的定义方式
	 */
	virtual void AddOrReplaceMacros(const FuncReg* macro_funcs);

	virtual void AddOrReplaceMacros(const string& name, ICodeGenFunction* func);


	/**
	 * @brief 移除全部宏指令
	 * @details 移除全部宏指令
	 */
	virtual void RemoveAllMacros();

    /**
     * 临时保存当前宏到堆栈中，还可以恢复
     * @method SaveMacros
     */
    virtual void SaveMacros();

    /**
     * 恢复之前保存的堆栈
     * @method RecoverMacros
     */
	virtual void RecoverMacros();


protected:

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


};


#endif /* end of include guard: PASS_H */
