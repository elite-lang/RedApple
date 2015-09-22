/**
 * Project Untitled
 */


#ifndef _INTNODE_H
#define _INTNODE_H

#include "ValueNode.h"
#include <string>

using namespace std;

class IntNode: public ValueNode {
public: 
	int value;

	IntNode(string num);
};

#endif //_INTNODE_H