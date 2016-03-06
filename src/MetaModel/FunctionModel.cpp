/*
* @Author: sxf
* @Date:   2015-11-13 16:45:51
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:46:12
*/

#include "MetaModel/FunctionModel.h"
#include "CodeGenContext.h"
#include "idtable.h"

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
	return context->getLLCG()->GetOrInsertFunction(name, func_type);
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


void FunctionModel::genMetaCode(CodeGenContext* context) {
	vector<string> data;
	data.push_back(return_type); // 参数表1, 返回类型
	for (int i = 0; i < name_list.size(); ++i) {
		data.push_back(type_list[i]); // 类型
		data.push_back(name_list[i]); // 名称
	}
	context->getLLCG()->MakeMetaList(name, data, func_type);
}

MetaType FunctionModel::getMetaType() {
	return function_meta_t;
}
