/* 
* @Author: sxf
* @Date:   2015-10-26 14:00:25
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-01 19:56:05
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "StructModel.h"
#include "IDNode.h"
#include <stdio.h>

static Value* function_macro(CodeGenContext* context, Node* node) {

	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name = node->getStr();
	id* i = context->FindST(function_name);
	if (i->type != function_t) return NULL;
	Function* F = (Function*) i->data;

	// 第三个参数, 参数表
	Node* args_node = node = node->getNext();
	std::vector<Type*> type_vec;
	std::vector<std::string> arg_name;
	if (args_node->getChild() != NULL) {
		for (Node* pC = args_node->getChild(); 
			 pC != NULL; pC = pC->getNext() ) 
		{
			Node* pSec = pC->getChild()->getNext();
			Type* t = context->FindType(pSec);
			type_vec.push_back(t);
			StringNode* str_node = (StringNode*)(pSec->getNext());
			arg_name.push_back(str_node->getStr());	
		}
	}

	// 第四个参数, 代码块
	node = node->getNext();
	BasicBlock* bb = context->createBlock(F); // 创建新的Block

	// 特殊处理参数表, 这个地方特别坑，你必须给每个函数的参数
	// 手动AllocaInst开空间，再用StoreInst存一遍才行，否则一Load就报错
	// context->MacroMake(args_node->getChild());
	if (args_node->getChild() != NULL) {
		context->MacroMake(args_node);
		int i = 0;
		for (auto arg = F->arg_begin(); i != arg_name.size(); ++arg, ++i) {
			arg->setName(arg_name[i]);
			Value* argumentValue = arg;
			ValueSymbolTable* st = bb->getValueSymbolTable();
			Value* v = st->lookup(arg_name[i]);
			new StoreInst(argumentValue, v, false, bb);
		}
	}
	context->MacroMake(node);

	// 处理块结尾
	bb = context->getNowBlock();
	if (bb->getTerminator() == NULL)
		ReturnInst::Create(*(context->getContext()), bb);
	return F;
}

static Value* set_macro(CodeGenContext* context, Node* node) {
	// 参数一 类型
	Type* t = context->FindType(node);
	if (t == NULL) {
		errs() <<  "找不到该类型的定义：";
		errs() << ((StringNode*)node)->getStr().c_str() << "\n";
		exit(1);
	}

	// 参数二 变量名
	Node* var_node = node->getNext();
	const char* var_name = ((StringNode*)var_node)->getStr().c_str();
	AllocaInst *alloc = new AllocaInst(t, var_name, context->getNowBlock());

	// 参数三 初始化值
	// 这里有个问题 初始化为常数时,Store会出问题
	node = var_node->getNext();
	if (node == NULL) return alloc;
	Value* init_expr = node->codeGen(context);
	if (init_expr == NULL) {
		errs() << "变量的初始化无效: " << var_name << "\n";
		exit(1);
	}
	new StoreInst(init_expr, alloc, false, context->getNowBlock());
	return alloc;
}

static Value* call_macro(CodeGenContext* context, Node* node) {
	// 参数一 函数名
	Value* func = context->getFunction(node);
	if (func == NULL) {
		errs() <<  "找不到函数的定义：";
		errs() << ((StringNode*)node)->getStr().c_str() << "\n";
		exit(1);
	}

	// 其余参数 要传入的参数
	std::vector<Value*> args;
	for (Node* p = node->getNext(); p != NULL; p = p->getNext()) {
		Value* v = p->codeGen(context);
		if (v != NULL)
			args.push_back(v);
	}

	CallInst *call = CallInst::Create(func, args, "", context->getNowBlock());
	return call;
}

static Value* for_macro(CodeGenContext* context, Node* node) {
	// 参数一 初始化
	BasicBlock* father_block = context->getNowBlock();
	BasicBlock* init_block   = context->createBlock();
	context->MacroMake(node);

	// 参数二 终止条件
	node = node->getNext();
	BasicBlock* end_block = context->createBlock();
	Value* condition = context->MacroMake(node);

	// 参数三 每次循环
	node = node->getNext();
	BasicBlock* do_block = context->createBlock();
	context->MacroMake(node);

	// 参数四 循环体
	node = node->getNext();
	BasicBlock* work_block = context->createBlock();
	context->MacroMake(node);

	// 生成for循环
	BasicBlock* false_block = context->createBlock();
	BranchInst* branch      = BranchInst::Create(work_block, false_block, condition, end_block);
	BranchInst::Create(init_block, father_block);
	BranchInst::Create(end_block, init_block);
	BranchInst::Create(do_block,   work_block);
	BranchInst::Create(end_block,  do_block);

	return branch;
}

static Value* while_macro(CodeGenContext* context, Node* node) {
	// 参数一 条件
	BasicBlock* father_block = context->getNowBlock();
	BasicBlock* pd_block     = context->createBlock();
	Value* condition = context->MacroMake(node);

	// 参数二 循环体
	node = node->getNext();
	BasicBlock* true_block = context->createBlock();
	context->MacroMake(node);

	// 生成while循环
	BasicBlock* false_block = context->createBlock();
	BranchInst* branch      = BranchInst::Create(true_block, false_block, condition, pd_block);
	BranchInst::Create(pd_block, father_block);
	BranchInst::Create(pd_block, true_block);

	return branch;
}

static Value* if_macro(CodeGenContext* context, Node* node) {
	// 参数一 条件
	Value* condition = context->MacroMake(node);
	BasicBlock* father_block = context->getNowBlock();

	// 参数二 为真时, 跳转到的Label
	node = node->getNext();

	BasicBlock* true_block = context->createBlock();
	context->MacroMake(node);

	// 参数三 为假时, 跳转到的Label
	node = node->getNext();
	BasicBlock* false_block = context->createBlock();
	if (node != NULL) {
		context->MacroMake(node);
		BasicBlock* end_block = context->createBlock();
		BranchInst::Create(end_block, true_block);
		BranchInst::Create(end_block, false_block);
	} else {
		BranchInst::Create(false_block, true_block);
	}

	BranchInst* branch = BranchInst::Create(true_block, false_block, condition, father_block);

	return branch;
}
static Value* opt2_macro(CodeGenContext* context, Node* node) {
	std::string opt = node->getStr();

	Node* op1 = (node = node->getNext());
	if (node == NULL) return NULL;
	Node* op2 = (node = node->getNext());
	if (node == NULL) return NULL;

	if (opt == "=") {
		context->setIsSave(true); // 这两句设置的目前是为下面的节点解析时,返回指针而不是load后的值
		Value* ans1 = op1->codeGen(context);
		context->setIsSave(false);
		Value* ans2 = op2->codeGen(context);
		return new StoreInst(ans2, ans1, false, context->getNowBlock());
	}

	if (opt == ".") {
		bool save_bool = false;
		if (context->isSave()) {
			save_bool = true;
			context->setIsSave(false);
		}

		Value* ans1 = op1->codeGen(context);
		StringNode* sn = (StringNode*)op2;
		string ans2 = sn->getStr();
		Type* ans1_type = ans1->getType();
		if (!ans1_type->getPointerElementType()->isStructTy()) {
			errs() << "‘.’运算前，类型错误： " << *(ans1_type) << "\n";
			return NULL;
		}
		string struct_name = ans1_type->getPointerElementType()->getStructName();
		// errs() << "符号: " << struct_name << "\n"; 
		id* i = context->st->find(struct_name);
		if (i == NULL)  {
			errs() << "符号未找到: " << struct_name << "\n"; 
		}
		if (i->type != struct_t) {
			errs() << "‘.’运算前，符号表错误\n"; 
		}
		StructModel* sm = (StructModel*)(i->data);
		// errs() << "ans2: " << ans2 << "\n";
		int n = sm->find(ans2);
 
		// Constant* zero = Constant::getNullValue(Type::getInt64Ty(*(context->getContext())));
		ConstantInt* zero = ConstantInt::get(Type::getInt32Ty(*(context->getContext())), 0);
		ConstantInt* num = ConstantInt::get(Type::getInt32Ty(*(context->getContext())), n);
		// errs() << "0 -> " << *zero << "\n";
		// errs() << n << " -> " << *num << "\n";
    	std::vector<Value*> indices;
    	indices.push_back(zero); 
    	indices.push_back(num);

    	// Type *t = GetElementPtrInst::getIndexedType(ans1->getType(), indices);
    	// errs() << "type : " << *(ans1->getType()->getPointerElementType()) << "\n";
    	// errs() << "type : " << *t << "\n";
		GetElementPtrInst* ptr = GetElementPtrInst::Create(ans1, indices, "", context->getNowBlock());
		if (save_bool) return ptr;
		return new LoadInst(ptr, "", false, context->getNowBlock());			
	}

	Instruction::BinaryOps instr;
	if (opt == "+") { instr = Instruction::Add;  goto binOper; }
	if (opt == "-") { instr = Instruction::Sub;  goto binOper; }
	if (opt == "*") { instr = Instruction::Mul;  goto binOper; }
	if (opt == "/") { instr = Instruction::SDiv; goto binOper; }

	CmpInst::Predicate instp;
	if (opt == "==") { instp = CmpInst::Predicate::ICMP_EQ;  goto cmpOper; } 
	if (opt == "!=") { instp = CmpInst::Predicate::ICMP_NE;  goto cmpOper; } 
	if (opt == "<=") { instp = CmpInst::Predicate::ICMP_SLE; goto cmpOper; } 
	if (opt == ">=") { instp = CmpInst::Predicate::ICMP_SGE; goto cmpOper; } 
	if (opt == "<")  { instp = CmpInst::Predicate::ICMP_SLT; goto cmpOper; } 
	if (opt == ">")  { instp = CmpInst::Predicate::ICMP_SGT; goto cmpOper; } 
	return NULL;

binOper:
	// errs() << opt << ":\t" << *(op1->codeGen(context)) << " " << *(op2->codeGen(context)) << "\n";
	return BinaryOperator::Create(instr, op1->codeGen(context), 
		op2->codeGen(context), "", context->getNowBlock());

cmpOper:
	Value* ans1 = op1->codeGen(context);
	Value* ans2 = op2->codeGen(context);
	// errs() << opt << ":\t" << *ans1 << " " << *ans2 << "\n";
	return CmpInst::Create(Instruction::ICmp, instp, 
		ans1, ans2, 
		"", context->getNowBlock());
}

static Value* struct_macro(CodeGenContext* context, Node* node) {
	return NULL;
}

static Value* return_macro(CodeGenContext* context, Node* node) {
	Value* v = node->codeGen(context);
	return ReturnInst::Create(*(context->getContext()), v, context->getNowBlock());
}

static Value* new_macro(CodeGenContext* context, Node* node) {
	Type* ITy = Type::getInt64Ty(*(context->getContext()));
	id* i = context->FindST(node);
	Type* t;
	if (i->type == type_t) t = (Type*)(i->data);
	else if (i->type == struct_t) {
		StructModel* sm = (StructModel*)(i->data);
		t = sm->struct_type;
	} else { 
		printf("Error: new的类型错误\n");
		return NULL;
	}
	Constant* AllocSize = ConstantExpr::getSizeOf(t);
	// AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);

	BasicBlock* bb = context->getNowBlock();
	Instruction* Malloc = CallInst::CreateMalloc(bb, ITy, t, AllocSize);
	Malloc->insertAfter(&(bb->back()));
	// errs() << "type:" << *t << "\n";
	// errs() << "alloc_size:" << *AllocSize << "\n";
	// errs() << "BasicBlock:" << *bb << "\n";
	// errs() << "Malloc:" << *Malloc << "\n";
	return Malloc;
}

extern const FuncReg macro_funcs[] = {
	{"function", function_macro},
	{"struct",   struct_macro},
	{"set",      set_macro},
	{"call",     call_macro},
	{"opt2",     opt2_macro},
	{"for",      for_macro},
	{"while",    while_macro},
	{"if",       if_macro},
	{"return",   return_macro},
	{"new",      new_macro},
	{NULL, NULL}
};