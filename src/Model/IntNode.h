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

	virtual void printSelf();
};

#endif //_INTNODE_H