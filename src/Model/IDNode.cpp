/**
 * Project Untitled
 */


#include "IDNode.h"
#include "elegantlist.hpp"

/**
 * IDNode implementation
 */

IDNode* IDNode::Create(const char* _value) {
	return new IDNode(_value);
}

IDNode* IDNode::Create(char _value) {
	return new IDNode(_value);
}

IDNode::IDNode(const char* _value){
	this->value = _value;
}
IDNode::IDNode(char _value){
	this->value = _value;
}

void IDNode::printSelf() {
	// printf("ID %s", value.c_str());
	Node::el.print(value);
}

NodeType IDNode::getType() {
	return id_node_t;
}
