/* 
* @Author: sxf
* @Date:   2015-12-19 10:44:37
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-19 13:03:03
*/

#include "CodeGenFunction.h"
#include <cassert>
#include "CodeGenContext.h"

CodeGenFunction::CodeGenFunction() {

}

CodeGenFunction::CodeGenFunction(CodeGenCFunction _cfunc) {
	*this = _cfunc;
}

CodeGenFunction::CodeGenFunction(int _lua_code) {
	*this = _lua_code;
}

const CodeGenFunction& CodeGenFunction::operator= (const CodeGenFunction& obj) {
	this->isCFunction = obj.isCFunction;
	this->func = obj.func;
	return *this;
}

const CodeGenFunction& CodeGenFunction::operator= (CodeGenCFunction _cfunc) {
	isCFunction = true;
	func.cfunc = _cfunc;
	return *this;
}

const CodeGenFunction& CodeGenFunction::operator= (int _lua_code) {
	isCFunction = false;
	func.lua_code = _lua_code;
	return *this;
}


LValue CodeGenFunction::call(CodeGenContext* ctx, Node* n) {
	if (isCFunction) {
		CodeGenCFunction f = getCFunc();
		return f(ctx, n);
	} else {
		CodeRunLua f = ctx->getCodeRunLua();
		return f(getLuaCode(), ctx, n);
	}
}

bool CodeGenFunction::isCFunc() {
	return isCFunction;
}

CodeGenCFunction CodeGenFunction::getCFunc() {
	assert(isCFunction);
	return func.cfunc;
}

int CodeGenFunction::getLuaCode() {
	assert(!isCFunction);
	return func.lua_code;
}
