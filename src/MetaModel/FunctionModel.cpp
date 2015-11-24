/* 
* @Author: sxf
* @Date:   2015-11-13 16:45:51
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 14:58:14
*/

#include "FunctionModel.h"
#include "CodeGenContext.h"

FunctionModel::FunctionModel(
	std::string& 			  name,
	std::string& 			  ret_type,
	std::vector<std::string>& type_list,
	std::vector<std::string>& name_list,
	std::vector<Constant*>&   init_list
) : MetaModel(name)
{
	this->return_type = ret_type;
	this->type_list = type_list;
	this->name_list = name_list;
	this->init_list = init_list;
}

int FunctionModel::find(std::string& name)
{
	int i = 0;
	if (name_list.size() == 0) return -1;
	for (auto& p : name_list) {
		if (name == p) return i;
		++i;
	}
	return -1;
}

void FunctionModel::insertToST(CodeGenContext* context) {
	context->st->insert(name, function_t, this); // 插入符号表中
}

llvm::Function* FunctionModel::getFunction(CodeGenContext* context) {
	if (func != NULL) return func;
	Module* M = context->getModule();
	func = dyn_cast<Function>(M->getOrInsertFunction(name, func_type));
	return func;
}


Value* FunctionModel::genCode(CodeGenContext* context) {
	Type* ret_type = context->FindType(return_type);
	std::vector<Type*> type_vec;
	for (auto& s : type_list) {
		Type* t = context->FindType(s);
		type_vec.push_back(t);
	}
	// 先合成一个函数
	func_type = FunctionType::get(ret_type, type_vec, 
		/*not vararg*/false);
	return NULL;
}

cJSON* FunctionModel::genJson() {

}

Value* FunctionModel::genMetaCode(CodeGenContext* context) {
	Module* M = context->getMetaModule();
	Function* F = M->getFunction("elite_meta_init");
	vector<Value*> args_list;
	args_list.push_back(geti8StrVal(*M, name.c_str()));
	args_list.push_back(geti8StrVal(*M, return_type.c_str()));
	vector<Metadata*> init_meta_list;
	for (int i = 0; i < name_list.size(); ++i) {
		args_list.push_back(geti8StrVal(*M, type_list[i].c_str()));
		args_list.push_back(geti8StrVal(*M, name_list[i].c_str()));
		Constant* c = init_list[i];
		if (c != NULL)
			init_meta_list.push_back(ConstantAsMetadata::get(c));
		else
			init_meta_list.push_back(MDString::get(M->getContext(), "NULL"));
	}
	args_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));
	Function* nowFunc = dyn_cast<Function>(M->getOrInsertFunction(name, func_type));
	Type* nowType = func_type->getPointerTo();
	args_list.push_back(ConstantExpr::getBitCast(nowFunc, nowType));
	BasicBlock& bb = F->getEntryBlock();
	Function* FuncF = M->getFunction("elite_meta_function");
	CallInst::Create(FuncF, args_list, "", &bb);
	return NULL;
}

MetaType FunctionModel::getType() {
	return function_meta_t;
}