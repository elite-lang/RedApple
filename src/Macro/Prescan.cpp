/* 
* @Author: sxf
* @Date:   2015-10-29 11:05:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-01 19:10:54
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "Model/StructModel.h"
#include "IDNode.h"
#include <stdio.h>

static Value* function_macro(CodeGenContext* context, Node* node) {
	// 第二个参数, 函数名
	node = node->getNext();
	std::string func_name = node->getStr();
	return NULL;
}

static Value* struct_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 结构体名
	std::string struct_name = node->getStr();
	StructType *ans = StructType::create(*(context->getContext()), struct_name);
	context->st->insert(struct_name, type_t, ans); // 插入符号表中
	return NULL;
}

static Value* struct_type_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 结构体名
	std::string struct_name = node->getStr();
	id* i = context->st->find(struct_name);
	if (i->type != type_t) return NULL;
	StructType* ans = (StructType*)(i->data);

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
	// context->DefType(struct_name, ans);

	StructModel* sm = new StructModel(arg_name, ans);
	context->st->insert(struct_name, struct_t, sm); // 插入符号表中
	return NULL;
}


static Value* function_type_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 返回类型
	Type* ret_type = context->FindType(node);

	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name = node->getStr();

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
	context->st->insert(function_name, function_t, F); // 插入符号表中
	return F;
}

extern const FuncReg macro_prescan[] = {
	{"function", function_macro},
	{"struct",   struct_macro},
	{NULL, NULL}
};

extern const FuncReg macro_pretype[] = {
	{"function", function_type_macro},
	{"struct",   struct_type_macro},
	{NULL, NULL}
};