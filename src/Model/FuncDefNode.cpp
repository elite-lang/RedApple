/**
 * Project Untitled
 */


#include "FuncDefNode.h"

/**
 * FuncDefNode implementation
 */


FuncDefNode::FuncDefNode(string type_name, string func_name, 
				 list<VarDefNode*> *vardef_node_list, BlockNode *funcblock) {
	
}


void FuncDefNode::setTags(list<string> *funcdef_xs_node) {
	this->funcdef_xs_node = funcdef_xs_node;
}