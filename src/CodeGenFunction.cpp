/* 
* @Author: sxf
* @Date:   2015-12-19 10:44:37
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:10:42
*/

#include "CodeGenFunction.h"
#include <cassert>
#include "CodeGenContext.h"

CodeGenFunction::CodeGenFunction() {
}

CodeGenFunction::CodeGenFunction(CodeGenCFunction _cfunc) {
	*this = _cfunc;
}

const CodeGenFunction& CodeGenFunction::operator= (const CodeGenFunction& obj) {
	this->cfunc = obj.cfunc;
	return *this;
}

const CodeGenFunction& CodeGenFunction::operator= (CodeGenCFunction _cfunc) {
	this->cfunc = _cfunc;
	return *this;
}

LValue CodeGenFunction::call(CodeGenContext* ctx, Node* n) {
	CodeGenCFunction f = getCFunc();
	return f(ctx, n);
}

CodeGenCFunction CodeGenFunction::getCFunc() {
	return this->cfunc;
}

