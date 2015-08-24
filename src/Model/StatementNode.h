/**
 * Project Untitled
 */


#ifndef _STATEMENTNODE_H
#define _STATEMENTNODE_H

#include "ASTNode.h"
#include <string>
using namespace std;

class StatementNode: public ASTNode {
public: 
	string name;
	int type;

	StatementNode(){}
	StatementNode(string name);
	~StatementNode();
	
	virtual void putSymbolTable();
	
	const string& getName();
	
	int getType();
};

#endif //_STATEMENTNODE_H