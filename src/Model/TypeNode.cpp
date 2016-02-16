/*
* @Author: sxf
* @Date:   2015-11-19 13:40:13
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-21 17:12:32
*/

#include "TypeNode.h"
#include "CodeGenContext.h"
#include "elegantlist.hpp"
#include <iostream>
using namespace std;

TypeNode* TypeNode::Create(const char* name, bool is_const, bool is_source) {
	return new TypeNode(name, is_const, is_source);
}

LValue TypeNode::typeGen(CodeGenContext* context) {
	LValue t = context->FindType(str);
	if (t == NULL) {
		cerr << "找不到该类型的定义：";
		cerr << str.c_str() << endl;
		exit(1);
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
	// printf("Type ");
	if (is_const) Node::el.print("const");
	// printf("%s", str.c_str());
	Node::el.print(str);
}

TypeNode::TypeNode(const char* name, bool is_const, bool is_source) {
	this->str = name;
	this->name = name;
	this->is_const = is_const;
	this->is_source = is_source;
	if (is_source) {
		this->str.insert(str.begin(), '*');
	}
}
