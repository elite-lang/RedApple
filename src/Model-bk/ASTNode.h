/**
 * Project Untitled
 */
#include<string>
using namespace std;

#ifndef _ASTNODE_H
#define _ASTNODE_H

class ASTNode {
public: 
    string* name;
	int type;
	
    string* getName();
	
	int getType();
	
	virtual void generateCode() = 0;
};

#endif //_ASTNODE_H
