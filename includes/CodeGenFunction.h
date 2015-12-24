/* 
* @Author: sxf
* @Date:   2015-12-19 10:43:27
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:10:10
*/


#ifndef CODE_GEN_FUNCTION_H
#define CODE_GEN_FUNCTION_H


#include "ICodeGenFunction.h"

/**
 * @brief 定义宏翻译函数指针
 */
typedef LValue (*CodeGenCFunction)(CodeGenContext*, Node*);

class CodeGenFunction : public ICodeGenFunction
{
public:
	CodeGenFunction();
	CodeGenFunction(CodeGenCFunction _cfunc);
	const CodeGenFunction& operator=(const CodeGenFunction& obj);
	const CodeGenFunction& operator=(CodeGenCFunction _cfunc);

	virtual LValue call(CodeGenContext*, Node*);
	CodeGenCFunction getCFunc();
	
private:
	CodeGenCFunction cfunc;
};

/**
 * @brief 宏翻译函数注册结构体
 * @details name 为宏名, func 为函数指针
 */
typedef struct _funcReg
{
	const char*      name;
	CodeGenCFunction func;
} FuncReg;



#endif // CODE_GEN_FUNCTION_H
