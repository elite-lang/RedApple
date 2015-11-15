/**
 * Project Untitled
 */


#include "IntNode.h"
#include <stdlib.h>
#include <stdio.h>
/**
 * IntNode implementation
 */

IntNode* IntNode::Create(const char* num) {
	return new IntNode(num);
}

IntNode::IntNode(const char* num) {
	value = atoi(num);
}

void IntNode::printSelf() {
	printf("Int %d", value);
}

NodeType IntNode::getType() {
	return int_node_t;
}