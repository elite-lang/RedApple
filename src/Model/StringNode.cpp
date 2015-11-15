/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-02 15:54:15
*/


#include "StringNode.h"
#include <stdio.h>


StringNode::StringNode(const char* _value){
	this->value = _value;
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
	printf("String %s", value.c_str());
}

NodeType StringNode::getType() {
	return string_node_t;
}