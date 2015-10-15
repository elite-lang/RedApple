/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-12 19:55:47
*/


#include "StringNode.h"
#include <stdio.h>

void StringNode::printSelf() {
	printf("String %s", value.c_str());
}

std::string StringNode::getType() {
	return std::string("StringNode");
}