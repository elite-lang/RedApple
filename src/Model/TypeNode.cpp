/* 
* @Author: sxf
* @Date:   2015-11-19 13:40:13
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-22 22:15:23
*/

#include "TypeNode.h"
#include "stdio.h"
#include "CodeGenContext.h"

TypeNode* TypeNode::Create(const char* name, bool is_const, bool is_source) {
	return new TypeNode(name, is_const, is_source);
}

Type* TypeNode::typeGen(CodeGenContext* context) {
	Type* t = context->FindType(str);
	if (t == NULL) {
		errs() << "找不到该类型的定义：";
		errs() << str.c_str() << "\n";
		exit(1);
	}
	if (dimension > 0) {
		// t = ArrayType::get(t, 0);
		return t->getPointerTo();
	}
	return t;
}

void TypeNode::addDimension() {
	++dimension;
	str += "[]";
}


NodeType TypeNode::getType() {
	return type_node_t;
}

string& TypeNode::getStr() {
	return str;
}

string& TypeNode::getTypeName() {
	return name;
}

void TypeNode::printSelf() {
	printf("Type ");
	if (is_const) printf("const ");
	printf("%s", str.c_str());
}

TypeNode::TypeNode(const char* name, bool is_const, bool is_source) {
	this->str = name;
	this->name = name;
	this->is_const = is_const;
	this->is_source = is_source;
}
