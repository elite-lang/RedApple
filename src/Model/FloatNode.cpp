/**
 * Project Untitled
 */


#include "FloatNode.h"
#include <cstdlib>
/**
 * FloatNode implementation
 */

FloatNode::FloatNode(const char* num) {
	value = atof(num);
}

FloatNode* FloatNode::Create(const char* num) {
	return new FloatNode(num);
}

NodeType FloatNode::getType() {
	return float_node_t;
}

float FloatNode::getFloat() {
	return value;
}
