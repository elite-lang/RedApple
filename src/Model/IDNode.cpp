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

NodeType IDNode::getType() {
	return id_node_t;
}