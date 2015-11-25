/* 
* @Author: sxf
* @Date:   2015-10-26 14:05:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 15:35:28
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "IDNode.h"
#include <stdio.h>

extern const FuncReg macro_classes_replace[];

static LValue class_macro(CodeGenContext* context, Node* node) {
	context->SaveMacros();
	context->AddOrReplaceMacros(macro_classes_replace);


	context->RecoverMacros();
	return NULL;
}

static LValue module_macro(CodeGenContext* context, Node* node) {
	

	return NULL;
}

extern const FuncReg macro_funcs[];

static LValue function_macro(CodeGenContext* context, Node* node) {
	context->SaveMacros();
	context->AddOrReplaceMacros(macro_funcs);

	context->RecoverMacros();
	return NULL;
}

static LValue set_macro(CodeGenContext* context, Node* node) {
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