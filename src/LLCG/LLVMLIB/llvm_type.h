/* 
* @Author: sxf
* @Date:   2015-11-24 08:43:52
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 12:04:04
*/


#ifndef LLVM_TYPE_H
#define LLVM_TYPE_H

#include "lvalue.h"

class llvm_type;

typedef shared_ptr<llvm_type> LLVMType;
#define LLTYPE(T) dynamic_pointer_cast<llvm_type>(T)

namespace llvm {
	class Type;
} // llvm

class llvm_type : public lvalue
{
public:
	llvm_type(llvm::Type* v);
	virtual ~llvm_type();
	
	operator llvm::Type* () const { return data; }

private:
	llvm::Type* data;
};



#endif // LLVM_TYPE_H
