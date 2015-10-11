/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-10 21:24:32
*/


#include "StringNode.h"
#include <stdio.h>

void StringNode::printSelf() {
	printf("String %s", str.c_str());
}

std::string StringNode::getType() {
	return std::string("StringNode");
}