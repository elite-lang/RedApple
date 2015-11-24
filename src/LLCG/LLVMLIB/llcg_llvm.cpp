/* 
* @Author: sxf
* @Date:   2015-11-23 21:41:19
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 14:51:04
*/

#include "llcg_llvm.h"

#include "llvm_value.h"
#include "llvm_type.h"


LValue llcg_llvm::FuncType(FunctionModel* fmodel) {

} // 返回FunctionType

LValue llcg_llvm::FuncType(LValue ret_type, vector<LValue>& types, bool isNotSure) {
	LLVMType ret = LLTYPE(ret_type);
	vector<Type*> arg_types;
	for (auto p : types) {
		LLVMType t = LLTYPE(p);
		arg_types.push_back(*t);
	}
	FunctionType* func_type = FunctionType::get(*ret, arg_types, 
		/*vararg*/isNotSure);
	return LValue(new llvm_type(func_type));
}

LValue llcg_llvm::GetOrInsertFunction(FunctionModel* fmodel) {

} // 返回Function

LValue llcg_llvm::GetOrInsertFunction(string& name, LValue func_type) {
	LLVMType _func_type = LLTYPE(func_type);
	Type* t = *_func_type;
	Constant* f = M->getOrInsertFunction(name, (FunctionType*)t);
	return LValue(new llvm_value(f));
}

LValue llcg_llvm::GetOrInsertFunction(string& name, LValue ret_type, vector<LValue>& types, bool isNotSure) {

}

void   llcg_llvm::FunctionBodyBegin(LValue func) {

} // 设置当前BasicBlock

void   llcg_llvm::FunctionBodyEnd() {

} // 处理函数结束

LValue llcg_llvm::Call(FunctionModel* fmodel, vector<LValue>& args) {
	LLVMValue _func = LLVALUE(fmodel->getFunction());
	vector<Value*> fargs;
	for (auto p : args) {
		LLVMValue t = LLVALUE(p);
		fargs.push_back(*t);
	}

	CallInst *call = CallInst::Create(_func, fargs, "", nowBlock);
	return LValue(new llvm_value(call));
} // 返回CallInst

LValue llcg_llvm::Call(LValue func, vector<LValue>& args) {
	LLVMValue _func = LLVALUE(func);
	vector<Value*> fargs;
	for (auto p : args) {
		LLVMValue t = LLVALUE(p);
		fargs.push_back(*t);
	}

	CallInst *call = CallInst::Create(*_func, fargs, "", nowBlock);
	return LValue(new llvm_value(call));
}

LValue llcg_llvm::Struct(StructModel* smodel) {

} // 返回StructType

LValue llcg_llvm::Struct(string& name, vector<LValue>& types) {

}


LValue llcg_llvm::DefVar(LValue var_type, string& name) {

}

LValue llcg_llvm::DefGlobalVar(LValue var_type, string& name) {

}

LValue llcg_llvm::Load(LValue var_addr) {

}

LValue llcg_llvm::Store(LValue var_addr, LValue value) {

}

LValue llcg_llvm::Opt1(string& opt, LValue value) {

}

LValue llcg_llvm::Opt2(string& opt, LValue value1, LValue value2) {

}

LValue llcg_llvm::For(LValue cond, LValue init, LValue pd, LValue work, LValue statement) {
	Value* condition = *LLVALUE(cond);
	Value* _init = *LLVALUE(init);
	Value* _pd   = *LLVALUE(pd);
	Value* _work = *LLVALUE(work);
	Value* _statement = *LLVALUE(statement);
	BasicBlock* init_block = dyn_cast<BasicBlock>(_init);
	BasicBlock* end_block  = dyn_cast<BasicBlock>(_pd);
	BasicBlock* work_block = dyn_cast<BasicBlock>(_work);
	BasicBlock* do_block   = dyn_cast<BasicBlock>(_statement);
	
	BasicBlock* false_block = createBlock();

	BranchInst* branch = BranchInst::Create(work_block, false_block, condition, end_block);
	BranchInst::Create(end_block, init_block);
	BranchInst::Create(do_block,  work_block);
	BranchInst::Create(end_block, do_block);

}

LValue llcg_llvm::While(LValue pd, LValue statement) {

}

LValue llcg_llvm::DoWhile(LValue statement, LValue pd) {

}

LValue llcg_llvm::DoUntil(LValue statement, LValue pd) {

}

LValue llcg_llvm::New(LValue var_type, vector<LValue>& args) {

}

LValue llcg_llvm::NewArray(LValue var_type, vector<LValue>& wd) {

}

LValue llcg_llvm::Int8() {
	return LValue(new llvm_type(Type::getInt8Ty(context)));
}

LValue llcg_llvm::Int16() {
	return LValue(new llvm_type(Type::getInt16Ty(context)));
}

LValue llcg_llvm::Int32() {
	return LValue(new llvm_type(Type::getInt32Ty(context)));
}

LValue llcg_llvm::Int64() {
	return LValue(new llvm_type(Type::getInt64Ty(context)));
}

LValue llcg_llvm::Float() {
	return LValue(new llvm_type(Type::getFloatTy(context)));
}

LValue llcg_llvm::Double() {
	return LValue(new llvm_type(Type::getDoubleTy(context)));
}

LValue llcg_llvm::Void() {
	return LValue(new llvm_type(Type::getVoidTy(context)));
}


Constant* geti8StrVal(Module& M, char const* str, Twine const& name) {
    LLVMContext& ctx = M.getContext(); // 千万别用Global Context
    Constant* strConstant = ConstantDataArray::getString(ctx, str);
    GlobalVariable* GVStr =
        new GlobalVariable(M, strConstant->getType(), true,
                           GlobalValue::InternalLinkage, strConstant, name);
    Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
    return strVal;
}

LValue llcg_llvm::ConstString(string& str) {
	return LValue(new llvm_value(geti8StrVal(*M, str.c_str(), "")));
}

LValue llcg_llvm::ConstInt(int num) {
	Type* t = Type::getInt64Ty(context);
	return LValue(new llvm_value(ConstantInt::get(t, num)));
}

LValue llcg_llvm::ConstDouble(double num) {
	Type* t = Type::getDoubleTy(context);
	return LValue(new llvm_value(ConstantFP::get(t, num)));
}

extern const LibFunc stdlibs[];

void llcg_llvm::BeginModule(string& name) {
	M = make_unique<Module>(name, context);
	register_stdlib(stdlibs);
}

void llcg_llvm::register_stdlib(const LibFunc* libs_func) {
	while (libs_func != NULL) {
		(*libs_func)(M.get());
		++libs_func;
	}
}

LValue llcg_llvm::GetNowBasicBlock() {
	return LValue(new llvm_value(nowBlock));
}

LValue llcg_llvm::CreateBasicBlock() {
	return LValue(new llvm_value(createBlock()));
}

LValue llcg_llvm::CreateBasicBlock(LValue func) {
	Value* v = *LLVALUE(func);
	Function* f = dyn_cast<Function>(v);
	return LValue(new llvm_value(createBlock(f)));
}

void llcg_llvm::VerifyAndWrite(string& outfile_name) {
	verifyModuleAndWrite(M.get(), outfile_name);
}

void llcg_llvm::verifyModuleAndWrite(llvm::Module* M, string& outfile_name) {
    // 校验问题, 这个函数需要一个输出流来打印错误信息
    if (verifyModule(*M, &errs())) {
        errs() << "构建LLVM字节码出错!\n";
        exit(1);
    }

    // 输出编译后的LLVM可读字节码
    outs() << "LLVM module:\n\n" << *M;
    outs() << "\n\n";
    outs().flush();

    // 输出二进制BitCode到.bc文件
    std::error_code ErrInfo;
    raw_ostream *out = new raw_fd_ostream(outfile_name.c_str(), ErrInfo, sys::fs::F_None);
    WriteBitcodeToFile(M, *out);
    out->flush(); delete out;
}

BasicBlock* llcg_llvm::createBlock() {
	return nowBlock = BasicBlock::Create(context, "", nowFunc);
}

BasicBlock* llcg_llvm::createBlock(Function* f) {
	nowFunc = f;
	return nowBlock = BasicBlock::Create(context, "entry", f);
}