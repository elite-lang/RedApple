/**
 * Project Untitled
 */


#ifndef _BLOCKNODE_H
#define _BLOCKNODE_H

#include "ExpressionNode.h"


class BlockNode: public ExpressionNode {
public: 
	
	void pushStack();
	
	void popStack();
};

#endif //_BLOCKNODE_H