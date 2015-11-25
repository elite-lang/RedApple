/* 
* @Author: sxf
* @Date:   2015-10-29 10:47:04
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 15:31:53
*/

#ifndef STRUCT_MODEL_H
#define STRUCT_MODEL_H

#include "MetaModel.h"
#include <vector>
#include <string>

class StructModel : public MetaModel
{
public:
	StructModel(std::string& name,
		std::vector<std::string>& type_list,
		std::vector<std::string>& name_list);

	int find(std::string& name);

	std::vector<std::string> type_list;
	std::vector<std::string> name_list;
	LValue getStruct(CodeGenContext* context);

	virtual void insertToST(CodeGenContext* context);
	virtual void genCode(CodeGenContext* context);
	virtual cJSON* genJson();
	virtual void genMetaCode(CodeGenContext* context);
	virtual MetaType getMetaType();
private:
	LValue struct_type;
};



#endif // STRUCT_MODEL_H
