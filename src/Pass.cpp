
#include "Pass.h"

Pass::Pass (const FuncReg* macro_funcs) {
	AddOrReplaceMacros(macro_funcs);
}

Pass::Pass (const FuncReg* macro_funcs, const FuncReg* macro_funcs2) {
	AddOrReplaceMacros(macro_funcs);
	AddOrReplaceMacros(macro_funcs2);
}

Pass::~Pass () {

}

void Pass::Init() {

}

ICodeGenFunction* Pass::getMacro(const string& str) {
	auto func = macro_map.find(str);
	if (func != macro_map.end()) return func->second;
	else return NULL;
}


void Pass::AddOrReplaceMacros(const FuncReg* macro_funcs) {
	while (true) {
		const char*      name = macro_funcs->name;
		CodeGenCFunction func = macro_funcs->func;
		if (name == 0) return;
		string fname = name;
		auto p = macro_map.find(fname);
		if (p != macro_map.end())
			p->second = new CodeGenFunction(func);
		else
			macro_map[fname] = new CodeGenFunction(func);
		++macro_funcs;
	}
}

void Pass::AddOrReplaceMacros(const string& name, ICodeGenFunction* func) {
	macro_map[name] = func;
}

void Pass::RemoveAllMacros() {
	macro_map.clear();
}


void Pass::SaveMacros() {
	macro_save_stack.push(macro_map);
}

void Pass::RecoverMacros() {
	macro_map = macro_save_stack.top();
	macro_save_stack.pop();
}
