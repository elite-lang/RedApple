/* 
* @Author: sxf
* @Date:   2015-10-29 11:05:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 17:16:03
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "MetaModel/StructModel.h"
#include "MetaModel/FunctionModel.h"
#include "IDNode.h"
#include <stdio.h>

static LValue function_macro(CodeGenContext* context, Node* node) {
		// 第一个参数, 返回类型
	std::string ret_type = node->getStr();

	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name = node->getStr();

	// 第三个参数, 参数表
	Node* args_node = node = node->getNext();
	std::vector<std::string> type_vec;
	std::vector<std::string> arg_name;
	if (args_node->getChild() != NULL) {
		for (Node* pC = args_node->getChild(); 
			 pC != NULL; pC = pC->getNext() ) 
		{
			Node* pSec = pC->getChild()->getNext();
			type_vec.push_back(pSec->getStr());
			pSec = pSec->getNext();
			arg_name.push_back(pSec->getStr());	
			pSec = pSec->getNext();
			if (pSec != NULL)
			{
				LValue v = pSec->getNext()->codeGen(context);
			} else {
			}
		}
	}
	FunctionModel* fm = new FunctionModel(function_name, ret_type, type_vec, arg_name);
	fm->insertToST(context);
	return NULL;
}

static LValue struct_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 结构体名
	std::string struct_name = node->getStr();
	Node* args_node = node->getNext();
	std::vector<std::string> type_vec;
	std::vector<std::string> arg_name;
	if (args_node->getChild() != NULL) {
		for (Node* pC = args_node->getChild(); 
			 pC != NULL; pC = pC->getNext() ) 
		{
			Node* pSec = pC->getChild()->getNext();
			type_vec.push_back(pSec->getStr());
			arg_name.push_back(pSec->getNext()->getStr());	
		}
	}
	StructModel* sm = new StructModel(struct_name, type_vec, arg_name);
	sm->insertToST(context);
	return NULL;
}

static LValue struct_type_macro(CodeGenContext* context, Node* node) {
	// 第一个参数, 结构体名
	std::string struct_name = node->getStr();
	id* i = context->st->find(struct_name);
	if (i->type != struct_t) return NULL;
	auto sm = dynamic_pointer_cast<StructModel>(i->data);
	sm->genCode(context);
	return NULL;
}


static LValue function_type_macro(CodeGenContext* context, Node* node) {
	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name = node->getStr();
	id* i = context->st->find(function_name);
	if (i->type != function_t) return NULL;
	auto fm = dynamic_pointer_cast<FunctionModel>(i->data);
	fm->genCode(context);
	return NULL;
}

static LValue defmacro_macro(CodeGenContext* context, Node* node) {
	context->setUserMacro(node->getStr(), node->getNext());
	return NULL;
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

extern const FuncReg macro_defmacro[] = {
	{"defmacro",   defmacro_macro},
	{NULL, NULL}
};
