/* 
* @Author: sxf
* @Date:   2015-11-13 17:08:01
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-14 16:48:26
*/


#ifndef META_MODEL_H
#define META_MODEL_H

#include <string>

typedef struct cJSON cJSON;

namespace llvm {
	class Module;
} // llvm

enum MetaType
{
	struct_meta_t, function_meta_t
};

class MetaModel
{
public:
	MetaModel(std::string);
	~MetaModel();

	virtual void insertToST(CodeGenContext* context) = 0;
	virtual Value* genCode(CodeGenContext* context) = 0;
	virtual cJSON* genJson() = 0;
	virtual Value* genMetaCode(CodeGenContext* context) = 0;
	virtual MetaType getType() = 0;

	static MetaModel* readJson(cJSON*);
	static MetaModel* readMetaCode(Module*);

private:
	std::string name;
};



#endif // META_MODEL_H
