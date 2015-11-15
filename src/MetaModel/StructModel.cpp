/* 
* @Author: sxf
* @Date:   2015-10-31 18:24:33
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-15 16:51:15
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
	context->st->insert(name, struct_t, this); // 插入符号表中
}

llvm::StructType* StructModel::getStruct(CodeGenContext* context) {
	if (struct_type == NULL)
		struct_type = StructType::create(*(context->getContext()), name);
	return struct_type;
}

Value* StructModel::genCode(CodeGenContext* context) {
	std::vector<Type*> type_vec;
	for (auto& s : type_list) {
		Type* t = context->FindType(s);
		type_vec.push_back(t);
	}
	StructType* st = getStruct(context);
	st->setBody(type_vec);
	return NULL;
}

cJSON* StructModel::genJson() {

}

Value* StructModel::genMetaCode(CodeGenContext* context) {

}

MetaType StructModel::getType() {
	return struct_meta_t;
}
