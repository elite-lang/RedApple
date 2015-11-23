/* 
* @Author: sxf
* @Date:   2015-11-13 12:07:03
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-15 16:51:49
*/


#ifndef FUNCTION_MODEL_H
#define FUNCTION_MODEL_H

#include "MetaModel.h"
#include <vector>
#include <string>

namespace llvm {
	class Function;
	class FunctionType;
	class Constant;
}

class FunctionModel : public MetaModel
{
public:
	FunctionModel(
		std::string& 			  name,
		std::string& 			  ret_type,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list,
		std::vector<llvm::Constant*>& init_list
	);

	int find(std::string& name);

	std::string return_type;
	std::vector<std::string> type_list;
	std::vector<std::string> name_list;
	std::vector<llvm::Constant*> init_list; // 没有的置为空数组

	llvm::FunctionType* func_type;
	llvm::Function* getFunction(CodeGenContext* context);

	virtual void insertToST(CodeGenContext* context);
	virtual llvm::Value* genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual llvm::Value* genMetaCode(CodeGenContext* context);
	virtual MetaType getType();
private:
	llvm::Function* func = NULL;
};



#endif // FUNCTION_MODEL_H
