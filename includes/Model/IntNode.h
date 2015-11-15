/**
 * Project Untitled
 */


#ifndef _INTNODE_H
#define _INTNODE_H

#include "Node.h"

class IntNode: public Node {
public: 
	static IntNode* Create(const char* num);
	virtual Value* codeGen(CodeGenContext* context); 
	virtual void printSelf();
	virtual NodeType getType();
protected:
	IntNode(const char* num);
	int value;
};

#endif //_INTNODE_H