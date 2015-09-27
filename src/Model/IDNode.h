/**
 * Project Untitled
 */


#ifndef _IDNODE_H
#define _IDNODE_H

#include "Node.h"
#include <string>

using namespace std;

class IDNode: public Node {
public: 
	IDNode(string name);

	void getSymbolTable();
};

#endif //_IDNODE_H