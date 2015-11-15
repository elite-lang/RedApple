/* 
* @Author: sxf
* @Date:   2015-11-13 12:07:03
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-14 16:42:01
*/


#ifndef FUNCTION_MODEL_H
#define FUNCTION_MODEL_H

#include "MetaModel.h"

namespace llvm {
	class Function;
	class FunctionType;
}


class FunctionModel : public MetaModel
{
public:
	FunctionModel(std::string& name,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list,
		std::vector<Constant*>&   init_list
	);

	int find(std::string& name);

	std::string return_type;
	std::vector<std::string> type_list;
	std::vector<std::string> name_list;
	std::vector<Constant*>   init_list; // 没有的置为NULL

	llvm::Function* func;
	llvm::FunctionType* func_type;

	virtual void insertToST(CodeGenContext* context);
	virtual Value* genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual Value* genMetaCode(CodeGenContext* context);
	virtual MetaType getType();
};



#endif // FUNCTION_MODEL_H
