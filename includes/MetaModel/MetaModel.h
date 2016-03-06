/*
* @Author: sxf
* @Date:   2015-11-13 17:08:01
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-18 23:05:21
*/


#ifndef META_MODEL_H
#define META_MODEL_H

#include <string>
#include "LLCG/lvalue.h"



enum MetaType
{
	struct_meta_t, function_meta_t, macro_meta_t, map_meta_t
};

class CodeGenContext;

/**
 * @brief 元数据模型基类
 */
class MetaModel : public lvalue
{
public:
	MetaModel(std::string);
	virtual ~MetaModel();

	virtual void insertToST(CodeGenContext* context) = 0;
	virtual void genCode(CodeGenContext* context) = 0;
	virtual void genMetaCode(CodeGenContext* context) = 0;
	virtual MetaType getMetaType() = 0;

	static MetaModel* readJson();
	static MetaModel* readMetaCode();
protected:
	std::string name;
};



#endif // META_MODEL_H
