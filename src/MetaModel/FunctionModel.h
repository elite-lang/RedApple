/* 
* @Author: sxf
* @Date:   2015-11-13 12:07:03
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 14:59:41
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

	llvm::Function* getFunction(CodeGenContext* context);
	llvm::FunctionType* getFunctionType() { return func_type; }

	virtual void insertToST(CodeGenContext* context);
	virtual llvm::Value* genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual llvm::Value* genMetaCode(CodeGenContext* context);
	virtual MetaType getType();
private:
	llvm::Function* func = NULL;
	llvm::FunctionType* func_type;
};



#endif // FUNCTION_MODEL_H
