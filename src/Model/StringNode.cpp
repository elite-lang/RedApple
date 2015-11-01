/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-01 18:48:49
*/


#include "StringNode.h"
#include <stdio.h>

void StringNode::printSelf() {
	printf("String %s", value.c_str());
}

NodeType StringNode::getType() {
	return string_node_t;
}