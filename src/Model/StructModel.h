/* 
* @Author: sxf
* @Date:   2015-10-29 10:47:04
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-31 18:28:12
*/

#ifndef STRUCT_MODEL_H
#define STRUCT_MODEL_H

#include <llvm/IR/DerivedTypes.h>
#include <vector>
#include <string>
#include <iostream>

class StructModel
{
public:
	StructModel(std::vector<std::string>& name_list, llvm::StructType* t);
	int find(std::string name);
	std::vector<std::string> name_list;
	llvm::StructType* struct_type;
};



#endif // STRUCT_MODEL_H
