/* 
* @Author: sxf
* @Date:   2015-11-24 15:39:17
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 15:39:57
*/

#include "llvm_value.h"

llvm_value::llvm_value(llvm::Value* v) {
	data = v;
}

llvm_value::~llvm_value() {

}
