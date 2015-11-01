/**
 * Project Untitled
 */


#ifndef _INTNODE_H
#define _INTNODE_H

#include "Node.h"

class IntNode: public Node {
public: 
	int value;

	IntNode(const char* num);
	virtual Value* codeGen(CodeGenContext* context); 
	virtual void printSelf();
	virtual NodeType getType();
};

#endif //_INTNODE_H