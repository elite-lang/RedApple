/* 
* @Author: sxf
* @Date:   2015-10-29 11:05:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-29 20:04:54
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "Model/StructModel.h"
#include "IDNode.h"
#include <stdio.h>


static Value* function_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 返回类型
	Type* ret_type = context->FindType(node);

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
			Type* t = context->FindType(pSec);
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

static Value* struct_macro(CodeGenContext* context, Node* node) {
		// 第一个参数, 结构体名
	std::string struct_name;
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		struct_name = str_node->getStr();
	}

	StructType *ans = StructType::create(*(context->getContext()), struct_name);

	// 第二个参数, 类型表
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
	ans->setBody(type_vec);
	context->DefType(struct_name, ans);

	StructModel* sm = new StructModel(arg_name, ans);
	context->st->insert(struct_name, 1, sm); // 插入符号表中
	return NULL;
}

extern const FuncReg macro_prescan[] = {
	{"function", function_macro},
	{"struct",   struct_macro},
	{NULL, NULL}
};