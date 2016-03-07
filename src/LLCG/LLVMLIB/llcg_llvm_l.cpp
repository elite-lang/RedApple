/*
* @Author: sxf
* @Date:   2015-12-24 17:54:40
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 17:17:52
*/

#include "llcg_llvm.h"

#include "llvm_value.h"
#include "llvm_type.h"

lvalue* llcg_llvm::l_FuncType(FunctionModel* fmodel) {
	return nullptr;
} // 返回FunctionType

lvalue* llcg_llvm::l_FuncType(lvalue* ret_type, vector<lvalue*>& types, bool isNotSure) {
	llvm_type* ret = dynamic_cast<llvm_type*>(ret_type);
	vector<Type*> arg_types;
	for (auto p : types) {
		llvm_type* t = dynamic_cast<llvm_type*>(p);
		arg_types.push_back(*t);
	}
	FunctionType* func_type = FunctionType::get(*ret, arg_types,
		/*vararg*/isNotSure);
	return new llvm_type(func_type);
}

lvalue* llcg_llvm::l_GetOrInsertFunction(FunctionModel* fmodel) {
	return nullptr;
} // 返回Function

lvalue* llcg_llvm::l_GetOrInsertFunction(const string& name, lvalue* func_type) {
	llvm_type* _func_type = dynamic_cast<llvm_type*>(func_type);
	Type* t = *_func_type;
	Constant* f = M->getOrInsertFunction(name, (FunctionType*)t);
	return new llvm_value(f);
}

lvalue* llcg_llvm::l_GetOrInsertFunction(const string& name, lvalue* ret_type, vector<lvalue*>& types, bool isNotSure) {
	return nullptr;
}

void   llcg_llvm::l_FunctionBodyBegin(lvalue* func, vector<string>& name_list) {
	Value* _func = *dynamic_cast<llvm_value*>(func);
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

void   llcg_llvm::l_FunctionBodyEnd() {
	if (nowBlock->getTerminator() == NULL)
		ReturnInst::Create(context, nowBlock);
} // 处理函数结束

lvalue* llcg_llvm::l_getFunction(const string& name) {
	Function* f = M->getFunction(name);
	return new llvm_value(f);
} // 从当前模块中获取一个函数

lvalue* llcg_llvm::l_Call(FunctionModel* fmodel, vector<lvalue*>& args) {
	return nullptr;
} // 返回CallInst

lvalue* llcg_llvm::l_Call(lvalue* func, vector<lvalue*>& args) {
	llvm_value* _func = dynamic_cast<llvm_value*>(func);
	vector<Value*> fargs;
	for (auto p : args) {
		llvm_value* t = dynamic_cast<llvm_value*>(p);
		fargs.push_back(*t);
	}

	CallInst *call = CallInst::Create(*_func, fargs, "", nowBlock);
	return new llvm_value(call);
}

lvalue* llcg_llvm::l_Struct(StructModel* smodel) {
	return nullptr;
} // 返回StructType

lvalue* llcg_llvm::l_Struct(lvalue* _struct, vector<lvalue*>& types) {
	std::vector<Type*> type_vec;
	for (auto p : types) {
		Type* t = *dynamic_cast<llvm_type*>(p);
		type_vec.push_back(t);
	}
	Type* struct_type = *dynamic_cast<llvm_type*>(_struct);
	StructType* st = dyn_cast<StructType>(struct_type);
	st->setBody(type_vec);
	return new llvm_type(st);
}

lvalue* llcg_llvm::l_DeclareStruct(const string& name) {
	StructType* st = StructType::create(context, name);
	return new llvm_type(st);
}

lvalue* llcg_llvm::l_DefVar(lvalue* var_type, const string& name) {
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	return new llvm_value(alloc);
} // 返回分配的地址

lvalue* llcg_llvm::l_DefVar(lvalue* var_type, const string& name, lvalue* init) {
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	Value* v = *dynamic_cast<llvm_value*>(init);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	new StoreInst(v, alloc, false, nowBlock);
	return new llvm_value(alloc);
}

lvalue* llcg_llvm::l_DefGlobalVar(lvalue* var_type, const string& name) {
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	Value* v = new GlobalVariable(*M, t, false, GlobalValue::LinkageTypes::ExternalLinkage, NULL, name);
	return new llvm_value(v);
}

lvalue* llcg_llvm::l_DefGlobalVar(lvalue* var_type, const string& name, lvalue* init) {
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	Value* v = *dynamic_cast<llvm_value*>(init);
	Value* g = new GlobalVariable(*M, t, false, GlobalValue::LinkageTypes::ExternalLinkage,
									dyn_cast<Constant>(v), name);
	return new llvm_value(g);
}

lvalue* llcg_llvm::l_Load(lvalue* var_addr) {
	Value* ptr = *dynamic_cast<llvm_value*>(var_addr);
	Value* v = new LoadInst(ptr, "", false, nowBlock);
	return new llvm_value(v);
}

lvalue* llcg_llvm::l_Store(lvalue* var_addr, lvalue* value) {
	Value* addr = *dynamic_cast<llvm_value*>(var_addr);
	Value* v = *dynamic_cast<llvm_value*>(value);
	new StoreInst(v, addr, false, nowBlock);
	return value;
}

lvalue* llcg_llvm::l_Opt1(const string& opt, lvalue* value) {
	Value* ans = *dynamic_cast<llvm_value*>(value);

	AtomicRMWInst::BinOp bop;
	Value* one = ConstantInt::get(Type::getInt64Ty(context), 1);
	Value* ret;
	if (opt == "-") { ret = BinaryOperator::CreateNeg(ans, "", nowBlock);
						return new llvm_value(ret);  }
	if (opt == "~") { ret = BinaryOperator::CreateNot(ans, "", nowBlock);
						return new llvm_value(ret);  }
	if (opt == "++") { bop = AtomicRMWInst::BinOp::Add;  goto selfWork; }
	if (opt == "--") { bop = AtomicRMWInst::BinOp::Sub;  goto selfWork; }
	if (opt == "b++") { bop = AtomicRMWInst::BinOp::Add; goto saveWork; }
	if (opt == "b--") { bop = AtomicRMWInst::BinOp::Sub; goto saveWork; }

selfWork:
	new AtomicRMWInst(bop, ans, one, AtomicOrdering::SequentiallyConsistent,
		SynchronizationScope::CrossThread, nowBlock);
	ret = new LoadInst(ans, "", false, nowBlock);
	return new llvm_value(ret);

saveWork:
	ret = new AtomicRMWInst(bop, ans, one, AtomicOrdering::SequentiallyConsistent,
		SynchronizationScope::CrossThread, nowBlock);
	return new llvm_value(ret);
}


static Value* getCast(Value* v, Type* t, BasicBlock* bb) {
	Instruction::CastOps cops = CastInst::getCastOpcode(v, true, t, true);
	return CastInst::Create(cops, v, t, "", bb);
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


lvalue* llcg_llvm::l_Opt2(const string& opt, lvalue* value1, lvalue* value2) {
	Value* ans1 = *dynamic_cast<llvm_value*>(value1);
	Value* ans2 = *dynamic_cast<llvm_value*>(value2);
	Type* t1 = ans1->getType();
	Type* t2 = ans2->getType();
	Instruction::BinaryOps instr;
	if (t1->isIntegerTy() && t2->isIntegerTy() && t1->getIntegerBitWidth()==t2->getIntegerBitWidth() ) {
		if (opt == "+") { instr = Instruction::Add;  goto binOper; }
		if (opt == "-") { instr = Instruction::Sub;  goto binOper; }
		if (opt == "*") { instr = Instruction::Mul;  goto binOper; }
		if (opt == "/") { instr = Instruction::SDiv; goto binOper; }
		if (opt == "%") { instr = Instruction::SRem; goto binOper; }
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
		if (opt == "%") { instr = Instruction::FRem; goto binOper; }
	}

binOper:
	Value* ret = BinaryOperator::Create(instr, ans1, ans2, "", nowBlock);
	return new llvm_value(ret);
}

lvalue* llcg_llvm::l_Cmp(const string& opt, lvalue* value1, lvalue* value2) {
	Value* ans1 = *dynamic_cast<llvm_value*>(value1);
	Value* ans2 = *dynamic_cast<llvm_value*>(value2);
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
	return new llvm_value(ret);
}

lvalue* llcg_llvm::l_Assignment(const string& opt, lvalue* value1, lvalue* value2) {
	Value* ans1 = *dynamic_cast<llvm_value*>(value1);
	Value* ans2 = *dynamic_cast<llvm_value*>(value2);
	if (opt == "=") {
		Value* ret = new StoreInst(ans2, ans1, false, nowBlock);
		return new llvm_value(ret);
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
	return new llvm_value(ret);
}

lvalue* llcg_llvm::l_Dot(lvalue* value, int num) {
	Value* ans = *dynamic_cast<llvm_value*>(value);
	ConstantInt* zero = ConstantInt::get(Type::getInt32Ty(context), 0);
	ConstantInt* n    = ConstantInt::get(Type::getInt32Ty(context), num);
	std::vector<Value*> indices;
	indices.push_back(zero);
	indices.push_back(n);
	GetElementPtrInst* ptr = GetElementPtrInst::CreateInBounds(ans, indices, "", nowBlock);
	return new llvm_value(ptr);
}

lvalue* llcg_llvm::l_Select(lvalue* value, vector<lvalue*>& args) {
	Value* v = *dynamic_cast<llvm_value*>(value);
	Value* len_array;
	errs() << "type:" << *(v->getType());
	if (!(v->getType()->isPointerTy())) return nullptr;
	if (v->getType()->getPointerElementType()->isIntegerTy(64))
		len_array = v;
	else
		len_array = CastInst::CreatePointerCast(v, Type::getInt64PtrTy(context));
	Value* index;
	if (args.size() != 0) {
		index = *dynamic_cast<llvm_value*>(args[0]);
		for (int i = -2, j = 0; j < args.size()-1; --i, ++j) {
			Value* len = new LoadInst(ptrMove(len_array, i), "", false, nowBlock);
			index = BinaryOperator::Create(Instruction::Mul, index, len, "", nowBlock);
			Value* other = *dynamic_cast<llvm_value*>(args[j+1]);
			index = BinaryOperator::Create(Instruction::Add, index, other, "", nowBlock);
		}
	} else { errs() << "error index\n"; }

	std::vector<Value*> indices;
	indices.push_back(index);
	GetElementPtrInst* ptr = GetElementPtrInst::CreateInBounds(v, indices, "", nowBlock);
	return new llvm_value(ptr);
}

void   llcg_llvm::l_If(lvalue* cond, lvalue* father, lvalue* true_block, lvalue* true_block_end,
						lvalue* false_block, lvalue* false_block_end, bool isElseWork) {
	Value* condition    = *dynamic_cast<llvm_value*>(cond);
	Value* _father      = *dynamic_cast<llvm_value*>(father);
	Value* _true_block  = *dynamic_cast<llvm_value*>(true_block);
	Value* _false_block = *dynamic_cast<llvm_value*>(false_block);
	BasicBlock* father_block  = dyn_cast<BasicBlock>(_father);
	BasicBlock* _true_block_  = dyn_cast<BasicBlock>(_true_block);
	BasicBlock* _false_block_ = dyn_cast<BasicBlock>(_false_block);

	if (isElseWork) {
		BasicBlock* end_block = createBlock();
		if (_true_block_->getTerminator() == NULL)
			BranchInst::Create(end_block, _true_block_);
		if (_false_block_->getTerminator() == NULL)
			BranchInst::Create(end_block, _false_block_);
	} else {
		if (_true_block_->getTerminator() == NULL)
			BranchInst::Create(_false_block_, _true_block_);
	}
	BranchInst* branch = BranchInst::Create(_true_block_, _false_block_, condition, father_block);
}

void   llcg_llvm::l_For(lvalue* cond, lvalue* init, lvalue* pd, lvalue* work, lvalue* statement, lvalue* statement_end, lvalue* end) {
	Value* condition  = *dynamic_cast<llvm_value*>(cond);
	Value* _init      = *dynamic_cast<llvm_value*>(init);
	Value* _pd        = *dynamic_cast<llvm_value*>(pd);
	Value* _work      = *dynamic_cast<llvm_value*>(work);
	Value* _statement = *dynamic_cast<llvm_value*>(statement);
	Value* _end       = *dynamic_cast<llvm_value*>(end);
	BasicBlock* init_block = dyn_cast<BasicBlock>(_init);
	BasicBlock* end_block  = dyn_cast<BasicBlock>(_pd);
	BasicBlock* do_block   = dyn_cast<BasicBlock>(_work);
	BasicBlock* work_block = dyn_cast<BasicBlock>(_statement);
	BasicBlock* false_block = dyn_cast<BasicBlock>(_end);

	BranchInst* branch = BranchInst::Create(work_block, false_block, condition, end_block);
	if (init_block->getTerminator() == NULL)
		BranchInst::Create(end_block, init_block);
	if (work_block->getTerminator() == NULL)
		BranchInst::Create(do_block,  work_block);
	if (do_block->getTerminator() == NULL)
		BranchInst::Create(end_block, do_block);
}

void   llcg_llvm::l_While(lvalue* cond, lvalue* father, lvalue* pd, lvalue* statement, lvalue* statement_end, lvalue* end) {
	Value* condition  = *dynamic_cast<llvm_value*>(cond);
	Value* _father    = *dynamic_cast<llvm_value*>(father);
	Value* _pd        = *dynamic_cast<llvm_value*>(pd);
	Value* _statement = *dynamic_cast<llvm_value*>(statement);
	Value* _end       = *dynamic_cast<llvm_value*>(end);

	BasicBlock* father_block = dyn_cast<BasicBlock>(_father);
	BasicBlock* pd_block     = dyn_cast<BasicBlock>(_pd);
	BasicBlock* true_block   = dyn_cast<BasicBlock>(_statement);
	BasicBlock* false_block  = dyn_cast<BasicBlock>(_end);

	// 生成while循环
	BranchInst* branch = BranchInst::Create(true_block, false_block, condition, pd_block);
	if (father_block->getTerminator() == NULL)
		BranchInst::Create(pd_block, father_block);
	if (true_block->getTerminator() == NULL)
		BranchInst::Create(pd_block, true_block);
}

void   llcg_llvm::l_DoWhile(lvalue* statement, lvalue* pd) {

}

void   llcg_llvm::l_DoUntil(lvalue* statement, lvalue* pd) {

}

lvalue* llcg_llvm::l_New(lvalue* var_type, vector<lvalue*>& args, const string& funcname) {
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	Type* ITy = Type::getInt64Ty(context);
	Constant* AllocSize = ConstantExpr::getSizeOf(t);
	Instruction* Malloc = CallInst::CreateMalloc(nowBlock, ITy, t, AllocSize);
	Malloc->insertAfter(&(nowBlock->back()));
	return new llvm_value(Malloc);
}

lvalue* llcg_llvm::l_NewArray(lvalue* var_type, vector<lvalue*>& wd, const string& funcname) {
	// 这里实现自定义的数组malloc函数
	Type* t = *dynamic_cast<llvm_type*>(var_type);
	ConstantInt* zero = ConstantInt::get(Type::getInt64Ty(context), 0);
	vector<Value*> args;
	for (auto p : wd) {
		Value* v = *dynamic_cast<llvm_value*>(p);
		args.push_back(v);
	}
	args.push_back(zero);
	string func_name = "malloc_array";
	CallInst *call = CallInst::Create(M->getFunction(func_name),
		args, "", nowBlock);
	// t = ArrayType::get(t, 0);
	t = t->getPointerTo();
	Value* ret = CastInst::CreatePointerCast(call, t, "", nowBlock);
	return new llvm_value(ret);
}

lvalue* llcg_llvm::l_Delete(lvalue* pointer, const string& funcname) {
	Value* ptr  = *dynamic_cast<llvm_value*>(pointer);
	Instruction* Free = CallInst::CreateFree(ptr, nowBlock);
	return new llvm_value(Free);
}

lvalue* llcg_llvm::l_DeleteArray(lvalue* pointer, const string& funcname) {
	return NULL;
}



lvalue* llcg_llvm::l_Return() {
	Value* ret = ReturnInst::Create(context, nullptr, nowBlock);
	return new llvm_value(ret);
}

lvalue* llcg_llvm::l_Return(lvalue* var) {
	Value* v = *dynamic_cast<llvm_value*>(var);
	Value* ret = ReturnInst::Create(context, v, nowBlock);
	return new llvm_value(ret);
}


lvalue* llcg_llvm::l_Int8() {
	return new llvm_type(Type::getInt8Ty(context));
}

lvalue* llcg_llvm::l_Int16() {
	return new llvm_type(Type::getInt16Ty(context));

}

lvalue* llcg_llvm::l_Int32() {
	return new llvm_type(Type::getInt32Ty(context));

}

lvalue* llcg_llvm::l_Int64() {
	return new llvm_type(Type::getInt64Ty(context));

}

lvalue* llcg_llvm::l_Float() {
	return new llvm_type(Type::getFloatTy(context));

}

lvalue* llcg_llvm::l_Double() {
	return new llvm_type(Type::getDoubleTy(context));

}

lvalue* llcg_llvm::l_Void() {
	return new llvm_type(Type::getVoidTy(context));

}

lvalue* llcg_llvm::l_ConstString(const string& str) {
	return new llvm_value(geti8StrVal(*M, str.c_str(), ""));

}

lvalue* llcg_llvm::l_ConstInt(int num) {
	Type* t = Type::getInt64Ty(context);
	return new llvm_value(ConstantInt::get(t, num));
}

lvalue* llcg_llvm::l_ConstDouble(double num) {
	Type* t = Type::getDoubleTy(context);
	return new llvm_value(ConstantFP::get(t, num));
}

extern const LibFunc stdlibs[];

void   llcg_llvm::l_BeginModule(const string& name) {
	M = llvm::make_unique<Module>(name, context);
	register_stdlib(M.get(), stdlibs);
}

void   llcg_llvm::l_VerifyAndWrite(const string& outfile_name) {
	verifyModuleAndWrite(M.get(), outfile_name);

}

void   llcg_llvm::l_MakeMetaModule(const string& outfile_name, const string& module_name) {
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
        *M, at, false, GlobalValue::AppendingLinkage, init_var, "llvm.globallcg_llvm::l_ctors"
    );

    verifyModuleAndWrite(M, outfile_name);
}


lvalue* llcg_llvm::l_GetNowBasicBlock() {
	return new llvm_value(nowBlock);

}

lvalue* llcg_llvm::l_CreateBasicBlock() {
	return new llvm_value(createBlock());

}

lvalue* llcg_llvm::l_CreateBasicBlock(lvalue* func) {
	Value* v = *dynamic_cast<llvm_value*>(func);
	Function* f = dyn_cast<Function>(v);
	return new llvm_value(createBlock(f));
}


void   llcg_llvm::l_MakeMetaList(vector<string>& list) {
	Module* M = meta_M.get();
	Function* F = M->getFunction("elite_meta_init");
	vector<Constant*> args_list;
	for (int i = 0; i < list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, list[i].c_str(), ""));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_list");
	Constant* list_vec = getPtrArray(*M, args_list);
	vector<Value*> args;
	args.push_back(list_vec);
	CallInst::Create(FuncF, args, "", &bb);
}

void   llcg_llvm::l_MakeMetaList(const string& name, vector<string>& list, lvalue* fp) {
	Module* M = meta_M.get();
	Function* F = M->getFunction("elite_meta_init");
	vector<Constant*> args_list;
	for (int i = 0; i < list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, list[i].c_str(), ""));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));

	Constant* list_vec = getPtrArray(*M, args_list);
	vector<Value*> args;
	args.push_back(geti8StrVal(*M, name.c_str(), "")); // 第一个参数, 函数名
	args.push_back(list_vec); // 第二个参数, 函数类型定义字符串列表

	Type* ft = *dynamic_cast<llvm_type*>(fp);
	Function* nowFunc = dyn_cast<Function>(M->getOrInsertFunction(name, (FunctionType*)ft));
	Type* nowType = Type::getInt8PtrTy(M->getContext());
	args.push_back(ConstantExpr::getBitCast(nowFunc, nowType));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_function");
	CallInst::Create(FuncF, args, "", &bb);
}

void llcg_llvm::l_CloseTerminator(lvalue* basicblock, lvalue* target) {
	Value* _basicblock = *dynamic_cast<llvm_value*>(basicblock);
	Value* _target     = *dynamic_cast<llvm_value*>(target);
	BasicBlock* bb     = dyn_cast<BasicBlock>(_basicblock);
	BasicBlock* tg     = dyn_cast<BasicBlock>(_target);
	if (bb->getTerminator() == NULL)
		BranchInst::Create(tg, bb);
}
