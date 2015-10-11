/* 
* @Author: sxf
* @Date:   2015-10-10 18:45:20
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-11 14:16:35
*/

#include "CodeGenContext.h"
#include "StringNode.h"

Value* CodeGenContext::MacroMake(Node* node) {
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		CodeGenFunction func = getMacro(str_node->getStr());
		if (func != NULL) {
			return func(this, node->getNext());
		}
	}
	return NULL;
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
		if (p == macro_map.end()) p->second = func;
		else macro_map.insert(pair<string, CodeGenFunction>(name, func));
		++macro_funcs;
	}
}

Value* function_macro(CodeGenContext* context, Node* node) {
	std::string function_name;
	if (node->getType() == "StringNode") {
		StringNode* str_node = (StringNode*)node;
		function_name = str_node->getStr();
	}
	Module* M = context->getModule();
	M->getOrInsertFunction(function_name.c_str(), 
		Type::getInt32Ty(*(context->getContext())),
		Type::getInt32Ty(*(context->getContext())),
		(Type *)0);
}

Value* class_macro(CodeGenContext* content, Node* node) {

}

Value* module_macro(CodeGenContext* content, Node* node) {

}

Value* set_macro(CodeGenContext* content, Node* node) {

}

Value* call_macro(CodeGenContext* content, Node* node) {

}

Value* for_macro(CodeGenContext* content, Node* node) {

}

Value* while_macro(CodeGenContext* content, Node* node) {

}

Value* if_macro(CodeGenContext* content, Node* node) {

}

static const FuncReg macro_funcs[] = {
	{"function", function_macro},
	{"class", class_macro},
	{"module", module_macro},
	{"set", set_macro},
	{"call", call_macro},
	{"for", for_macro},
	{"while", while_macro},
	{"if", if_macro},
	{NULL, NULL}
};

CodeGenContext::CodeGenContext(Node* node) {
	root = node;
	AddOrReplaceMacros(macro_funcs);
}

CodeGenContext::~CodeGenContext() {
	
}