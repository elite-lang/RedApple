/* 
* @Author: sxf
* @Date:   2015-10-26 14:05:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-26 20:52:11
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "IDNode.h"
#include <stdio.h>

extern const FuncReg macro_classes_replace[];

static Value* class_macro(CodeGenContext* context, Node* node) {
	context->SaveMacros();
	context->AddOrReplaceMacros(macro_classes_replace);


	context->RecoverMacros();
	return NULL;
}

static Value* module_macro(CodeGenContext* context, Node* node) {
	

	return NULL;
}

extern const FuncReg macro_funcs[];

static Value* function_macro(CodeGenContext* context, Node* node) {
	context->SaveMacros();
	context->AddOrReplaceMacros(macro_funcs);

	context->RecoverMacros();
	return NULL;
}

static Value* set_macro(CodeGenContext* context, Node* node) {
	return NULL;
}


extern const FuncReg macro_classes_replace[] = {
	{"function",    function_macro},
	{"set",   set_macro},
	{NULL, NULL}
};


extern const FuncReg macro_classes[] = {
	{"class",    class_macro},
	{"module",   module_macro},
	{NULL, NULL}
};