/**
 * Project Untitled
 */


#ifndef _IDNODE_H
#define _IDNODE_H

#include "ExpressionNode.h"
#include <string>

using namespace std;

class IDNode: public ExpressionNode {
public: 
	IDNode(string name);

	void getSymbolTable();
};

#endif //_IDNODE_H