/* 
* @Author: sxf
* @Date:   2015-11-23 22:03:33
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 16:42:19
*/

#ifndef LLVM_VALUE_H
#define LLVM_VALUE_H

#include "lvalue.h"

class llvm_value;

typedef shared_ptr<llvm_value> LLVMValue;
#define LLVALUE(T) dynamic_pointer_cast<llvm_value>(T)

namespace llvm {
	class Value;
} // llvm

class llvm_value : public lvalue
{
public:
	llvm_value(llvm::Value* v);
	virtual ~llvm_value();
	
	operator llvm::Value* () const { return data; }
	virtual string getTypeName();
	
private:
	llvm::Value* data;
};



#endif // LLVM_VALUE_H
