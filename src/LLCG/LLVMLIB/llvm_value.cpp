/* 
* @Author: sxf
* @Date:   2015-11-24 15:39:17
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 16:55:59
*/

#include "llvm_value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"

using namespace llvm;

llvm_value::llvm_value(llvm::Value* v) {
	data = v;
}

llvm_value::~llvm_value() {

}

string llvm_value::getTypeName() {
	Type* t = data->getType();
	StructType* st = (StructType*)(t->getPointerElementType());
	return st->getName().str();
}