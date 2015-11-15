/* 
* @Author: sxf
* @Date:   2015-10-29 10:47:04
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-15 16:51:40
*/

#ifndef STRUCT_MODEL_H
#define STRUCT_MODEL_H

#include "MetaModel.h"
#include <vector>
#include <string>

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
	llvm::StructType* getStruct(CodeGenContext* context);

	virtual void insertToST(CodeGenContext* context);
	virtual llvm::Value* genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual llvm::Value* genMetaCode(CodeGenContext* context);
	virtual MetaType getType();
private:
	llvm::StructType* struct_type = NULL;
};



#endif // STRUCT_MODEL_H
