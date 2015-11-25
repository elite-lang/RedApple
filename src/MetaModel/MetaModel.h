/* 
* @Author: sxf
* @Date:   2015-11-13 17:08:01
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 15:31:21
*/


#ifndef META_MODEL_H
#define META_MODEL_H

#include <string>
#include "LLCG/lvalue.h"

typedef struct cJSON cJSON;

enum MetaType
{
	struct_meta_t, function_meta_t, macro_meta_t
};

class CodeGenContext;

class MetaModel : public lvalue
{
public:
	MetaModel(std::string);
	~MetaModel();

	virtual void insertToST(CodeGenContext* context) = 0;
	virtual void genCode(CodeGenContext* context) = 0;
	virtual cJSON* genJson() = 0;
	virtual void genMetaCode(CodeGenContext* context) = 0;
	virtual MetaType getMetaType() = 0;

	static MetaModel* readJson(cJSON*);
	static MetaModel* readMetaCode();

protected:
	std::string name;
};



#endif // META_MODEL_H
