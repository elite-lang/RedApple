/* 
* @Author: sxf
* @Date:   2015-11-23 21:41:19
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-26 08:11:08
*/

#include "llcg_llvm.h"

#include "llvm_value.h"
#include "llvm_type.h"

llcg_llvm::llcg_llvm() {
	meta_M = make_unique<Module>("", context);
	register_metalib();
}

llcg_llvm::~llcg_llvm() {
	llvm_shutdown();
}

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

void   llcg_llvm::FunctionBodyBegin(LValue func, vector<string>& name_list) {
	Value* _func = *LLVALUE(func);
	Function* F = dyn_cast<Function>(_func);
	int i = 0;
	for (auto arg = F->arg_begin(); i != name_list.size(); ++arg, ++i) {
		arg->setName(name_list[i]);
		Value* argumentValue = arg;
		ValueSymbolTable* st = nowBlock->getValueSymbolTable();
		Value* v = st->lookup(name_list[i]);
		new StoreInst(argumentValue, v, false, nowBlock);
	}
} // 设置当前BasicBlock

void   llcg_llvm::FunctionBodyEnd() {
	if (nowBlock->getTerminator() == NULL)
		ReturnInst::Create(context, nowBlock);
} // 处理函数结束

LValue llcg_llvm::getFunction(string& name) {
	Function* f = M->getFunction(name);
	return LValue(new llvm_value(f));
}

LValue llcg_llvm::Call(FunctionModel* fmodel, vector<LValue>& args) {
	// LLVMValue _func = LLVALUE(fmodel->getFunction());
	vector<Value*> fargs;
	for (auto p : args) {
		LLVMValue t = LLVALUE(p);
		fargs.push_back(*t);
	}

	// CallInst *call = CallInst::Create(_func, fargs, "", nowBlock);
	// return LValue(new llvm_value(call));
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

LValue llcg_llvm::Struct(LValue _struct, vector<LValue>& types) {
	std::vector<Type*> type_vec;
	for (auto p : types) {
		Type* t = *LLTYPE(p);
		type_vec.push_back(t);
	}
	Type* struct_type = *LLTYPE(_struct);
	StructType* st = dyn_cast<StructType>(struct_type);
	st->setBody(type_vec);
	return LValue(new llvm_type(st));
}

LValue llcg_llvm::DeclareStruct(string& name) {
	StructType* st = StructType::create(context, name);
	return LValue(new llvm_type(st));
}


LValue llcg_llvm::DefVar(LValue var_type, string& name) {
	Type* t = *LLTYPE(var_type);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	return LValue(new llvm_value(alloc));
}

LValue llcg_llvm::DefVar(LValue var_type, string& name, LValue init) {
	Type* t = *LLTYPE(var_type);
	Value* v = *LLVALUE(init);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	new StoreInst(v, alloc, false, nowBlock);
	return LValue(new llvm_value(alloc));
}

LValue llcg_llvm::DefGlobalVar(LValue var_type, string& name) {
	Type* t = *LLTYPE(var_type);
	Value* v = new GlobalVariable(*M, t, false, GlobalValue::LinkageTypes::ExternalLinkage, NULL, name);
	return LValue(new llvm_value(v));

}

LValue llcg_llvm::DefGlobalVar(LValue var_type, string& name, LValue init) {
	Type* t = *LLTYPE(var_type);
	Value* v = *LLVALUE(init);
	Value* g = new GlobalVariable(*M, t, false, GlobalValue::LinkageTypes::ExternalLinkage, 
									dyn_cast<Constant>(v), name);
	return LValue(new llvm_value(g));
}

LValue llcg_llvm::Load(LValue var_addr) {
	Value* ptr = *LLVALUE(var_addr);
	Value* v = new LoadInst(ptr, "", false, nowBlock);		
	return LValue(new llvm_value(v));
}

LValue llcg_llvm::Store(LValue var_addr, LValue value) {
	Value* addr = *LLVALUE(var_addr);
	Value* v = *LLVALUE(value);
	new StoreInst(v, addr, false, nowBlock);
	return value;
}

LValue llcg_llvm::Opt1(string& opt, LValue value) {
	Value* ans = *LLVALUE(value);

	AtomicRMWInst::BinOp bop; 
	Value* one = ConstantInt::get(Type::getInt64Ty(context), 1); 
	Value* ret;
	if (opt == "~") { ret = BinaryOperator::CreateNot(ans, "", nowBlock); 
						return LValue(new llvm_value(ret));  }
	if (opt == "++") { bop = AtomicRMWInst::BinOp::Add;  goto selfWork; }
	if (opt == "--") { bop = AtomicRMWInst::BinOp::Sub;  goto selfWork; }
	if (opt == "b++") { bop = AtomicRMWInst::BinOp::Add; goto saveWork; }
	if (opt == "b--") { bop = AtomicRMWInst::BinOp::Sub; goto saveWork; }

selfWork:
	new AtomicRMWInst(bop, ans, one, AtomicOrdering::SequentiallyConsistent, 
		SynchronizationScope::CrossThread, nowBlock);
	ret = new LoadInst(ans, "", false, nowBlock);	
	return LValue(new llvm_value(ret));

saveWork:
	ret = new AtomicRMWInst(bop, ans, one, AtomicOrdering::SequentiallyConsistent, 
		SynchronizationScope::CrossThread, nowBlock);
	return LValue(new llvm_value(ret));
}

static Value* getCast(Value* v, Type* t, BasicBlock* bb) {
	Instruction::CastOps cops = CastInst::getCastOpcode(v, true, t, true);
	CastInst::Create(cops, v, t, "", bb);
}

static void normalize_type(Value*& v1, Value*& v2, BasicBlock* bb) {
	Type* t1 = v1->getType();
	Type* t2 = v2->getType();
	if (t1->isDoubleTy() || t2->isDoubleTy()) {
		if (!t1->isDoubleTy()) v1 = getCast(v1, t2, bb);
		if (!t2->isDoubleTy()) v2 = getCast(v2, t1, bb);
		return;
	}
	if (t1->isFloatTy() || t2->isFloatTy()) {
		if (!t1->isFloatTy()) v1 = getCast(v1, t2, bb);
		if (!t2->isFloatTy()) v2 = getCast(v2, t1, bb);
		return;
	}
}


LValue llcg_llvm::Opt2(string& opt, LValue value1, LValue value2) {
	Value* ans1 = *LLVALUE(value1);
	Value* ans2 = *LLVALUE(value2);
	Type* t1 = ans1->getType();
	Type* t2 = ans2->getType();
	Instruction::BinaryOps instr;
	if (t1->isIntegerTy() && t2->isIntegerTy() && t1->getIntegerBitWidth()==t2->getIntegerBitWidth() ) {
		if (opt == "+") { instr = Instruction::Add;  goto binOper; }
		if (opt == "-") { instr = Instruction::Sub;  goto binOper; }
		if (opt == "*") { instr = Instruction::Mul;  goto binOper; }
		if (opt == "/") { instr = Instruction::SDiv; goto binOper; }
		if (opt == "<<") { instr = Instruction::Shl; goto binOper; }
		if (opt == ">>") { instr = Instruction::LShr; goto binOper; } // 注意，这里没处理有符号数和无符号数的问题 AShr(arithmetic)
		if (opt == "&") { instr = Instruction::And; goto binOper; }
		if (opt == "|") { instr = Instruction::Or; goto binOper; }
		if (opt == "^") { instr = Instruction::Xor; goto binOper; }
	} else {
		normalize_type(ans1, ans2, nowBlock);
		if (opt == "+") { instr = Instruction::FAdd;  goto binOper; }
		if (opt == "-") { instr = Instruction::FSub;  goto binOper; }
		if (opt == "*") { instr = Instruction::FMul;  goto binOper; }
		if (opt == "/") { instr = Instruction::FDiv; goto binOper; }
	}

binOper:
	Value* ret = BinaryOperator::Create(instr, ans1, ans2, "", nowBlock);
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Cmp(string& opt, LValue value1, LValue value2) {
	Value* ans1 = *LLVALUE(value1);
	Value* ans2 = *LLVALUE(value2);
	Type* t1 = ans1->getType();
	Type* t2 = ans2->getType();
	CmpInst::Predicate instp;
	if (t1->isIntegerTy() && t2->isIntegerTy() && t1->getIntegerBitWidth()==t2->getIntegerBitWidth() ) {
		if (opt == "==") { instp = CmpInst::Predicate::ICMP_EQ;  goto cmpOper; } 
		if (opt == "!=") { instp = CmpInst::Predicate::ICMP_NE;  goto cmpOper; } 
		if (opt == "<=") { instp = CmpInst::Predicate::ICMP_SLE; goto cmpOper; } 
		if (opt == ">=") { instp = CmpInst::Predicate::ICMP_SGE; goto cmpOper; } 
		if (opt == "<")  { instp = CmpInst::Predicate::ICMP_SLT; goto cmpOper; } 
		if (opt == ">")  { instp = CmpInst::Predicate::ICMP_SGT; goto cmpOper; } 
	} else {
		normalize_type(ans1, ans2, nowBlock);
		if (opt == "==") { instp = CmpInst::Predicate::FCMP_OEQ;  goto cmpOper; } 
		if (opt == "!=") { instp = CmpInst::Predicate::FCMP_ONE;  goto cmpOper; } 
		if (opt == "<=") { instp = CmpInst::Predicate::FCMP_OLE; goto cmpOper; } 
		if (opt == ">=") { instp = CmpInst::Predicate::FCMP_OGE; goto cmpOper; } 
		if (opt == "<")  { instp = CmpInst::Predicate::FCMP_OLT; goto cmpOper; } 
		if (opt == ">")  { instp = CmpInst::Predicate::FCMP_OGT; goto cmpOper; }
	}

cmpOper:
	Value* ret = CmpInst::Create(Instruction::ICmp, instp, ans1, ans2, "", nowBlock);
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Assignment(string& opt, LValue value1, LValue value2) {
	Value* ans1 = *LLVALUE(value1);
	Value* ans2 = *LLVALUE(value2);
	if (opt == "=") {
		Value* ret = new StoreInst(ans2, ans1, false, nowBlock);
		return LValue(new llvm_value(ret));
	}
	AtomicRMWInst::BinOp bop; 
	if (opt == "+=") { bop = AtomicRMWInst::BinOp::Add; goto rmwOper; }
	if (opt == "-=") { bop = AtomicRMWInst::BinOp::Sub; goto rmwOper; }
	if (opt == "&=") { bop = AtomicRMWInst::BinOp::And; goto rmwOper; }
	if (opt == "|=") { bop = AtomicRMWInst::BinOp::Or;  goto rmwOper; }
	if (opt == "^=") { bop = AtomicRMWInst::BinOp::Xor; goto rmwOper; }
rmwOper:
	Value* ret = new AtomicRMWInst(bop, ans1, ans2, AtomicOrdering::SequentiallyConsistent, 
		SynchronizationScope::CrossThread, nowBlock);
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Dot(LValue value, int num) {
	Value* ans = *LLVALUE(value);
	ConstantInt* zero = ConstantInt::get(Type::getInt32Ty(context), 0);
	ConstantInt* n    = ConstantInt::get(Type::getInt32Ty(context), num);
	std::vector<Value*> indices;
	indices.push_back(zero); 
	indices.push_back(n);

	GetElementPtrInst* ptr = GetElementPtrInst::Create(ans, indices, "", nowBlock);
	return LValue(new llvm_value(ptr));
}

LValue llcg_llvm::Select(LValue value, vector<LValue>& args) {
	Value* v = *LLVALUE(value);
	Value* len_array;
	if (v->getType()->getPointerElementType()->isIntegerTy(64))
		len_array = v;
	else 
		len_array = CastInst::CreatePointerCast(v, Type::getInt64PtrTy(context));
	Value* index;
	if (args.size() != 0) {
		index = *LLVALUE(args[0]);
		for (int i = -2, j = 0; j < args.size()-1; --i, ++j) {
			Value* len = new LoadInst(ptrMove(len_array, i), "", false, nowBlock);
			index = BinaryOperator::Create(Instruction::Mul, index, len, "", nowBlock);
			Value* other = *LLVALUE(args[j+1]);	
			index = BinaryOperator::Create(Instruction::Add, index, other, "", nowBlock);
		}
	} else { errs() << "error index\n"; }

	std::vector<Value*> indices;
	indices.push_back(index);
	GetElementPtrInst* ptr = GetElementPtrInst::Create(v, indices, "", nowBlock);
	return LValue(new llvm_value(ptr));
}

GetElementPtrInst* llcg_llvm::ptrMove(Value* v, int n) {
	std::vector<Value*> indices;
	ConstantInt* num = ConstantInt::get(Type::getInt64Ty(context), n);
	indices.push_back(num);
	GetElementPtrInst* p1 = GetElementPtrInst::Create(v, indices, "", nowBlock);
}


void llcg_llvm::If(LValue cond, LValue father, LValue true_block, LValue false_block, bool isElseWork) {
	Value* condition    = *LLVALUE(cond);
	Value* _father      = *LLVALUE(father);
	Value* _true_block  = *LLVALUE(true_block);
	Value* _false_block = *LLVALUE(false_block);
	BasicBlock* father_block  = dyn_cast<BasicBlock>(_father);
	BasicBlock* _true_block_  = dyn_cast<BasicBlock>(_true_block);
	BasicBlock* _false_block_ = dyn_cast<BasicBlock>(_false_block);

	if (isElseWork) {
		BasicBlock* end_block = createBlock();
		BranchInst::Create(end_block, _true_block_);
		BranchInst::Create(end_block, _false_block_);
	} else {
		BranchInst::Create(_false_block_, _true_block_);
	}
	BranchInst* branch = BranchInst::Create(_true_block_, _false_block_, condition, father_block);
}

void llcg_llvm::For(LValue cond, LValue init, LValue pd, LValue work, LValue statement) {
	Value* condition  = *LLVALUE(cond);
	Value* _init      = *LLVALUE(init);
	Value* _pd        = *LLVALUE(pd);
	Value* _work      = *LLVALUE(work);
	Value* _statement = *LLVALUE(statement);
	BasicBlock* init_block = dyn_cast<BasicBlock>(_init);
	BasicBlock* end_block  = dyn_cast<BasicBlock>(_pd);
	BasicBlock* do_block   = dyn_cast<BasicBlock>(_work);
	BasicBlock* work_block = dyn_cast<BasicBlock>(_statement);
	
	BasicBlock* false_block = createBlock();
	BranchInst* branch = BranchInst::Create(work_block, false_block, condition, end_block);
	BranchInst::Create(end_block, init_block);
	BranchInst::Create(do_block,  work_block);
	BranchInst::Create(end_block, do_block);
}

void llcg_llvm::While(LValue cond, LValue father, LValue pd, LValue statement) {
	Value* condition  = *LLVALUE(cond);
	Value* _father    = *LLVALUE(father);
	Value* _pd        = *LLVALUE(pd);
	Value* _statement = *LLVALUE(statement);

	BasicBlock* father_block = dyn_cast<BasicBlock>(_father);
	BasicBlock* pd_block     = dyn_cast<BasicBlock>(_pd);
	BasicBlock* true_block   = dyn_cast<BasicBlock>(_statement);

	// 生成while循环
	BasicBlock* false_block = createBlock();
	BranchInst* branch = BranchInst::Create(true_block, false_block, condition, pd_block);
	BranchInst::Create(pd_block, father_block);
	BranchInst::Create(pd_block, true_block);
}

void llcg_llvm::DoWhile(LValue statement, LValue pd) {

}

void llcg_llvm::DoUntil(LValue statement, LValue pd) {

}

LValue llcg_llvm::New(LValue var_type, vector<LValue>& args) {
	Type* t = *LLTYPE(var_type);
	Type* ITy = Type::getInt64Ty(context);
	Constant* AllocSize = ConstantExpr::getSizeOf(t);
	Instruction* Malloc = CallInst::CreateMalloc(nowBlock, ITy, t, AllocSize);
	Malloc->insertAfter(&(nowBlock->back()));
	return LValue(new llvm_value(Malloc));
}

LValue llcg_llvm::NewArray(LValue var_type, vector<LValue>& wd) {
	// 这里实现自定义的数组malloc函数
	Type* t = *LLTYPE(var_type);
	ConstantInt* zero = ConstantInt::get(Type::getInt64Ty(context), 0);
	vector<Value*> args;
	for (auto p : wd) {
		Value* v = *LLVALUE(p);
		args.push_back(v);
	}
	args.push_back(zero);
	string func_name = "malloc_array";
	CallInst *call = CallInst::Create(M->getFunction(func_name), 
		args, "", nowBlock);
	// t = ArrayType::get(t, 0);
	t = t->getPointerTo();
	Value* ret = CastInst::CreatePointerCast(call, t, "", nowBlock);
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Return() {
	Value* ret = ReturnInst::Create(context, nullptr, nowBlock);
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Return(LValue var) {
	Value* v = *LLVALUE(var);
	Value* ret = ReturnInst::Create(context, v, nowBlock);
	return LValue(new llvm_value(ret));
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


Constant* llcg_llvm::geti8StrVal(Module& M, char const* str, Twine const& name) {
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
	register_stdlib(M.get(), stdlibs);
}

void llcg_llvm::register_stdlib(Module* M, const LibFunc* libs_func) {
	while (*libs_func != NULL) {
		(*libs_func)(M);
		++libs_func;
	}
}

extern const LibFunc metalibs[];

void llcg_llvm::register_metalib() {
	register_stdlib(meta_M.get(), metalibs);
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
	   // 输出编译后的LLVM可读字节码
    outs() << "LLVM module:\n\n" << *M;
    outs() << "\n\n";
    outs().flush();

    // 校验问题, 这个函数需要一个输出流来打印错误信息
    if (verifyModule(*M, &errs())) {
        errs() << "构建LLVM字节码出错!\n";
        exit(1);
    }
 
    // 输出二进制BitCode到.bc文件
    std::error_code ErrInfo;
    raw_ostream *out = new raw_fd_ostream(outfile_name.c_str(), ErrInfo, sys::fs::F_None);
    WriteBitcodeToFile(M, *out);
    out->flush(); delete out;
}

void llcg_llvm::MakeMetaModule(string& outfile_name, string& module_name) {
	Module* M = meta_M.get();
    M->setModuleIdentifier(module_name);

    Function *F = M->getFunction("elite_meta_init");
    BasicBlock& bb = F->getEntryBlock(); // 仅有一个BasicBlock,所以这样写可以
    if (bb.getTerminator() == NULL) // 处理函数结尾返回
        ReturnInst::Create(context, &bb);
    Type* function_ptr = F->getFunctionType()->getPointerTo(); // 获取函数指针类型

    vector<Type*> type_list;
    type_list.push_back(Type::getInt32Ty(M->getContext()));
    type_list.push_back(function_ptr);
    type_list.push_back(Type::getInt8PtrTy(M->getContext()));

    vector<Constant*> init_list;
    init_list.push_back(ConstantInt::get(Type::getInt32Ty(M->getContext()), 65535));
    init_list.push_back(ConstantExpr::getBitCast(F, function_ptr));
    init_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));

    StructType* st = StructType::create(type_list, "");
    ArrayType* at = ArrayType::get(st, 1);
    vector<Constant*> array_init_list;
    array_init_list.push_back(ConstantStruct::get(st, init_list));
    Constant* init_var = ConstantArray::get(at, array_init_list);

    GlobalVariable* gv = new GlobalVariable(
        *M, at, false, GlobalValue::AppendingLinkage, init_var, "llvm.global_ctors"
    );

    verifyModuleAndWrite(M, outfile_name);
}



BasicBlock* llcg_llvm::createBlock() {
	return nowBlock = BasicBlock::Create(context, "", nowFunc);
}

BasicBlock* llcg_llvm::createBlock(Function* f) {
	nowFunc = f;
	return nowBlock = BasicBlock::Create(context, "entry", f);
}

void llcg_llvm::MakeMetaList(vector<string>& list) {
	Module* M = meta_M.get();
	Function* F = M->getFunction("elite_meta_init");
	vector<Value*> args_list;
	for (int i = 0; i < list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, list[i].c_str(), ""));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_list");
	CallInst::Create(FuncF, args_list, "", &bb);
}

void llcg_llvm::MakeMetaList(string& name, vector<string>& list, LValue fp) {
	Module* M = meta_M.get();
	Function* F = M->getFunction("elite_meta_init");
	vector<Value*> args_list;
	for (int i = 0; i < list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, list[i].c_str(), ""));
		// Constant* c = init_list[i];
		// if (c != NULL)
		// 	init_meta_list.push_back(ConstantAsMetadata::get(c));
		// else
		// 	init_meta_list.push_back(MDString::get(M->getContext(), "NULL"));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));
	Type* ft = *LLTYPE(fp);
	Function* nowFunc = dyn_cast<Function>(M->getOrInsertFunction(name, (FunctionType*)ft));
	Type* nowType = Type::getInt8PtrTy(M->getContext());
	args_list.push_back(ConstantExpr::getBitCast(nowFunc, nowType));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_function");
	CallInst::Create(FuncF, args_list, "", &bb);
}