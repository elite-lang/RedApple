/*
* @Author: sxf
* @Date:   2015-10-10 18:45:20
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:37:24
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "IDNode.h"
#include <iostream>
#include "idtable.h"
#include "MacroTranslate.h"
#include "Pass.h"

using namespace std;

LValue CodeGenContext::MacroMake(Node* node) {
	if (node == NULL) return NULL;

	if (node->isIDNode()) {
		shared_ptr<MacroModel> p = getUserMacro(node->getStr());
		if (p != NULL) {
			Node* user_macro_node = p->getData();
			MacroTranslate translater;
			Node* replace = translater.Marco(user_macro_node, node->getNext());
			LValue ans = MacroMake(replace);
			Node::FreeAll(replace);
			return ans;
		}
		ICodeGenFunction* func = now_pass->getMacro(node->getStr());
		if (func != NULL) {
			return func->call(this, node->getNext());
		}
		return NULL;
	}
	if (node->getChild() != NULL && node->getChild()->isIDNode())
		return MacroMake(node->getChild());
	LValue ans;
	for (Node* p = node->getChild(); p != NULL; p = p->getNext())
		ans = MacroMake(p);
	return ans;
}


id* CodeGenContext::FindST(Node* node) const{
	return FindST(node->getStr());
}

id* CodeGenContext::FindST(const string& str) const {
	return st->find(str);
}

LValue CodeGenContext::getFunction(Node* node) {
	return getFunction(node->getStr());
}

LValue CodeGenContext::getFunction(const std::string& name) {
	shared_ptr<FunctionModel> fm = getFunctionModel(name);
	if (fm != NULL) return fm->getFunction(this);
	LValue defined_func = getLLCG()->getFunction(name);
	if (defined_func != NULL) return defined_func;
	cerr << "函数未定义: " << name << endl;
	return NULL;
}

shared_ptr<FunctionModel> CodeGenContext::getFunctionModel(const string& name) {
	id* i = FindST(name);
	if (i == NULL) {
		return NULL;
	}
	if (i->type != function_t) {
		return NULL;
	}
	return dynamic_pointer_cast<FunctionModel>(i->data);
}

shared_ptr<MacroModel> CodeGenContext::getUserMacro(const std::string& name) {
	id* i = FindST(name);
	if (i == NULL) return NULL;
	if (i->type != macro_t) return NULL;
	return dynamic_pointer_cast<MacroModel>(i->data);
}

void  CodeGenContext::setUserMacro(const std::string& name, Node* node) {
	LValue p(new MacroModel(name, node));
	st->insert(name, macro_t, p);
}

shared_ptr<StructModel> CodeGenContext::getStructModel(const string& name) {
	id* i = FindST(name);
	if (i == NULL) {
		printf("错误: 符号 %s 未找到\n", name.c_str());
		return NULL;
	}
	if (i->type != struct_t) {
		printf("错误: 符号 %s 类型不是结构体\n", name.c_str());
		return NULL;
	}
	return dynamic_pointer_cast<StructModel>(i->data);
}


void CodeGenContext::DefType(string name, LValue t) {
	st->insert(name, type_t, t);
}

LValue CodeGenContext::FindSrcType(const string& name) {
	id* i = st->find(name);
	LValue ret_type;
	if (i != NULL && i->type == type_t) {
		LValue t = i->data;
		ret_type = t;
	} else if (i != NULL && i->type == struct_t) {
		auto s = dynamic_pointer_cast<StructModel>(i->data);
		ret_type = s->getStruct(this);
	} else {
		cerr << "找不到该类型的定义： ";
		cerr << name << "\n";
		return NULL;
	}
	return ret_type;
}

LValue CodeGenContext::FindSrcType(Node* node) {
	return FindSrcType(node->getStr());
}

LValue CodeGenContext::FindType(const string& name) {
	string find_name = name; int d = 0;
	bool is_source = false;
	if (find_name[0] == '*') {
		is_source = true;
		find_name.erase(find_name.begin());
	}

	while (find_name.back() == ']') {
		find_name.pop_back();
		find_name.pop_back();
		++d;
	}
	LValue t = FindSrcType(find_name);
	if (t->isStructType() && !is_source)
		t = t->getPointerTo();
	if (d >= 1) { // - -! 这么白痴的错误居然瞪了两个小时没发现
		t = t->getPointerTo();
	}
	return t;
}

LValue CodeGenContext::FindType(Node* node) {
	return FindType(node->getStr());
}

void CodeGenContext::DefVar(const string& name, LValue addr) {
	st->insert(name, var_t, addr);
}

LValue CodeGenContext::FindVar(const string& name) {
	id* i = st->find(name);
	if (i != NULL && i->type == var_t) {
		return i->data;
	}
	cerr << "变量未找到: " << name << endl;
	return NULL;
}


void CodeGenContext::setNormalType() {
	DefType("void",   codeGenerator->Void());
	DefType("int",    codeGenerator->Int64());
	DefType("float",  codeGenerator->Float());
	DefType("double", codeGenerator->Double());

	DefType("int8",   codeGenerator->Int8());
	DefType("int16",  codeGenerator->Int16());
	DefType("int32",  codeGenerator->Int32());
	DefType("int64",  codeGenerator->Int64());

	DefType("uint64",  codeGenerator->Int64());
	DefType("uint8",   codeGenerator->Int8());
	DefType("uint16",  codeGenerator->Int16());
	DefType("uint32",  codeGenerator->Int32());
	DefType("uint64",  codeGenerator->Int64());

	DefType("byte",   codeGenerator->Int8());
	DefType("char",   codeGenerator->Int8());
	DefType("wchar",  codeGenerator->Int32());
}


CodeGenContext::CodeGenContext() {
	_save = false;
	st = new IDTable();
	codeGenerator = llcg::CreateLLVM();
	setNormalType();
}

void CodeGenContext::Init() {
	_save = false;
}

CodeGenContext::~CodeGenContext() {
	delete st;
	delete codeGenerator;
}
