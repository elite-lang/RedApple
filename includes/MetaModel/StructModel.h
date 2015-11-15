/* 
* @Author: sxf
* @Date:   2015-10-29 10:47:04
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-14 15:43:58
*/

#ifndef STRUCT_MODEL_H
#define STRUCT_MODEL_H

#include "MetaModel.h"
#include <vector>
#include <iostream>

namespace llvm {
	class StructType;
}

class StructModel : public MetaModel
{
public:
	StructModel(std::string& name,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list);

	int find(std::string& name);

	std::vector<std::string> type_list;
	std::vector<std::string> name_list;
	llvm::StructType* struct_type;

	virtual void insertToST(CodeGenContext* context);
	virtual Value* genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual Value* genMetaCode(CodeGenContext* context);
	virtual MetaType getType();
};



#endif // STRUCT_MODEL_H
