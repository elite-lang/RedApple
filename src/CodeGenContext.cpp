/* 
* @Author: sxf
* @Date:   2015-10-10 18:45:20
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-17 12:20:10
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "IDNode.h"
#include <stdio.h>

Value* CodeGenContext::MacroMake(Node* node) {
	if (node == NULL) return NULL;
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		CodeGenFunction func = getMacro(str_node->getStr());
		if (func != NULL) {
			return func(this, node->getNext());
		}
	} 
	if (node->getChild() != NULL && node->getChild()->getType() == "StringNode")
		return MacroMake(node->getChild());
	Value* ans;
	for (Node* p = node->getChild(); p != NULL; p = p->getNext()) 
		ans = MacroMake(p);
	return ans;
}

CodeGenFunction CodeGenContext::getMacro(string& str) {
	auto func = macro_map.find(str);
	if (func != macro_map.end()) return func->second;
	else return NULL;
}

// void CodeGenContext::AddMacros(const FuncReg* macro_funcs) {
//
// }

void CodeGenContext::AddOrReplaceMacros(const FuncReg* macro_funcs) {
	while (true) {
		const char*     name = macro_funcs->name;
		CodeGenFunction func = macro_funcs->func;
		if (name == NULL) return;
		auto p = macro_map.find(name);
		if (p != macro_map.end()) p->second = func;
		else macro_map.insert(pair<string, CodeGenFunction>(name, func));
		++macro_funcs;
	}
}

Type* CodeGenContext::getNormalType(Node* node) {
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		return getNormalType(str_node->getStr());
	}
	return 0;
}

Type* CodeGenContext::getNormalType(std::string& str) {
	if (str == "void") 
		return Type::getVoidTy(*Context);
	if (str == "int") 
		return Type::getInt64Ty(*Context);
	if (str == "float") 
		return Type::getFloatTy(*Context);
	if (str == "double") 
		return Type::getDoubleTy(*Context);
	return 0;
}

Function* CodeGenContext::getFunction(Node* node) {
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		return getFunction(str_node->getStr());
	}
	return 0;
}

Function* CodeGenContext::getFunction(std::string& name) {
	return M->getFunction(name);
}

void CodeGenContext::nowFunction(Function* _nowFunc) {
	nowFunc = _nowFunc;
}

BasicBlock* CodeGenContext::getNowBlock() {
	return nowBlock;
}

BasicBlock* CodeGenContext::createBlock() {
	return nowBlock = BasicBlock::Create(*Context, "", nowFunc);
}

BasicBlock* CodeGenContext::createBlock(Function* f) {
	nowFunc = f;
	return nowBlock = BasicBlock::Create(*Context, "entry", f);
}

Value* function_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 返回类型
	Type* ret_type = context->getNormalType(node);

	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name;
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		function_name = str_node->getStr();
	}

	// 第三个参数, 参数表
	Node* args_node = node = node->getNext();
	std::vector<Type*> type_vec;
	std::vector<std::string> arg_name;
	if (args_node->getChild() != NULL) {
		for (Node* pC = args_node->getChild(); 
			 pC != NULL; pC = pC->getNext() ) 
		{
			Node* pSec = pC->getChild()->getNext();
			Type* t = context->getNormalType(pSec);
			type_vec.push_back(t);
			StringNode* str_node = (StringNode*)(pSec->getNext());
			arg_name.push_back(str_node->getStr());	
		}
	}
	// 先合成一个函数
	FunctionType *FT = FunctionType::get(ret_type, type_vec, 
		/*not vararg*/false);

	Module* M = context->getModule();
	Function *F = Function::Create(FT, Function::ExternalLinkage, 
		function_name, M);
	context->nowFunction(F);

	if (F->getName() != function_name) {
		// Delete the one we just made and get the existing one.
		F->eraseFromParent();
		F = M->getFunction(function_name);

		// If F already has a body, reject this.
		if (!F->empty()) {
			fprintf(stderr, "Error: redefinition of function\n");
			return 0;
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

Value* class_macro(CodeGenContext* context, Node* node) {
	return NULL;
}

Value* module_macro(CodeGenContext* context, Node* node) {
	return NULL;
}

Value* set_macro(CodeGenContext* context, Node* node) {
	// 参数一 类型
	Type* t = context->getNormalType(node);
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

Value* call_macro(CodeGenContext* context, Node* node) {
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

Value* for_macro(CodeGenContext* context, Node* node) {
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

Value* while_macro(CodeGenContext* context, Node* node) {
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

Value* if_macro(CodeGenContext* context, Node* node) {
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

Value* opt2_macro(CodeGenContext* context, Node* node) {
	if (node->getType() != "StringNode") return NULL;
	StringNode* str_node = (StringNode*)node;
	std::string opt = str_node->getStr();

	Node* op1 = (node = node->getNext());
	if (node == NULL) return NULL;
	Node* op2 = (node = node->getNext());
	if (node == NULL) return NULL;

	if (opt == "=") {
		std::string name = ((IDNode*)op1)->getStr();
		BasicBlock* bb = context->getNowBlock();
		ValueSymbolTable* st = bb->getValueSymbolTable();
		Value* ans1 = st->lookup(name);
		Value* ans2 = op2->codeGen(context);
		return new StoreInst(ans2, ans1, false, context->getNowBlock());
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

static const FuncReg macro_funcs[] = {
	{"function", function_macro},
	{"class",    class_macro},
	{"module",   module_macro},
	{"set",      set_macro},
	{"call",     call_macro},
	{"opt2",     opt2_macro},
	{"for",      for_macro},
	{"while",    while_macro},
	{"if",       if_macro},
	{NULL, NULL}
};

CodeGenContext::CodeGenContext(Node* node) {
	root = node;
	AddOrReplaceMacros(macro_funcs);
}

CodeGenContext::~CodeGenContext() {
	
}
