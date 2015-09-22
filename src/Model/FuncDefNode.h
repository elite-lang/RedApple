/**
 * Project Untitled
 */


#ifndef _FUNCDEFNODE_H
#define _FUNCDEFNODE_H

#include "DelegateDefNode.h"
#include "BlockNode.h"
#include "VarDefNode.h"
#include <list>
#include <string>

using namespace std;

class FuncDefNode: public DelegateDefNode {
public: 

	FuncDefNode(string type_name, string func_name, 
				 list<VarDefNode*> *vardef_node_list, BlockNode *funcblock);

	void setTags(list<string> *funcdef_xs_node);

private:
	BlockNode* funcblock;
	list<string> *funcdef_xs_node;
};

#endif //_FUNCDEFNODE_H