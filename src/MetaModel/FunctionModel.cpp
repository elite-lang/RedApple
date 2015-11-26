/* 
* @Author: sxf
* @Date:   2015-11-13 16:45:51
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 22:20:59
*/

#include "FunctionModel.h"
#include "CodeGenContext.h"

FunctionModel::FunctionModel(
	std::string& 			  name,
	std::string& 			  ret_type,
	std::vector<std::string>& type_list,
	std::vector<std::string>& name_list
) : MetaModel(name)
{
	this->return_type = ret_type;
	this->type_list = type_list;
	this->name_list = name_list;
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
	context->st->insert(name, function_t, LValue(this)); // 插入符号表中
}

LValue FunctionModel::getFunction(CodeGenContext* context) {
	if (func != NULL) return func;
	func = context->getLLCG()->GetOrInsertFunction(name, func_type);
	return func;
}


void FunctionModel::genCode(CodeGenContext* context) {
	LValue ret_type = context->FindType(return_type);
	std::vector<LValue> type_vec;
	for (auto& s : type_list) {
		LValue t = context->FindType(s);
		type_vec.push_back(t);
	}
	// 先合成一个函数
	func_type = context->getLLCG()->FuncType(ret_type, type_vec, 
		/*not vararg*/false);
}

cJSON* FunctionModel::genJson() {

}

void FunctionModel::genMetaCode(CodeGenContext* context) {
	vector<string> data;
	data.push_back("function");
	data.push_back(name);
	data.push_back(return_type);
	for (int i = 0; i < name_list.size(); ++i) {
		data.push_back(type_list[i]);
		data.push_back(name_list[i]);
	}
	context->getLLCG()->MakeMetaList(name, data, func_type);
}

MetaType FunctionModel::getMetaType() {
	return function_meta_t;
}