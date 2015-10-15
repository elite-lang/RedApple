/**
 * Project Untitled
 */


#include "IDNode.h"

/**
 * IDNode implementation
 */

void IDNode::printSelf() {
	printf("ID %s", value.c_str());
}

std::string IDNode::getType() {
	return std::string("IDNode");
}