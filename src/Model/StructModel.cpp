/* 
* @Author: sxf
* @Date:   2015-10-31 18:24:33
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-31 19:22:30
*/

#include "StructModel.h"

StructModel::StructModel(std::vector<std::string>& name_list, llvm::StructType* t)
{
	for (auto p : name_list) {
		this->name_list.push_back(p);
	}
	struct_type = t;
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