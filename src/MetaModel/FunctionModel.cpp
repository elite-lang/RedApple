/* 
* @Author: sxf
* @Date:   2015-11-13 16:45:51
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-14 17:00:47
*/

#include "FunctionModel.h"

FunctionModel(std::string& name,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list,
		std::vector<Constant*>&   init_list)
	: MetaType(name)
{
	meta_type = struct_meta_t;
	this->type_list = type_list;
	this->name_list = name_list;
	this->init_list = init_list;
}

int StructModel::find(std::string name)
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

Value* FunctionModel::genCode(CodeGenContext* context) {
	Type ret_type = context->FindType(return_type);
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

}

MetaType FunctionModel::getType() {
	return function_meta_t;
}