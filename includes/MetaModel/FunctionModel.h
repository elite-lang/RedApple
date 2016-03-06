/*
* @Author: sxf
* @Date:   2015-11-13 12:07:03
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-14 17:16:42
*/


#ifndef FUNCTION_MODEL_H
#define FUNCTION_MODEL_H

#include "MetaModel.h"
#include <vector>
#include <string>

/**
 * @brief 函数模型元类型
 */
class FunctionModel : public MetaModel
{
public:
	FunctionModel(
		std::string& 			  name,
		std::string& 			  ret_type,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list
	);

	int find(std::string& name);

	std::string return_type;
	std::vector<std::string> type_list;
	std::vector<std::string> name_list;

	LValue getFunction(CodeGenContext* context);
	LValue getFunctionType() { return func_type; }

	virtual void insertToST(CodeGenContext* context);
	virtual void genCode(CodeGenContext* context);
	virtual void genMetaCode(CodeGenContext* context);
	virtual MetaType getMetaType();
private:
	LValue func_type;
};



#endif // FUNCTION_MODEL_H
