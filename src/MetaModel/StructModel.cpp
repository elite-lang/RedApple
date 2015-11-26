/* 
* @Author: sxf
* @Date:   2015-10-31 18:24:33
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-26 20:36:36
*/

#include "StructModel.h"
#include "CodeGenContext.h"




StructModel::StructModel(std::string& name,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list)
	: MetaModel(name)
{
	this->type_list = type_list;
	this->name_list = name_list;
}

int StructModel::find(std::string& name)
{
	int i = 0;
	if (name_list.size() == 0) return -1;
	for (auto& p : name_list) {
		if (name == p) return i;
		++i;
	}
	return -1;
}

void StructModel::insertToST(CodeGenContext* context) {
	context->st->insert(name, struct_t, LValue(this)); // 插入符号表中
}

LValue StructModel::getStruct(CodeGenContext* context) {
	if (struct_type == NULL)
		struct_type = context->getLLCG()->DeclareStruct(name);
	return struct_type;
}

void StructModel::genCode(CodeGenContext* context) {
	std::vector<LValue> type_vec;
	for (auto& s : type_list) {
		LValue t = context->FindType(s);
		type_vec.push_back(t);
	}
	struct_type = context->getLLCG()->Struct(getStruct(context), type_vec);
}

cJSON* StructModel::genJson() {

}

void StructModel::genMetaCode(CodeGenContext* context) {
	vector<string> data;
	data.push_back("struct");
	data.push_back(name);
	for (int i = 0; i < name_list.size(); ++i) {
		data.push_back(type_list[i]);
		data.push_back(name_list[i]);
	}
	context->getLLCG()->MakeMetaList(data);
}

MetaType StructModel::getMetaType() {
	return struct_meta_t;
}
