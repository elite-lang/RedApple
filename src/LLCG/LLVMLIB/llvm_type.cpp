/* 
* @Author: sxf
* @Date:   2015-11-24 15:40:26
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 15:41:01
*/

#include "llvm_type.h"

llvm_type::llvm_type(llvm::Type* v) {
	data = v;
}

llvm_type::~llvm_type() {

}