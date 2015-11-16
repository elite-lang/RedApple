/* 
* @Author: sxf
* @Date:   2015-11-13 17:08:01
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-16 16:59:30
*/


#ifndef META_MODEL_H
#define META_MODEL_H

#include <string>

typedef struct cJSON cJSON;

namespace llvm {
	class Module;
	class Value;
	class Constant;
} // llvm

enum MetaType
{
	struct_meta_t, function_meta_t
};

class CodeGenContext;

class MetaModel
{
public:
	MetaModel(std::string);
	~MetaModel();

	virtual void insertToST(CodeGenContext* context) = 0;
	virtual llvm::Value* genCode(CodeGenContext* context) = 0;
	virtual cJSON* genJson() = 0;
	virtual llvm::Value* genMetaCode(CodeGenContext* context) = 0;
	virtual MetaType getType() = 0;

	static MetaModel* readJson(cJSON*);
	static MetaModel* readMetaCode(llvm::Module*);

protected:
	std::string name;
	llvm::Constant* geti8StrVal(llvm::Module& M, char const* str);
};



#endif // META_MODEL_H
