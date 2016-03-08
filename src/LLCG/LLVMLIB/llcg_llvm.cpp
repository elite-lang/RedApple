/*
* @Author: sxf
* @Date:   2015-11-23 21:41:19
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 17:17:28
*/

#include "llcg_llvm.h"
#include "llvm_value.h"
#include "llvm_type.h"

#include <iostream>
#include <fstream>
using namespace std;
llcg_llvm::llcg_llvm() {
	meta_M = llvm::make_unique<Module>("", context);
	register_metalib();
}

llcg_llvm::~llcg_llvm() {
	llvm_shutdown();
}

LValue llcg_llvm::FuncType(FunctionModel* fmodel) {
	return nullptr;
} // 返回FunctionType

LValue llcg_llvm::FuncType(LValue ret_type, vector<LValue>& types, bool isNotSure) {
	LLVMType ret = LLTYPE(ret_type);
	vector<Type*> malloc_type;
	for (auto p : types) {
		LLVMType t = LLTYPE(p);
		malloc_type.push_back(*t);
	}
	FunctionType* func_type = FunctionType::get(*ret, malloc_type,
		/*vararg*/isNotSure);
	return LValue(new llvm_type(func_type));
}

LValue llcg_llvm::GetOrInsertFunction(FunctionModel* fmodel) {
	return nullptr;
} // 返回Function

LValue llcg_llvm::GetOrInsertFunction(const string& name, LValue func_type) {
	LLVMType _func_type = LLTYPE(func_type);
	Type* t = *_func_type;
	Constant* f = M->getOrInsertFunction(name, (FunctionType*)t);
	return LValue(new llvm_value(f));
}

LValue llcg_llvm::GetOrInsertFunction(const string& name, LValue ret_type, vector<LValue>& types, bool isNotSure) {
	return nullptr;
}

void llcg_llvm::FunctionBodyBegin(LValue func, vector<string>& name_list) {
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

LValue llcg_llvm::getFunction(const string& name) {
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
	return nullptr;
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
	return nullptr;
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

LValue llcg_llvm::DeclareStruct(const string& name) {
	StructType* st = StructType::create(context, name);
	return LValue(new llvm_type(st));
}


LValue llcg_llvm::DefVar(LValue var_type, const string& name) {
	Type* t = *LLTYPE(var_type);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	return LValue(new llvm_value(alloc));
}

LValue llcg_llvm::DefVar(LValue var_type, const string& name, LValue init) {
	Type* t = *LLTYPE(var_type);
	Value* v = *LLVALUE(init);
	AllocaInst *alloc = new AllocaInst(t, name, nowBlock);
	new StoreInst(v, alloc, false, nowBlock);
	return LValue(new llvm_value(alloc));
}

LValue llcg_llvm::DefGlobalVar(LValue var_type, const string& name) {
	Type* t = *LLTYPE(var_type);
	Value* v = new GlobalVariable(*M, t, false, GlobalValue::LinkageTypes::ExternalLinkage, NULL, name);
	return LValue(new llvm_value(v));
}

LValue llcg_llvm::DefGlobalVar(LValue var_type, const string& name, LValue init) {
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

LValue llcg_llvm::Opt1(const string& opt, LValue value) {
	Value* ans = *LLVALUE(value);

	AtomicRMWInst::BinOp bop;
	Value* one = ConstantInt::get(Type::getInt64Ty(context), 1);
	Value* ret;
	if (opt == "-") { ret = BinaryOperator::CreateNeg(ans, "", nowBlock);
						return LValue(new llvm_value(ret));  }
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


LValue llcg_llvm::Opt2(const string& opt, LValue value1, LValue value2) {
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
	return LValue(new llvm_value(ret));
}

LValue llcg_llvm::Cmp(const string& opt, LValue value1, LValue value2) {
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

LValue llcg_llvm::Assignment(const string& opt, LValue value1, LValue value2) {
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
	GetElementPtrInst* ptr = GetElementPtrInst::CreateInBounds(ans, indices, "", nowBlock);
	return LValue(new llvm_value(ptr));
}

LValue llcg_llvm::Select(LValue value, vector<LValue>& args) {
	Value* v = *LLVALUE(value);
	Value* len_array;
	errs() << "type:" << *(v->getType());
	if (!(v->getType()->isPointerTy())) return nullptr;
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
	GetElementPtrInst* ptr = GetElementPtrInst::CreateInBounds(v, indices, "", nowBlock);
	return LValue(new llvm_value(ptr));
}

GetElementPtrInst* llcg_llvm::ptrMove(Value* v, int n) {
	std::vector<Value*> indices;
	ConstantInt* num = ConstantInt::get(Type::getInt64Ty(context), n);
	indices.push_back(num);
	GetElementPtrInst* p1 = GetElementPtrInst::CreateInBounds(v, indices, "", nowBlock);
	return p1;
}


void llcg_llvm::If(LValue cond, LValue father, LValue true_block, LValue true_block_end,
				   LValue false_block, LValue false_block_end, bool isElseWork) {

	// 由于动态转换模板的特殊性,需要先转换成Value*类型
	Value* condition              = *LLVALUE(cond);
 	Value* _father                = *LLVALUE(father);
 	Value* _true_block            = *LLVALUE(true_block);
 	Value* _false_block           = *LLVALUE(false_block);
	Value* _true_block_end        = *LLVALUE(true_block_end);
	Value* _false_block_end       = *LLVALUE(false_block_end);

 	BasicBlock* father_block      = dyn_cast<BasicBlock>(_father);
 	BasicBlock* _true_block_      = dyn_cast<BasicBlock>(_true_block);
 	BasicBlock* _false_block_     = dyn_cast<BasicBlock>(_false_block);
	BasicBlock* _true_block_end_  = dyn_cast<BasicBlock>(_true_block_end);
	BasicBlock* _false_block_end_ = dyn_cast<BasicBlock>(_false_block_end);

	if (isElseWork) {
		BasicBlock* end_block = createBlock();
		if (_true_block_end_->getTerminator() == NULL)
			BranchInst::Create(end_block, _true_block_end_);
		if (_false_block_end_->getTerminator() == NULL)
			BranchInst::Create(end_block, _false_block_end_);
	} else {
		if (_true_block_end_->getTerminator() == NULL)
			BranchInst::Create(_false_block_, _true_block_end_);
	}
	BranchInst* branch = BranchInst::Create(_true_block_, _false_block_, condition, father_block);
}

void llcg_llvm::For(LValue cond, LValue init, LValue pd, LValue work, LValue statement, LValue statement_end, LValue end) {
	Value* condition           = *LLVALUE(cond);
	Value* _init               = *LLVALUE(init);
	Value* _pd                 = *LLVALUE(pd);
	Value* _work               = *LLVALUE(work);
	Value* _statement          = *LLVALUE(statement);
	Value* _statement_end      = *LLVALUE(statement_end);
	Value* _end                = *LLVALUE(end);
	BasicBlock* init_block     = dyn_cast<BasicBlock>(_init);
	BasicBlock* end_block      = dyn_cast<BasicBlock>(_pd);
	BasicBlock* do_block       = dyn_cast<BasicBlock>(_work);
	BasicBlock* work_block     = dyn_cast<BasicBlock>(_statement);
	BasicBlock* work_block_end = dyn_cast<BasicBlock>(_statement_end);
	BasicBlock* false_block    = dyn_cast<BasicBlock>(_end);

	BranchInst* branch = BranchInst::Create(work_block, false_block, condition, end_block);
	if (init_block->getTerminator() == NULL)
		BranchInst::Create(end_block, init_block);
	if (work_block_end->getTerminator() == NULL)
		BranchInst::Create(do_block,  work_block_end);
	if (do_block->getTerminator() == NULL)
		BranchInst::Create(end_block, do_block);
}

void llcg_llvm::While(LValue cond, LValue father, LValue pd, LValue statement, LValue statement_end, LValue end) {
	Value* condition           = *LLVALUE(cond);
	Value* _father             = *LLVALUE(father);
	Value* _pd                 = *LLVALUE(pd);
	Value* _statement          = *LLVALUE(statement);
	Value* _statement_end      = *LLVALUE(statement_end);
	Value* _end                = *LLVALUE(end);
	BasicBlock* father_block   = dyn_cast<BasicBlock>(_father);
	BasicBlock* pd_block       = dyn_cast<BasicBlock>(_pd);
	BasicBlock* true_block     = dyn_cast<BasicBlock>(_statement);
	BasicBlock* true_block_end = dyn_cast<BasicBlock>(_statement_end);
	BasicBlock* false_block    = dyn_cast<BasicBlock>(_end);

	// 生成while循环
	BranchInst* branch = BranchInst::Create(true_block, false_block, condition, pd_block);
	if (father_block->getTerminator() == NULL)
		BranchInst::Create(pd_block, father_block);
	if (true_block_end->getTerminator() == NULL)
		BranchInst::Create(pd_block, true_block_end);
}

void llcg_llvm::DoWhile(LValue statement, LValue pd) {

}

void llcg_llvm::DoUntil(LValue statement, LValue pd) {

}

void llcg_llvm::Goto(LValue target) {
	if (nowBlock->getTerminator()) return;
	Value* v = *LLVALUE(target);
	BasicBlock* target_block = dyn_cast<BasicBlock>(v);
	BranchInst::Create(target_block, nowBlock);
}

LValue llcg_llvm::New(LValue var_type, vector<LValue>& args, const string& funcname) {
	Type* t = *LLTYPE(var_type);
	Type* ITy = Type::getInt64Ty(context);
	Constant* AllocSize = ConstantExpr::getSizeOf(t);
	Instruction* Malloc;
	if (funcname == "") {
		Malloc = CallInst::CreateMalloc(nowBlock, ITy, t, AllocSize);
		Malloc->insertAfter(&(nowBlock->back()));
	} else {
		Function* f = (Function*)(M->getOrInsertFunction(funcname, getMallocType()));
		f->setCallingConv(CallingConv::C);
		vector<Value*> fargs;
		fargs.push_back(AllocSize);
		CallInst* call = CallInst::Create(f, fargs, "", nowBlock);
		Malloc = CastInst::CreatePointerCast(call, t->getPointerTo(), "", nowBlock);
	}
	return LValue(new llvm_value(Malloc));
}

LValue llcg_llvm::NewArray(LValue var_type, vector<LValue>& wd, const string& funcname) {
	// 这里实现自定义的数组malloc函数
	Type* t = *LLTYPE(var_type);
	ConstantInt* zero = ConstantInt::get(Type::getInt64Ty(context), 0);
	vector<Value*> args;
	Constant* AllocSize = ConstantExpr::getSizeOf(t); // 获取当前类型的大小
	t = t->getPointerTo(); // 将t转换为指针类型
	Value* Malloc; CallInst *call;

	args.push_back(AllocSize);
	for (auto p : wd) {
		Value* v = *LLVALUE(p);
		args.push_back(v);
	}
	args.push_back(zero);
	if (funcname == "") {
		string func_name = "malloc_array";
		call = CallInst::Create(M->getFunction(func_name),
			args, "", nowBlock);
	} else {
		Function* f = (Function*)(M->getOrInsertFunction(funcname, getMallocArrayType()));
		f->setCallingConv(CallingConv::C);
		call = CallInst::Create(f, args, "", nowBlock);
	}

	Malloc = CastInst::CreatePointerCast(call, t, "", nowBlock);
	return LValue(new llvm_value(Malloc));
}

LValue   llcg_llvm::Delete(LValue pointer, const string& funcname) {
	Value* ptr  = *LLVALUE(pointer);
	Instruction* Free = CallInst::CreateFree(ptr, nowBlock);
	return LValue(new llvm_value(Free));
}

LValue   llcg_llvm::DeleteArray(LValue pointer, const string& funcname) {
	return NULL;
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
#if defined(LLVM_3_7)
	Constant* strVal = ConstantExpr::getGetElementPtr(GVStr->getType(), GVStr, indices, true);
#else
	Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
#endif
    return strVal;
}

Constant* llcg_llvm::getPtrArray(Module& M, vector<Constant*>& args_list) {
    LLVMContext& ctx = M.getContext(); // 千万别用Global Context
    ArrayType* arr_type = ArrayType::get(Type::getInt8PtrTy(M.getContext()), args_list.size());
    Constant* strConstant = ConstantArray::get(arr_type, args_list);
    GlobalVariable* GVStr =
        new GlobalVariable(M, arr_type, true,
                           GlobalValue::InternalLinkage, strConstant, "");
    Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
    Constant* indices[] = {zero, zero};
#if defined(LLVM_3_7)
	Constant* strVal = ConstantExpr::getGetElementPtr(GVStr->getType(), GVStr, indices, true);
#else
	Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
#endif
	return strVal;
}

LValue llcg_llvm::ConstString(const string& str) {
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

void llcg_llvm::BeginModule(const string& name) {
	M = llvm::make_unique<Module>(name, context);
	register_stdlib(M.get(), stdlibs);
	nowBlock = NULL;
	nowFunc = NULL;
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

void llcg_llvm::SetNowBasicBlock(LValue nowBlock) {
	Value* v = *LLVALUE(nowBlock);
	this->nowBlock = dyn_cast<BasicBlock>(v);
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

void llcg_llvm::VerifyAndWrite(const string& outfile_name) {
	verifyModuleAndWrite(M.get(), outfile_name);
}

void llcg_llvm::verifyModuleAndWrite(llvm::Module* M, const string& outfile_name) {
	// 输出编译后的LLVM可读字节码
	outs() << "LLVM module:\n\n" << *M;
	outs() << "\n\n";
	outs().flush();

    // 校验问题, 这个函数需要一个输出流来打印错误信息
	outs() << "\n";
    if (verifyModule(*M, &outs())) {
        outs() << "构建LLVM字节码出错!\n";
        exit(1);
    }

	std::error_code ErrInfo;
	raw_fd_ostream fs(outfile_name+".bitcode", ErrInfo, sys::fs::F_None);
	fs << *M << "\n";
	fs.flush();

    // 输出二进制BitCode到.bc文件

    raw_fd_ostream out(outfile_name, ErrInfo, sys::fs::F_None);
    WriteBitcodeToFile(M, out);
    out.flush();
}

void llcg_llvm::MakeMetaModule(const string& outfile_name, const string& module_name) {
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

void llcg_llvm::MakeMetaList(const string& name, vector<string>& list, LValue fp) {
	Module* M = meta_M.get();
	Function* F = M->getFunction("elite_meta_init");
	vector<Constant*> args_list;
	for (int i = 0; i < list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, list[i].c_str(), ""));
		// Constant* c = init_list[i];
		// if (c != NULL)
		// 	init_meta_list.push_back(ConstantAsMetadata::get(c));
		// else
		// 	init_meta_list.push_back(MDString::get(M->getContext(), "NULL"));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));

	Constant* list_vec = getPtrArray(*M, args_list);
	vector<Value*> args;
	args.push_back(geti8StrVal(*M, name.c_str(), "")); // 第一个参数, 函数名
	args.push_back(list_vec); // 第二个参数, 函数类型定义字符串列表

	Type* ft = *LLTYPE(fp);
	Function* nowFunc = dyn_cast<Function>(M->getOrInsertFunction(name, (FunctionType*)ft));
	Type* nowType = Type::getInt8PtrTy(M->getContext());
	args.push_back(ConstantExpr::getBitCast(nowFunc, nowType));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_function");
	CallInst::Create(FuncF, args, "", &bb);
}


FunctionType* llcg_llvm::getMallocType() {
	if (malloc_type != NULL) return malloc_type;

	Type* ret = Type::getVoidTy(context)->getPointerTo();
	vector<Type*> types;
	types.push_back(Type::getInt64Ty(context));
	malloc_type = FunctionType::get(ret, types, /*vararg*/false);
	return malloc_type;
}

FunctionType* llcg_llvm::getMallocArrayType() {
	if (malloc_array_type != NULL) return malloc_array_type;

	Type* ret = Type::getVoidTy(context)->getPointerTo();
	vector<Type*> types;
	types.push_back(Type::getInt64Ty(context));
	malloc_array_type = FunctionType::get(ret, types, /*vararg*/true);
	return malloc_array_type;
}


void llcg_llvm::CloseTerminator(LValue basicblock, LValue target) {
	Value* _basicblock = *LLVALUE(basicblock);
	Value* _target     = *LLVALUE(target);
	BasicBlock* bb     = dyn_cast<BasicBlock>(_basicblock);
	BasicBlock* tg     = dyn_cast<BasicBlock>(_target);
	if (bb->getTerminator() == NULL)
		BranchInst::Create(tg, bb);
}

FunctionType* llcg_llvm::malloc_type = NULL;
FunctionType* llcg_llvm::malloc_array_type = NULL;
