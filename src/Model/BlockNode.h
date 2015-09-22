/**
 * Project Untitled
 */

#ifndef _BLOCKNODE_H
#define _BLOCKNODE_H

#include "ExpressionNode.h"
#include "StatementNode.h"
#include <list>

using namespace std;

class BlockNode: public ExpressionNode {
public: 
	BlockNode();
	BlockNode(list<StatementNode*> *statement_list);

	void pushStack();
	
	void popStack();

private:
	list<StatementNode*> *statement_list;
};

#endif //_BLOCKNODE_H