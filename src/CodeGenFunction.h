/* 
* @Author: sxf
* @Date:   2015-12-19 10:43:27
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-19 12:59:24
*/


#ifndef CODE_GEN_FUNCTION_H
#define CODE_GEN_FUNCTION_H

#include "Node.h"
#include "LLCG/llcg.h"
class CodeGenContext;

/**
 * @brief 定义宏翻译函数指针
 */
typedef LValue (*CodeGenCFunction)(CodeGenContext*, Node*);

/**
 * @brief 定义Lua宏翻译函数指针
 */
typedef LValue (*CodeRunLua)(int, CodeGenContext*, Node*);


class CodeGenFunction
{
public:
	CodeGenFunction();
	CodeGenFunction(CodeGenCFunction _cfunc);
	CodeGenFunction(int _lua_code);
	const CodeGenFunction& operator=(const CodeGenFunction& obj);
	const CodeGenFunction& operator=(CodeGenCFunction _cfunc);
	const CodeGenFunction& operator=(int _lua_code);

	LValue call(CodeGenContext*, Node*);
	bool isCFunc();
	CodeGenCFunction getCFunc();
	int getLuaCode();
private:
	bool isCFunction;
	union {
		CodeGenCFunction cfunc;
		int lua_code;
	} func;
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
