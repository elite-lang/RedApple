/* 
* @Author: sxf
* @Date:   2015-10-29 10:47:04
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-29 15:44:46
*/

#ifndef STRUCT_MODEL_H
#define STRUCT_MODEL_H

#include <llvm/IR/DerivedTypes.h>
#include <vector>
#include <string>

class StructModel
{
public:
	StructModel(std::vector<std::string> name_list, llvm::StructType* t) {
		this->name_list = name_list;
		struct_type = t;
	}
	std::vector<std::string> name_list;
	llvm::StructType* struct_type;
};



#endif // STRUCT_MODEL_H
