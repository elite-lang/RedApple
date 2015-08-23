/**
 * Project Untitled
 */


#ifndef _FUNCDEFNODE_H
#define _FUNCDEFNODE_H

#include "DelegateDefNode.h"
#include "BlockNode.h"

class FuncDefNode: public DelegateDefNode {
public: 
	BlockNode& funcblock;
	string ret_type;
	list<string> args_type;
};

#endif //_FUNCDEFNODE_H