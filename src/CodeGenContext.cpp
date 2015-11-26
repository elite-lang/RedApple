/* 
* @Author: sxf
* @Date:   2015-10-10 18:45:20
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-26 20:38:59
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "IDNode.h"
#include <iostream>

#include "MacroTranslate.h"

using namespace std;

LValue CodeGenContext::MacroMake(Node* node) {
	if (node == NULL) return NULL;

	if (node->isIDNode()) {
		shared_ptr<MacroModel> p = getUserMacro(node->getStr());
		if (p != NULL) {
			Node* user_macro_node = p->getData();
			MacroTranslate translater;
			Node* replace = translater.Marco(user_macro_node, node->getNext());
			replace->print(1);
			LValue ans = MacroMake(replace);
			Node::FreeAll(replace);
			return ans;
		}
		CodeGenFunction func = getMacro(node->getStr());
		if (func != NULL) {
			return func(this, node->getNext());
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

CodeGenFunction CodeGenContext::getMacro(string& str) {
	auto func = macro_map.find(str);
	if (func != macro_map.end()) return func->second;
	else return NULL;
}

void CodeGenContext::ScanOther(Node* node) {
	PreMacro();
    MacroMake(node);
    printf("-- 宏解析流程完成 --\n");
    PreInit();
    MacroMake(node);
    printf("-- 预处理流程完成 --\n");
    // pretype流程，负责处理所有类型相关问题并更新符号表
    PreTypeInit();
    MacroMake(node);
    printf("-- 类型化流程完成 --\n");
}

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

void CodeGenContext::RemoveAllMacros() {
	macro_map.clear();
}


id* CodeGenContext::FindST(Node* node) const{ 
	return FindST(node->getStr());
}

LValue CodeGenContext::getFunction(Node* node) {
	return getFunction(node->getStr());
}

LValue CodeGenContext::getFunction(std::string& name) {
	shared_ptr<FunctionModel> fm = getFunctionModel(name);
	if (fm != NULL) return fm->getFunction(this);
	LValue defined_func = getLLCG()->getFunction(name);
	if (defined_func != NULL) return defined_func;
	cerr << "错误！函数未定义：" << name << endl;
	return NULL;
}

shared_ptr<FunctionModel> CodeGenContext::getFunctionModel(string& name) {
	id* i = FindST(name);
	if (i == NULL) {
		return NULL;
	}
	if (i->type != function_t) { 
		return NULL;
	}
	return dynamic_pointer_cast<FunctionModel>(i->data);
}

shared_ptr<MacroModel> CodeGenContext::getUserMacro(std::string& name) {
	id* i = FindST(name);
	if (i == NULL) return NULL;
	if (i->type != macro_t) return NULL;
	return dynamic_pointer_cast<MacroModel>(i->data);
}

void  CodeGenContext::setUserMacro(std::string& name, Node* node) {
	LValue p(new MacroModel(name, node));
	st->insert(name, macro_t, p);
}

shared_ptr<StructModel> CodeGenContext::getStructModel(string& name) {
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

LValue CodeGenContext::FindSrcType(string& name) {
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

LValue CodeGenContext::FindType(string& name) {
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
	if (t->isStructType() && !is_source) t = t->getPointerTo();
	if (d > 1) {
		// t = ArrayType::get(t, 0);
		t = t->getPointerTo();
	}
	return t;
}

LValue CodeGenContext::FindType(Node* node) {
	return FindType(node->getStr());
}

void CodeGenContext::DefVar(string& name, LValue addr) {
	st->insert(name, var_t, addr);
}

LValue CodeGenContext::FindVar(string& name) {
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


void CodeGenContext::SaveMacros() {
	macro_save_stack.push(macro_map);
}

void CodeGenContext::RecoverMacros() {
	macro_map = macro_save_stack.top();
	macro_save_stack.pop();
}


extern const FuncReg macro_funcs[];
extern const FuncReg macro_classes[];
extern const FuncReg macro_prescan[];
extern const FuncReg macro_pretype[];
extern const FuncReg macro_defmacro[];

void CodeGenContext::PreInit() {
	RemoveAllMacros();
	AddOrReplaceMacros(macro_prescan);
}

void CodeGenContext::PreTypeInit() {
	RemoveAllMacros();
	AddOrReplaceMacros(macro_pretype);
}

void CodeGenContext::PreMacro() {
	RemoveAllMacros();
	AddOrReplaceMacros(macro_defmacro);
}

void CodeGenContext::Init() {
	RemoveAllMacros();
	AddOrReplaceMacros(macro_funcs);
	AddOrReplaceMacros(macro_classes);
}

CodeGenContext::CodeGenContext(Node* node) {
	root = node;
	_save = false;
	st = new IDTable();
	codeGenerator = llcg::CreateLLVM();
	setNormalType();
}

CodeGenContext::~CodeGenContext() {
	delete st;	
}
