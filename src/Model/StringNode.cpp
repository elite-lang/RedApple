/*
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-16 22:42:02
*/


#include "StringNode.h"
#include <stdio.h>
#include "elegantlist.hpp"
#include "Utils/StringEscape.h"

StringNode::StringNode(const char* _value){
	string data = _value;
	if (data[0] == '@')
		this->value = StringEscape(data.substr(2, data.length()-3));
	else
		this->value = StringEscape(data.substr(1, data.length()-2));
}

StringNode::StringNode(char _value){
	this->value = _value;
}

StringNode* StringNode::Create(const char* _value) {
	return new StringNode(_value);
}

StringNode* StringNode::Create(char _value) {
	return new StringNode(_value);
}


void StringNode::printSelf() {
	// printf("String %s", value.c_str());
	Node::el.print("\"" + value + "\"");
}

NodeType StringNode::getType() {
	return string_node_t;
}
