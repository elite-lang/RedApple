/* 
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-11 14:15:36
*/

#include "CodeGen.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "CodeGenContext.h"

CodeGen::CodeGen(Node* node) {
	// 创建一个上下文类
	content = new CodeGenContext(node); 
}

CodeGen::~CodeGen() {
	delete content;
}

void CodeGen::Make() {
	InitializeNativeTarget();
	LLVMContext Context;

	// Create some module to put our function into it.
	std::unique_ptr<Module> M(new Module("main", Context));
	content->setModule(M.get());

	content->MakeBegin();

	// 输出编译后的LLVM可读字节码
	outs() << "LLVM module:\n\n" << *M;
	outs() << "\n\n";
	outs().flush();

	// 输出二进制BitCode到a.bc文件
	std::error_code ErrInfo;
	raw_ostream *out = new raw_fd_ostream("a.bc", ErrInfo, sys::fs::F_None);
	WriteBitcodeToFile(M.get(), *out);
	out->flush(); delete out;
	llvm_shutdown();
}


