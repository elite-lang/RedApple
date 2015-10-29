/* 
* @Author: sxf
* @Date:   2015-10-10 18:45:20
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-29 20:02:56
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
		return NULL;
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

void CodeGenContext::DefType(string name, Type* t) {
	st->insert(name, 0, t);
}

Type* CodeGenContext::FindType(string& name) {
	id* i = st->find(name);
	if (i != NULL && i->type == 0) return (Type*)(i->data);
	else {
		errs() <<  "找不到该类型的定义： ";
		errs() << name << "\n";
		return NULL;
	}
}

Type* CodeGenContext::FindType(Node* node) {
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		return FindType(str_node->getStr());
	}
	return 0;
}

void CodeGenContext::setNormalType() {
	DefType("void", Type::getVoidTy(*Context));
	DefType("int", Type::getInt64Ty(*Context));
	DefType("float", Type::getFloatTy(*Context));
	DefType("double", Type::getDoubleTy(*Context));
}


void CodeGenContext::SaveMacros() {
	macro_save_stack.push(macro_map);
}

void CodeGenContext::RecoverMacros() {
	macro_map = macro_save_stack.top();
	macro_save_stack.pop();
}


extern const FuncReg macro_funcs[];
extern const FuncReg macro_classes[];

void CodeGenContext::Init() {
	AddOrReplaceMacros(macro_funcs);
	AddOrReplaceMacros(macro_classes);
	setNormalType();
}

CodeGenContext::CodeGenContext(Node* node) {
	root = node;
	_save = false;
	st = new IDTable();
}

CodeGenContext::~CodeGenContext() {
	delete st;
}
